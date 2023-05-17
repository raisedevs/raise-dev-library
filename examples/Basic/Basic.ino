// Include Arduino library.
#include <Arduino.h>

// Include WiFi library.
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error "Only ESP32 or ESP8266 currently supported!"
#endif

// Include RaiseDev library.
#include <RaiseDev.h>

// Setup method: run once.
void setup()
{
  // Setup connection to (hardcoded) WiFi nextwork.
  WiFi.begin("MY_WIFI_SSID", "MY_WIFI_PASSWORD");

  // Setup RaiseDev library.
  raiseDev.begin();
}

// Loop method: run repeatedly.
void loop()
{
  // Wait for WiFi connection.
  if (WiFi.status() != WL_CONNECTED)
  {
    // Yield to other tasks.
    yield();

    // Wait a second before trying again.
    delay(1000);

    // Go back to the top of the loop.
    return;
  }

  // Auto-update firmware if a newer one found on RaiseDev Console.
  raiseDev.updateFirmware(
      // Under the Workspace named MY_RAISE_DEV_CONSOLE_WORKSPACE.
      "MY_RAISE_DEV_CONSOLE_WORKSPACE",
      // With a version string created from `git describe` defined in platformio.ini.
      VERSION_STRING_FROM_GIT);
}
