# Raise.dev Library

Raise.dev's PlatformIO library for auto-updating ESP8266s and ESP32s from the Raise.dev Console.

## Usage

**Note: unless a Raise.dev cofounder has invited you to join the Raise.dev Console, you can't use this (yet).**

Once you've signed up and had your `User` enabled, work through the following steps:

1. If you haven't already, install PlatformIO:
    - If you're on macOS, you can install VSCode from Homebrew with `brew install --cask visual-studio-code` and install the PlatformIO VSCode plugin with `code --install-extension platformio.platformio-ide`

    - Alternatively, if you're on macOS but would rather use the terminal, you can install the PlatformIO CLI with `brew install --formula platformio`

    - Otherwise, use the [PlatformIO installer script](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html)

2. In your [`platformio.ini`](https://docs.platformio.org/en/latest/projectconf/index.html), add [`raisedevs/RaiseDev`](https://registry.platformio.org/libraries/raisedevs/RaiseDev) to your [`lib_deps`](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html)

    - Remember to update your libraries periodically with e.g. `pio pkg update --library raisedevs/RaiseDev`!

3. In your main C++ file, add:
    - `#include <RaiseDev.h>` at the top of the file to define the `RaiseDev` class in your project

    - Add `RaiseDev raiseDev;` to initialize an instance of the `RaiseDev` class

    - Add `raiseDev.begin();` in your `void setup()` function to initialize the `RaiseDev` library

    - Add `raiseDev.updateFirmware(YOUR_RAISE_DEV_CONSOLE_ACCOUNT, YOUR_CURRENT_FIRMWARE_VERSION);` replacing `YOUR_RAISE_DEV_CONSOLE_ACCOUNT` with your `Account` and `YOUR_CURRENT_FIRMWARE_VERSION` with your current project version

    - Different debug output levels can be set with `CORE_DEBUG_LEVEL`.
      Enable maximum debug messaging in your `platformio.ini` with e.g. `build_flags = -DCORE_DEBUG_LEVEL=5`.

    - If you're struggling to decide how to set `YOUR_CURRENT_FIRMWARE_VERSION`, consider using `git describe` in your `platformio.ini` with e.g. `build_flags = !echo '-Wall -Werror -DYOUR_CURRENT_FIRMWARE_VERSION=\\"'$(git describe --tags --always)'\\"'`

4. Whenever `raiseDev.updateFirmware()` is called, if it hasn't been too long since the last check, it will check for a new `Version` in the Raise.dev Console.
   If the version differs from `YOUR_CURRENT_FIRMWARE_VERSION`, your microcontroller's firmware will be auto-updated to that version.

5. Add [`raisedevs/version-upload-action`](https://github.com/raisedevs/version-upload-action) to your project's GitHub Actions workflow
    - If you haven't set up a workflow yet, don't worry!
      Check out the [workflow in this repository](https://github.com/raisedevs/raise-dev-library/blob/main/.github/workflows/build.yml) for an example of how to use PlatformIO in GitHub Actions.
      Note that you should use `pio run` instead of `pio ci` for building a `Firmware` GitHub repository.

6. If you haven't already, add the relevant `Account` and `Firmware` in the Raise.dev Console and connect the GitHub App to your GitHub Repository for your `Firmware`.

7. If you've set everything up correctly, when you push a new commit and/or tag to your GitHub repository: it will automatically update all your `Device`s!

If you have any problems: contact [Mike](mailto:mike@raise.dev) and he'll help.

## Status

Alpha and in active development.

## Contact

[Mike McQuaid](mailto:mike@raise.dev)

## License

Licensed under the [MIT License](https://en.wikipedia.org/wiki/MIT_License).
The full license text is available in [LICENSE.txt](https://github.com/raisedevs/raise-dev-library/blob/master/LICENSE.txt).
