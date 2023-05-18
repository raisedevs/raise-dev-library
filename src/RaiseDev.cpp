#include "RaiseDev.h"

#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
// Emulate the nicer default ESP32 APIs on ESP8266
auto httpUpdate = ESPhttpUpdate;

#if defined(DEBUG_ESP_PORT) && defined(DEBUG_RAISE_DEV)
const void printf_with_newline(const char *message, ...)
{
  va_list args;
  va_start(args, message);
  DEBUG_ESP_PORT.printf(message, args);
  va_end(args);
  DEBUG_ESP_PORT.println();
}

#define log_d(message, ...) printf_with_newline(message, ##__VA_ARGS__);
#define log_e(message, ...) printf_with_newline(message, ##__VA_ARGS__);
#define log_i(message, ...) printf_with_newline(message, ##__VA_ARGS__);
#else
#define log_d(message, ...)
#define log_e(message, ...)
#define log_i(message, ...)
#endif
#endif // defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)

const String RAISE_DEV_CONSOLE_DOMAIN =
    "https://console.raise.dev";
const char *RAISE_DEV_CONSOLE_ROOT_CA_CERTIFICATE =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDzTCCArWgAwIBAgIQCjeHZF5ftIwiTv0b7RQMPDANBgkqhkiG9w0BAQsFADBa\n"
    "MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl\n"
    "clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTIw\n"
    "MDEyNzEyNDgwOFoXDTI0MTIzMTIzNTk1OVowSjELMAkGA1UEBhMCVVMxGTAXBgNV\n"
    "BAoTEENsb3VkZmxhcmUsIEluYy4xIDAeBgNVBAMTF0Nsb3VkZmxhcmUgSW5jIEVD\n"
    "QyBDQS0zMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEua1NZpkUC0bsH4HRKlAe\n"
    "nQMVLzQSfS2WuIg4m4Vfj7+7Te9hRsTJc9QkT+DuHM5ss1FxL2ruTAUJd9NyYqSb\n"
    "16OCAWgwggFkMB0GA1UdDgQWBBSlzjfq67B1DpRniLRF+tkkEIeWHzAfBgNVHSME\n"
    "GDAWgBTlnVkwgkdYzKz6CFQ2hns6tQRN8DAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0l\n"
    "BBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIGA1UdEwEB/wQIMAYBAf8CAQAwNAYI\n"
    "KwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5j\n"
    "b20wOgYDVR0fBDMwMTAvoC2gK4YpaHR0cDovL2NybDMuZGlnaWNlcnQuY29tL09t\n"
    "bmlyb290MjAyNS5jcmwwbQYDVR0gBGYwZDA3BglghkgBhv1sAQEwKjAoBggrBgEF\n"
    "BQcCARYcaHR0cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQUzALBglghkgBhv1sAQIw\n"
    "CAYGZ4EMAQIBMAgGBmeBDAECAjAIBgZngQwBAgMwDQYJKoZIhvcNAQELBQADggEB\n"
    "AAUkHd0bsCrrmNaF4zlNXmtXnYJX/OvoMaJXkGUFvhZEOFp3ArnPEELG4ZKk40Un\n"
    "+ABHLGioVplTVI+tnkDB0A+21w0LOEhsUCxJkAZbZB2LzEgwLt4I4ptJIsCSDBFe\n"
    "lpKU1fwg3FZs5ZKTv3ocwDfjhUkV+ivhdDkYD7fa86JXWGBPzI6UAPxGezQxPk1H\n"
    "goE6y/SJXQ7vTQ1unBuCJN0yJV0ReFEQPaA1IwQvZW+cwdFD19Ae8zFnWSfda9J1\n"
    "CZMRJCQUzym+5iPDuI9yP+kHyCREU3qzuWFloUwOxkgAyXVjBYdwRVKD05WdRerw\n"
    "6DEdfgkfCv4+3ao8XnTSrLE=\n"
    "-----END CERTIFICATE-----\n";
const long GMT_OFFSET_SECONDS = 0;
const int DAYLIGHT_OFFSET_SECONDS = 0;
const char *NTP_SERVER_PRIMARY = "pool.ntp.org";
const char *NTP_SERVER_SECONDARY = "time.nist.gov";
const unsigned long MINIMUM_UNIX_TIME = 8 * 3600 * 2;
const unsigned long DEFAULT_UPDATE_INTERVAL_MILLISECONDS = 1000 * 15;
const unsigned long FAILED_UPDATE_INTERVAL_MILLISECONDS = 1000 * 60 * 2;

// Callback when HTTPUpdate starts
const void httpUpdateOnStart()
{
  log_i("Firmware download started");
}

// Callback when HTTPUpdate ends
const void httpUpdateOnEnd()
{
  log_i("Firmware download completed");
}

