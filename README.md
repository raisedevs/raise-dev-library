# Raise.dev Library

Raise.dev library for auto-updating ESP8266s and ESP32s from the Rails.dev Console.

## Building and Developing (on macOS)

We recommend building this by using VSCode and its PlatformIO plugin:

```console
# Install VSCode from Homebrew Cask
$ brew install --cask visual-studio-code

# Install the PlatformIO VSCode plugin
$ code --install-extension platformio.platformio-ide
```

Alternatively, you can build from the Terminal with:

```console
# Install PlatformIO from Homebrew
$ brew install --formula platformio

# Build using PlatformIO
$ pio run
```

## Building (on Linux)

See the [GitHub Actions `build.yml` workflow using Ubuntu](https://github.com/raisedevs/raise-dev-library/blob/main/.github/workflows/build.yml).
