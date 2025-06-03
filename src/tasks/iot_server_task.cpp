/**
 * @file       iot_server_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for IOT Server Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "iot_server_task.h"
#include "bsp_gpio.h"
#include "globals.h"

#include <Arduino_MQTT_Client.h>
#include <WiFi.h>

#include <Attribute_Request.h>
#include <Server_Side_RPC.h>
#include <Shared_Attribute_Update.h>
#include <ThingsBoard.h>

#include <Espressif_Updater.h>
#include <OTA_Firmware_Update.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

// Firmware Title & Version
constexpr char CURRENT_FIRMWARE_TITLE[]   = "SMART_HOME";
constexpr char CURRENT_FIRMWARE_VERSION[] = "1.0.0";

// Maximum amount of retries we attempt to download each firmware chunck over MQTT
constexpr uint8_t FIRMWARE_FAILURE_RETRIES = 12U;

// Size of each firmware chunck downloaded over MQTT,
// increased packet size, might increase download speed
constexpr uint16_t FIRMWARE_PACKET_SIZE = 4096U;

constexpr char     TOKEN[]          = COREIOT_TOKEN;
constexpr char     COREIOT_SERVER[] = "app.coreiot.io";
constexpr uint16_t COREIOT_PORT     = 1883U;

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
constexpr uint16_t MAX_MESSAGE_SEND_SIZE    = 512U;
constexpr uint16_t MAX_MESSAGE_RECEIVE_SIZE = 512U;
constexpr size_t   MAX_ATTRIBUTES           = 10U;

constexpr const char RPC_SWITCH_METHOD[]                = "setSwitchState";
constexpr char       RPC_REQUEST_CALLBACK_METHOD_NAME[] = "getCurrentTime";
constexpr uint8_t    MAX_RPC_SUBSCRIPTIONS              = 15U;
constexpr uint8_t    MAX_RPC_REQUEST                    = 15U;
constexpr uint64_t   REQUEST_TIMEOUT_MICROSECONDS       = 15000U * 1000U;

constexpr int16_t telemetrySendInterval = 30000U;

// DHT20 / SHT40
constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[]    = "humidity";

// Light Sensor
constexpr char ILLUMINANCE_KEY[] = "illuminance";

// BMP280 Sensor / SHT40
constexpr char PRESSURE_KEY[] = "pressure";
constexpr char ALTITUDE_KEY[] = "altitude";

// Attribute names
constexpr char LED_STATE_ATTR[]  = "ledState";
constexpr char FAN_SPEED_ATTR[]  = "fanSpeed";
constexpr char DOOR_STATE_ATTR[] = "doorState";
constexpr char FW_TITLE_ATTR[]   = "fw_title";
constexpr char FW_VERSION_ATTR[] = "fw_version";

// Flag to handle devices state and values
volatile bool ledStateChanged  = false;
volatile bool fanSpeedChanged  = false;
volatile bool doorStateChanged = false;

// Current devices states/values
volatile bool ledState  = false;
volatile int  fanSpeed  = 0;
volatile bool doorState = false;

// Statuses for updating
bool currentFWSent = false;

// Initialize used APIs
OTA_Firmware_Update<>                                   ota;
Server_Side_RPC<MAX_RPC_SUBSCRIPTIONS, MAX_RPC_REQUEST> rpc;
Attribute_Request<2U, MAX_ATTRIBUTES>                   attr_request;
Shared_Attribute_Update<3U, MAX_ATTRIBUTES>             shared_update;

const std::array<IAPI_Implementation *, 4U> apis = {&ota, &rpc, &attr_request, &shared_update};

// List of shared attributes for subscribing to their updates
constexpr std::array<const char *, 8U> SHARED_ATTRIBUTES_LIST = {FAN_SPEED_ATTR, FW_TITLE_ATTR,
                                                                 FW_VERSION_ATTR};

// List of client attributes for requesting them (Using to initialize device states)
constexpr std::array<const char *, 2U> CLIENT_ATTRIBUTES_LIST = {LED_STATE_ATTR, DOOR_STATE_ATTR};

WiFiClient          wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE, Default_Max_Stack_Size, apis);
Espressif_Updater<> updater;

bool subscribed = false;

/* Private function definitions ------------------------------------------- */
#ifdef OTA_UPDATE_MODULE
void update_starting_callback()
{
  // Nothing to do
}