// Callback when HTTPUpdate progresses
const void httpUpdateOnProgress(const int current_bytes, const int total_bytes)
{
  log_d("Firmware downloading: (%d/%d bytes)...", current_bytes, total_bytes);
}

// Callback when HTTPUpdate has an error
const void httpUpdateOnError(const int error_code)
{
  log_e("Firmware download error (HttpUpdate code %d)", error_code);
}

const void setSSLRootCert(WiFiClientSecure &wifiClientSecure)
{
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  wifiClientSecure.setCACert(RAISE_DEV_CONSOLE_ROOT_CA_CERTIFICATE);
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  wifiClientSecure.setTrustAnchors(new BearSSL::X509List(RAISE_DEV_CONSOLE_ROOT_CA_CERTIFICATE));
#endif
}

const void RaiseDev::begin()
{
  // Don't run begin() twice.
  if (begin_method_called)
  {
    return;
  }

  // So we can validate HTTPS connections.
  setSSLRootCert(wifiClientSecure);

  // Reading data over SSL may be slow, use a longer timeout (in seconds).
  wifiClientSecure.setTimeout(10);

  // Setup HTTP Updater callback methods.
  httpUpdate.onStart(httpUpdateOnStart);
  httpUpdate.onEnd(httpUpdateOnEnd);
  httpUpdate.onProgress(httpUpdateOnProgress);
  httpUpdate.onError(httpUpdateOnError);

  // Follow correctly implemented redirects.
  httpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  begin_method_called = true;
}

/// @brief Updates the current firmware to one from Raise.dev
/// @param workspace A String containing the Raise.dev workspace name
/// @param current_firmware_version A String to ensure that the firmware is not repeatedly updated to the same version
/// @return nothing, this method will restart on success
const void RaiseDev::updateFirmware(const String workspace, const String current_firmware_version)
{
  // Run begin() if consumers forgot to run it.
  if (!begin_method_called)
  {
    begin();
  }

  // Correct system time is required for SSL certificate validation
  if (!setClockViaNTP())
  {
    return;
  }

  // Wait longer before checking for updates if the last one failed.
  unsigned long update_interval_microseconds = DEFAULT_UPDATE_INTERVAL_MILLISECONDS;
  if (last_update_attempt_return_code == HTTP_UPDATE_FAILED)
  {
    update_interval_microseconds = FAILED_UPDATE_INTERVAL_MILLISECONDS;
  }

  // Check if we've updated in the last interval and early return if so.
  // Will always check immediately on first boot.
  const unsigned long current_milliseconds = millis();
  const unsigned long waited_since_last_update_milliseconds = current_milliseconds - last_update_attempt_milliseconds;
  if (waited_since_last_update_milliseconds < update_interval_microseconds)
  {
    return;
  }
  last_update_attempt_milliseconds = current_milliseconds;

  const String updater_url = String(RAISE_DEV_CONSOLE_DOMAIN + "/workspaces/" + workspace + "/updater");
  log_i("Updating current firmware version %s from %s", current_firmware_version.c_str(), updater_url.c_str());

  // This will update and reboot automatically on a successful, new firmware download.
  last_update_attempt_return_code = httpUpdate.update(
      wifiClientSecure, updater_url, current_firmware_version);

  switch (last_update_attempt_return_code)
  {
  case HTTP_UPDATE_FAILED:
    log_e("Firmware download error: %s (HttpUpdate code %d)",
          httpUpdate.getLastErrorString().c_str(),
          httpUpdate.getLastError());
    break;
  case HTTP_UPDATE_NO_UPDATES:
    log_i("Firmware already up-to-date");
    break;
  case HTTP_UPDATE_OK:
    // This should not happen, instead it should have already begun updating and rebooting.
    log_i("Firmware updated OK");
    break;
  }
}

// Set the system time using NTP
// Necessary to perform SSL validation
const bool RaiseDev::setClockViaNTP()
{
  if (ntp_completed)
  {
    return true;
  }

  if (!ntp_started)
  {
    // Set device time to UTC.
    configTime(GMT_OFFSET_SECONDS, DAYLIGHT_OFFSET_SECONDS, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);
    log_i("NTP time sync requested");
    ntp_started = true;
  }

  time_t current_unix_time = time(nullptr);

  // Check that the system time is above a sensible floor.
  if (current_unix_time < MINIMUM_UNIX_TIME)
  {
    return false;
  }

  // Convert Unix time to time structure.
  struct tm current_time_info;
  gmtime_r(&current_unix_time, &current_time_info);

  // Output time as a human-readable string.
  log_i("System time set via NTP to %s", asctime(&current_time_info));

  ntp_completed = true;

  return true;
}

// Define a global instance (unless told not to)
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_RAISE_DEV)
RaiseDev raiseDev;
#endif
