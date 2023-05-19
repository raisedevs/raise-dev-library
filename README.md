# Raise.dev Library

[Raise.dev](https://raise.dev)'s [PlatformIO](https://platformio.org) and [Arduino](https://www.arduino.cc) library for auto-updating ESP32s and ESP8266s from the Raise.dev Console.

**This library depends on the Arduino framework (for now) so is not (yet) suitable for ESP-IDF projects.**

## Usage

**Note: unless a Raise.dev cofounder has invited you to join the Raise.dev Console, you can't use this (yet).**

1. Sign in to the Raise.dev Console and follow the onboarding steps until it tells you to use this library.

1. Setup your preferred environment (we recommend [PlatformIO](https://platformio.org)/[Visual Studio Code](https://code.visualstudio.com/)/[macOS](https://www.apple.com/macos/)/[Homebrew](https://brew.sh)):
    <details open>
    <summary>

    ### PlatformIO/Visual Studio Code/macOS/Homebrew

    </summary>

    - install Visual Studio Code from Homebrew with:

      ```console
      brew install --cask visual-studio-code
      ```

    - install the [PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode) with:

      ```console
      code --install-extension platformio.platformio-ide
      ```

    </details>
    <details>
    <summary>

    ### PlatformIO/Terminal/macOS/Homebrew

    </summary>

    - install the [PlatformIO Core (CLI)](https://docs.platformio.org/en/stable/core/index.html) from Homebrew with:

      ```console
      brew install --formula platformio
      ```

    </details>
    <details>
    <summary>

    ### PlatformIO/Visual Studio Code/Other

    </summary>

    - use the [PlatformIO IDE for VSCode installation documentation](https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation)

    </details>
    <details>
    <summary>

    ### PlatformIO/Terminal/Other

    </summary>

    - use the [PlatformIO Core (CLI) installer script](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html)

    </details>
    <details>
    <summary>

    ### Arduino CLI/Terminal/macOS/Homebrew

    </summary>

    - install the [Arduino CLI](https://arduino.github.io/arduino-cli/latest/) from Homebrew with:

      ```console
      brew install --formula arduino-cli
      ```

    </details>
    <details>
    <summary>

    ### Arduino CLI/Terminal/Other

    </summary>

    - use the [Arduino CLI install script](https://arduino.github.io/arduino-cli/latest/installation/#use-the-install-script)

    </details>

1. Add this library to your preferred environment (we recommend PlatformIO)
    <details open>
    <summary>

    ### PlatformIO

    </summary>

    - in your [`platformio.ini`](https://docs.platformio.org/en/latest/projectconf/index.html), add the [`raisedevs/RaiseDev` PlatformIO library](https://registry.platformio.org/libraries/raisedevs/RaiseDev) to [`lib_deps`](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html) with:

      ```ini
      lib_deps = raisedevs/RaiseDev
      ```

    - or, if you have other libraries already:

      ```ini
      lib_deps =
          someexisting/Library
          raisedevs/RaiseDev
      ```

      - remember to periodically upgrade `raisedevs/Raisedev` and other PlatformIO libraries with:

        ```console
        pio pkg update
        ```

    </details>

    <details>
    <summary>

    ### Arduino CLI

    </summary>

    - if you haven't already, install the ESP32 Arduino core with:

      ```console
      arduino-cli core update-index --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
      arduino-cli core install esp32:esp32 --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
      ```

    - or the ESP8266 Arduino core with:

      ```console
      arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
      arduino-cli core install esp8266:esp8266 --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
      ```

    - install the `RaiseDev` Arduino library from the [Arduino Library Registry](https://github.com/arduino/library-registry) with:

      ```console
      arduino-cli lib install RaiseDev
      ```

    </details>

1. Set `VERSION_STRING_FROM_GIT` from `git describe` and pass it to the C++ preprocessor in your preferred environment
    <details open>
    <summary>

    ### PlatformIO

    </summary>

    - in your `platformio.ini`, shell out to `git describe --dirty --tags --always` and pass the value to the C++ preprocessor through `VERSION_STRING_FROM_GIT` in your [`build_src_flags`](<https://docs.platformio.org/en/latest/projectconf/sections/env/options/build/build_src_flags.html>) with:

      ```ini
      build_src_flags = !echo '-DVERSION_STRING_FROM_GIT=\\"'$(git describe --dirty --tags --always)'\\"'
      ```

    - or if you have other values already e.g. `-Wall -Werror`:

      ```ini
      build_src_flags = !echo '-Wall -Werror -DVERSION_STRING_FROM_GIT=\\"'$(git describe --dirty --tags --always)'\\"'
      ```

    </details>

    <details>
    <summary>

    ### Arduino CLI

    </summary>

    - in your `arduino-cli compile` call, shell out to `git describe --dirty --tags --always` and pass the value to the C++ preprocessor through `VERSION_STRING_FROM_GIT` by changing:

      ```console
      arduino-cli compile --fqbn esp32:esp32:nodemcu-32s .
      ```

    - to:

      ```console
      arduino-cli compile --fqbn esp32:esp32:nodemcu-32s --build-property build.extra_flags=-DVERSION_STRING_FROM_GIT=$(git describe --tags --always) .
      ```

    - or if you are passing multiple values:

      ```console
      arduino-cli compile --fqbn esp32:esp32:nodemcu-32s --build-property "build.extra_flags=-DSOME_DEFINE=1 -DVERSION_STRING_FROM_GIT=$(git describe --tags --always)" .
      ```

    - or if you are passing multiple values and cannot use double quotes (e.g. in a `package.json`):

      ```console
      arduino-cli compile --fqbn esp32:esp32:nodemcu-32s --build-property build.extra_flags=-DSOME_DEFINE=1\\ -DVERSION_STRING_FROM_GIT=$(git describe --tags --always) .
      ```

    </details>

1. In your main C++ file (i.e. the `.cpp` or `.ino` file containing `void setup() {` and `void loop() {`, add:
    - At the top of the file to define the `RaiseDev` class and automatically initialize the `raiseDev` instance in your project add:

      ```cpp
      #include <RaiseDev.h>
      ```

    - Optionally: in your `void setup()` function to initialize the `RaiseDev` library add:

      ```cpp
      raiseDev.begin();
      ```

      - If you skip this, it will be done on the first `raiseDev.updateFirmware()` call.

    - In your `void loop()` to request an auto-update add:

      ```cpp
      raiseDev.updateFirmware("your-raise-dev-console-workspace", VERSION_STRING_FROM_GIT);
      ```

      - replace `your-raise-dev-console-workspace` with your Raise.dev Console's `Workspace`'s name.
      - trust us: this should run every time in `loop()`: it will only actually check for updates if it's been sufficiently long since the last check.

    - Different debug output levels can be set with `CORE_DEBUG_LEVEL`.
      Enable maximum debug messaging in your `platformio.ini` with e.g. `build_flags = -DCORE_DEBUG_LEVEL=5`.

1. Temporarily maximise your debugging output so you can figure out what is working (or not) for your preferred environment and architecture (we recommend PlatformIO/ESP32)
    <details open>
    <summary>

    ### PlatformIO/ESP32

    </summary>

    - in your `platformio.ini`, pass `CORE_DEBUG_LEVEL=5` to the C++ preprocessor through [`build_flags`](<https://docs.platformio.org/en/latest/projectconf/sections/env/options/build/build_flags.html>) with:

      ```ini
      build_flags = -DCORE_DEBUG_LEVEL=5
      ```

    - or if you have other values already e.g. `-Wall -Werror`:

      ```ini
      build_flags = -Wall -Werror -DCORE_DEBUG_LEVEL=5
      ```

    </details>
    <details>
    <summary>

    ### PlatformIO/ESP8266

    </summary>

    - in your `platformio.ini`, pass the necessary debug variables to the C++ preprocessor through [`build_flags`](<https://docs.platformio.org/en/latest/projectconf/sections/env/options/build/build_flags.html>) with:

      ```ini
      build_flags = -DDEBUG_ESP_PORT=Serial -DDEBUG_ESP_WIFI -DDEBUG_ESP_HTTP_CLIENT -DDEBUG_ESP_HTTP_UPDATE -DDEBUG_ESP_UPDATER -DDEBUG_RAISE_DEV
      ```

    - or if you have other values already e.g. `-Wall -Werror`:

      ```ini
      build_flags = -Wall -Werror -DDEBUG_ESP_PORT=Serial -DDEBUG_ESP_WIFI -DDEBUG_ESP_HTTP_CLIENT -DDEBUG_ESP_HTTP_UPDATE -DDEBUG_ESP_UPDATER -DDEBUG_RAISE_DEV
      ```

    </details>
    <details>
    <summary>

    ### Arduino CLI/ESP32

    </summary>

    - in your `arduino-cli compile` call, pass `CORE_DEBUG_LEVEL=5` to the C++ preprocessor by changing:

      ```console
      arduino-cli compile --fqbn esp32:esp32:nodemcu-32s --build-property build.extra_flags=-DVERSION_STRING_FROM_GIT=$(git describe --tags --always) .
      ```

    - to:

      ```console
      arduino-cli compile --fqbn esp32:esp32:nodemcu-32s --build-property "build.extra_flags=-DVERSION_STRING_FROM_GIT=$(git describe --tags --always) -DCORE_DEBUG_LEVEL=5" .
      ```

    </details>
    <details>
    <summary>

    ### Arduino CLI/ESP8266

    </summary>

    - in your `arduino-cli compile` call, pass the necessary debug variables to the C++ preprocessor by changing:

      ```console
      arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 --build-property build.extra_flags=-DVERSION_STRING_FROM_GIT=$(git describe --tags --always) .
      ```

    - to:

      ```console
      arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 --build-property "build.extra_flags=-DVERSION_STRING_FROM_GIT=$(git describe --tags --always) -DDEBUG_ESP_PORT=Serial -DDEBUG_ESP_WIFI -DDEBUG_ESP_HTTP_CLIENT -DDEBUG_ESP_HTTP_UPDATE -DDEBUG_ESP_UPDATER -DDEBUG_RAISE_DEV" .
      ```

    </details>

1. If done correctly: the `git diff` for your firmware should look something like:

   ```diff
   --- a/platformio.ini
   +++ b/platformio.ini
   @@ -15,3 +15 @@ framework = arduino
   +lib_deps = raisedevs/RaiseDev
   +build_src_flags = !echo '-DVERSION_STRING_FROM_GIT=\\"'$(git describe --dirty --tags --always)'\\"'
   +build_flags = -DCORE_DEBUG_LEVEL=5
   --- a/src/main.cpp
   +++ b/src/main.cpp
   @@ -3,0 +4 @@
   +#include <RaiseDev.h>
   @@ -238,0 +240,2 @@ void setup()
   +  raiseDev.begin();
   +
   @@ -263,0 +267,2 @@ void loop()
   +  raiseDev.updateFirmware("your-raise-dev-console-workspace", VERSION_STRING_FROM_GIT);
   ```

1. Compile and upload the new firmware to your device and monitor the debugging output

    <details open>
    <summary>

    ### PlatformIO/Visual Studio Code

    </summary>

    - press `Upload and Monitor` in the PlatformIO VSCode side panel

    </details>
    <details>
    <summary>

    ### PlatformIO/Terminal

    </summary>

    - Upload the firmware with:

      ```console
      pio run --target upload
      ```

    - Monitor the device with:

      ```console
      pio device monitor
      ```

    </details>
    <details>
    <summary>

    ### Arduino CLI/Terminal

    </summary>

    - Upload the firmware with e.g.

      ```console
      arduino-cli upload  --fqbn esp32:esp32:nodemcu-32s --port /dev/cu.usbserial-0001 .
      ```

    - Monitor the device with e.g.

      ```console
      arduino-cli monitor --port /dev/cu.usbserial-0001
      ```

    </details>

1. Ensure that the "Last Seen" for your `Device` is updated in the Raise.dev Console. This will happen even if there is no new `Version` available whenever an update is requested.

1. Whenever `raiseDev.updateFirmware()` is called, if it hasn't been too long since the last check, it will check for a new `Version` in the Raise.dev Console.
   If the version differs from `VERSION_STRING_FROM_GIT`, your microcontroller's firmware will be auto-updated to that version.

1. When you've set up the [`raisedevs/version-upload-action`](https://github.com/raisedevs/version-upload-action) GitHub Action workflow correctly, when you push a new commit and/or tag to your GitHub repository: it will automatically update all your `Device`s!

---

If you have any problems: contact [Mike](mailto:mike@raise.dev) and he'll help.

## Status

Alpha and in active development.

## Contact

[Mike McQuaid](mailto:mike@raise.dev)

## License

Licensed under the [MIT License](https://en.wikipedia.org/wiki/MIT_License).
The full license text is available in [LICENSE.txt](https://github.com/raisedevs/raise-dev-library/blob/master/LICENSE.txt).