/// @brief End callback method that will be called as soon as the OTA firmware update, either finished
/// successfully or failed. Is meant to allow to either restart the device if the udpate was successfull or
/// to restart any stopped services before the update started in the subscribed update_starting_callback
/// @param success Either true (update successful) or false (update failed)
void finished_callback(const bool &success)
{
  if (success)
  {
  #ifdef DEBUG_PRINT
    Serial.println("Done, Reboot now");
  #endif // DEBUG_PRINT

  #ifdef ESP32
    esp_restart();
  #endif // ESP32

    return;
  }
  #ifdef DEBUG_PRINT
  Serial.println("Downloading firmware failed");
  #endif // DEBUG_PRINT
}

/// @brief Progress callback method that will be called every time our current progress of downloading the
/// complete firmware data changed, meaning it will be called if the amount of already downloaded chunks
/// increased. Is meant to allow to display a progress bar or print the current progress of the update into
/// the console with the currently already downloaded amount of chunks and the total amount of chunks
/// @param current Already received and processs amount of chunks
/// @param total Total amount of chunks we need to receive and process until the update has completed
void progress_callback(const size_t &current, const size_t &total)
{
  #ifdef DEBUG_PRINT
  Serial.printf("Progress %.2f%%\n", static_cast<float>(current * 100U) / total);
  #endif // DEBUG_PRINT
}
#endif // OTA_UPDATE_MODULE

void processSetLedState(const JsonVariantConst &data, JsonDocument &response)
{
  ledState = data;

#ifdef DEBUG_PRINT
  Serial.print("Received set led state RPC. New state: ");
  Serial.println(ledState);
#endif // DEBUG_PRINT

  StaticJsonDocument<1> response_doc;
  // Returning current state as response
  response_doc["newState"] = (bool) ledState;
  response.set(response_doc);

  ledStateChanged = true;
}

void processSetDoorState(const JsonVariantConst &data, JsonDocument &response)
{
  doorState = data;

#ifdef DEBUG_PRINT
  Serial.print("Received set door state RPC. New state: ");
  Serial.println(doorState);
#endif // DEBUG_PRINT

  StaticJsonDocument<1> response_doc;
  // Returning current state as response
  response_doc["newState"] = (bool) doorState;
  response.set(response_doc);

  doorStateChanged = true;
}

const std::array<RPC_Callback, 4U> rpcCallbacks = {RPC_Callback{"setLedValue", processSetLedState},
                                                   RPC_Callback{"setDoorState", processSetDoorState}};

/// @brief Shared attribute update callback
/// @param data New value of shared attributes which is changed
void processSharedAttributes(const JsonObjectConst &data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    const char *key = it->key().c_str();
    // OTA UPDATE
    if (strcmp(key, "fw_title") == 0 || strcmp(key, "fw_version") == 0)
    {
      String fwTitle   = data["fw_title"] | "";
      String fwVersion = data["fw_version"] | "";

      if (fwTitle == CURRENT_FIRMWARE_TITLE && compareVersion(CURRENT_FIRMWARE_VERSION, fwVersion) < 0)
      {
#ifdef DEBUG_PRINT
        Serial.println("New firmware available! Initiating OTA update...");
#endif // DEBUG_PRINT

        const OTA_Update_Callback callback(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, &updater,
                                           &finished_callback, &progress_callback, &update_starting_callback,
                                           FIRMWARE_FAILURE_RETRIES, FIRMWARE_PACKET_SIZE);
        ota.Start_Firmware_Update(callback);
      }
    }
    // FAN SPEED
    else if (strcmp(key, FAN_SPEED_ATTR) == 0)
    {
      const uint8_t newFanSpeed = it->value().as<uint8_t>();
      if (newFanSpeed <= 100)
      {
        fanSpeed = newFanSpeed;
      }
      else
      {
        fanSpeed = 100;
      }
#ifdef DEBUG_PRINT
      Serial.printf("Fan speed is set to: %d\n", fanSpeed);
#endif // DEBUG_PRINT

      fanSpeedChanged = true;
    }
    // DOOR STATE
    else if (strcmp(key, DOOR_STATE_ATTR) == 0)
    {
      doorState        = it->value().as<bool>();
      doorStateChanged = true;

#ifdef DEBUG_PRINT
      Serial.printf("Door state updated: %d \n", doorState);
#endif // DEBUG_PRINT
    }
  }
}

