#ifndef ___RAISE_DEV_H___
#define ___RAISE_DEV_H___

#include <time.h>

#if defined(ESP32)
#include <HTTPUpdate.h>
#elif defined(ESP8266)
#include <ESP8266httpUpdate.h>
#else
#error "Only ESP32 or ESP8266 are currently supported!"
#endif

// RaiseDev - API for Raise.dev
class RaiseDev
{
public:
  const void begin();
  const void updateFirmware(String workspace, String current_firmware_version);

private:
  const bool setClockViaNTP();

  WiFiClientSecure wifiClientSecure;
  bool begin_method_called = false;
  unsigned long last_update_attempt_milliseconds = 0;
  bool ntp_started = false;
  bool ntp_completed = false;
  t_httpUpdate_return last_update_attempt_return_code = HTTP_UPDATE_OK;
};

#endif // ___RAISE_DEV_H___