void processClientAttributes(const JsonObjectConst &data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    if (strcmp(it->key().c_str(), LED_STATE_ATTR) == 0)
    {
      ledState = it->value().as<bool>();
      switch ((int) ledState)
      {
        case 0:
          /*
            // BUILT-IN LED
            bspGpioDigitalWrite(LED_BUILTIN, HIGH); // OFF
          */
          // NEOPIXEL --- OFF
          rgb.setPixelColor(0, rgb.Color(0, 0, 0));
          rgb.setPixelColor(1, rgb.Color(0, 0, 0));
          rgb.setPixelColor(2, rgb.Color(0, 0, 0));
          rgb.setPixelColor(3, rgb.Color(0, 0, 0));
          rgb.show();

          break;
        case 1:
          /*
            // BUILT-IN LED
            bspGpioDigitalWrite(LED_BUILTIN, LOW); // ON
          */
          // NEOPIXEL --- ON
          rgb.setPixelColor(0, rgb.Color(255, 102, 0));
          rgb.setPixelColor(1, rgb.Color(255, 102, 0));
          rgb.setPixelColor(2, rgb.Color(255, 102, 0));
          rgb.setPixelColor(3, rgb.Color(255, 102, 0));
          rgb.show();

          break;
        default:
          /*
          // BUILT-IN LED
          bspGpioDigitalWrite(LED_BUILTIN, HIGH); // Fail-safe
          */
          // NEOPIXEL --- OFF
          rgb.setPixelColor(0, rgb.Color(0, 0, 0));
          rgb.setPixelColor(1, rgb.Color(0, 0, 0));
          rgb.setPixelColor(2, rgb.Color(0, 0, 0));
          rgb.setPixelColor(3, rgb.Color(0, 0, 0));
          rgb.show();

          break;
      }
    }

    else if (strcmp(it->key().c_str(), DOOR_STATE_ATTR) == 0)
    {
      doorState = it->value().as<bool>();
      if (doorState)
      {
        doorServo.setDoorStatus(doorState);
        doorServo.writePos(180);
        vTaskDelay(pdMS_TO_TICKS(15));
      }
      else
      {
        doorServo.setDoorStatus(doorState);
        doorServo.writePos(0);
        vTaskDelay(pdMS_TO_TICKS(15));
      }
    }
  }
}

// Attribute request did not receive a response in the expected amount of microseconds
void requestTimedOut()
{
#ifdef DEBUG_PRINT
  Serial.printf("Attribute request not receive response in (%llu) microseconds. Ensure client is connected "
                "to the MQTT broker and that the keys actually exist on the target device\n",
                REQUEST_TIMEOUT_MICROSECONDS);
#endif // DEBUG_PRINT
}

const Shared_Attribute_Callback<MAX_ATTRIBUTES>
attributes_callback(&processSharedAttributes, SHARED_ATTRIBUTES_LIST.cbegin(), SHARED_ATTRIBUTES_LIST.cend());

const Attribute_Request_Callback<MAX_ATTRIBUTES>
attribute_shared_request_callback(&processSharedAttributes, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut,
                                  SHARED_ATTRIBUTES_LIST);

const Attribute_Request_Callback<MAX_ATTRIBUTES>
attribute_client_request_callback(&processClientAttributes, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut,
                                  CLIENT_ATTRIBUTES_LIST);
/* Task definitions ------------------------------------------- */

void iotServerTask(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED && !tb.connected())
    {
      if (!tb.connect(COREIOT_SERVER, TOKEN, COREIOT_PORT))
      {
        Serial.println("Failed to connect");
        // Yield or delay to prevent watchdog trigger
        for (int i = 0; i < 10; ++i)
        {
          vTaskDelay(pdMS_TO_TICKS(100)); // delay in smaller chunks
          taskYIELD();                    // explicitly yield
        }
        continue;
      }
      else
      {
#ifdef DEBUG_PRINT
        Serial.println("Connected to IoT server!");
#endif // DEBUG_PRINT
      }

      // Send WiFi attributes
      tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
      tb.sendAttributeData("ssid", WiFi.SSID().c_str());
      tb.sendAttributeData("bssid", WiFi.BSSIDstr().c_str());
      tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());
      tb.sendAttributeData("channel", WiFi.channel());

#ifdef DEBUG_PRINT
      Serial.println("Subscribing for RPC...");
#endif // DEBUG_PRINT
      if (!rpc.RPC_Subscribe(rpcCallbacks.cbegin(), rpcCallbacks.cend()))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to subscribe for RPC");
#endif // DEBUG_PRINT
        return;
      }

      if (!shared_update.Shared_Attributes_Subscribe(attributes_callback))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to subscribe for shared attribute updates");
#endif // DEBUG_PRINT
        return;
      }

#ifdef DEBUG_PRINT
      Serial.println("Subscribe shared attributes done");
#endif // DEBUG_PRINT

      // Request current value of shared attributes
      if (!attr_request.Shared_Attributes_Request(attribute_shared_request_callback))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to request for shared attributes (fan speed)");
#endif // DEBUG_PRINT
        return;
      }

      // Request current states of client attributes
      if (!attr_request.Client_Attributes_Request(attribute_client_request_callback))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to request for client attributes");
#endif // DEBUG_PRINT
        return;
      }

// OTA UPDATE
#ifdef OTA_UPDATE_MODULE
      if (!currentFWSent)
      {
        currentFWSent = ota.Firmware_Send_Info(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
      }
#endif // OTA_UPDATE_MODULE
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // Retry after 5 seconds
  }
}

void sendTelemetryTask(void *pvParameters)
{
  TickType_t lastWakeTime = xTaskGetTickCount();

  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      if (tb.connected())
      {
#ifdef SHT4X_MODULE
        sht40.update();
        float temperature = sht40.getTemperature();
        float humidity    = sht40.getHumidity();

        if (!(isnan(temperature) || isnan(humidity)))
        {
  #ifdef DEBUG_PRINT
          Serial.print("Temperature: ");
          Serial.print(temperature);
          Serial.print(" °C, Humidity: ");
          Serial.print(humidity);
          Serial.println(" %");
  #endif // DEBUG_PRINT

          tb.sendTelemetryData(TEMPERATURE_KEY, temperature);
          tb.sendTelemetryData(HUMIDITY_KEY, humidity);
        }
#endif // SHT4X_MODULE

#ifdef BMP280_MODULE
        bmp280.update();
        float pressure = bmp280.getPressure();
        float altitude = bmp280.getAltitude();
        // float temperature = bmp280.getTemperature();
        if (!(isnan(pressure) || isnan(altitude)))
        {
  #ifdef DEBUG_PRINT
          Serial.print("Pressure: ");
          Serial.print(pressure);
          Serial.print(" Pa, Altitude: ");
          Serial.print(altitude);
          Serial.println(" m");
  #endif // DEBUG_PRINT
          tb.sendTelemetryData(PRESSURE_KEY, pressure);
          tb.sendTelemetryData(ALTITUDE_KEY, altitude);
        }
#endif // BMP280_MODULE

#ifdef LIGHT_SENSOR_MODULE
        lightSensor.read();
        float illuminance = lightSensor.getLightValuePercentage();
        if (!(isnan(illuminance)))
        {
  #ifdef DEBUG_PRINT
          Serial.print("Illuminance: ");
          Serial.print(illuminance);
          Serial.println(" lux");
  #endif // DEBUG_PRINT
          tb.sendTelemetryData(ILLUMINANCE_KEY, illuminance);
        }
#endif // LIGHT_SENSOR_MODULE

        // Send WiFi signal strength
        tb.sendAttributeData("rssi", WiFi.RSSI());
      }
    }
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(telemetrySendInterval));
  }
}

void updateDevicesStateTask(void *pvParameters)
{
  for (;;)
  {
    // Update LED
    if (ledStateChanged)
    {
      ledStateChanged = false;

      switch ((int) ledState)
      {
        case 0:
          /*
            // BUILT-IN LED
            bspGpioDigitalWrite(LED_BUILTIN, HIGH); // OFF
          */
          // NEOPIXEL --- OFF
          rgb.setPixelColor(0, rgb.Color(0, 0, 0));
          rgb.setPixelColor(1, rgb.Color(0, 0, 0));
          rgb.setPixelColor(2, rgb.Color(0, 0, 0));
          rgb.setPixelColor(3, rgb.Color(0, 0, 0));
          rgb.show();

          break;
        case 1:
          /*
            // BUILT-IN LED
            bspGpioDigitalWrite(LED_BUILTIN, LOW); // ON
          */
          // NEOPIXEL --- ON
          rgb.setPixelColor(0, rgb.Color(255, 102, 0));
          rgb.setPixelColor(1, rgb.Color(255, 102, 0));
          rgb.setPixelColor(2, rgb.Color(255, 102, 0));
          rgb.setPixelColor(3, rgb.Color(255, 102, 0));
          rgb.show();

          break;
        default:
          /*
          // BUILT-IN LED
          bspGpioDigitalWrite(LED_BUILTIN, HIGH); // Fail-safe
          */
          // NEOPIXEL --- OFF
          rgb.setPixelColor(0, rgb.Color(0, 0, 0));
          rgb.setPixelColor(1, rgb.Color(0, 0, 0));
          rgb.setPixelColor(2, rgb.Color(0, 0, 0));
          rgb.setPixelColor(3, rgb.Color(0, 0, 0));
          rgb.show();

          break;
      }
      tb.sendAttributeData(LED_STATE_ATTR, ledState);
    }

    // Update Door
#ifdef SERVO_MODULE
    if (doorStateChanged)
    {
      doorStateChanged = false;

      if (doorState)
      {
        doorServo.setDoorStatus(doorState);
        doorServo.writePos(180);
        vTaskDelay(pdMS_TO_TICKS(15));
      }
      else
      {
        doorServo.setDoorStatus(doorState);
        doorServo.writePos(0);
        vTaskDelay(pdMS_TO_TICKS(15));
      }

      tb.sendAttributeData(DOOR_STATE_ATTR, doorState);
    }
#endif // SERVO_MODULE

// Update Fan
#ifdef MINI_FAN_MODULE
    if (fanSpeedChanged)
    {
      fanSpeedChanged = false;

      miniFan.setFanSpeedPercentage(fanSpeed);
    }
#endif // MINI_FAN_MODULE

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void thingsboardLoopTask(void *pvParameters)
{
  for (;;)
  {
    tb.loop();
    vTaskDelay(pdMS_TO_TICKS(50)); // Short delay for processing
  }
}

void iotServerSetup()
{
  xTaskCreate(iotServerTask, "IOT Server Task", 8192, NULL, 1, NULL);
  xTaskCreate(sendTelemetryTask, "Send Telemetry Task", 8192, NULL, 1, NULL);
  xTaskCreate(thingsboardLoopTask, "ThingsBoard Loop Task", 8192, NULL, 1, NULL);
  xTaskCreate(updateDevicesStateTask, "Update Devices Status Task", 4096, NULL, 1, NULL);
}
/* End of file -------------------------------------------------------- */