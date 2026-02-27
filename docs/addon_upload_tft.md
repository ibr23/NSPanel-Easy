# Add-on: Upload TFT

## Description

This add-on enables your panel to upload TFT firmware files to the Nextion display,
either automatically after boot or manually via a button in Home Assistant.

The Nextion display has its own controller, memory, and firmware.
The UI elements and firmware are compiled into a TFT file which must be loaded
onto the display for proper functionality.
This add-on handles downloading the TFT file from a remote server (or a local URL)
and transferring it to the display over serial.

> [!NOTE]
> This add-on is **included by default** when using the standard `nspanel_esphome.yaml` package.
> You only need to reference it explicitly if you are using
> [granular package selection](install.md#granular-package-selection)
> or if you previously removed it (e.g., for memory optimization with Bluetooth Proxy).

### How it works

When the device boots and connects to Home Assistant, a version check compares
the current TFT version on the display with the expected version from the firmware.
Depending on your configuration, the add-on will either:

1. **Automatically upload** the matching TFT file after a configurable wait period, or
2. **Wait for a manual trigger** via the "Update TFT display" button in Home Assistant.

The TFT file source is determined by the "Update TFT display - Model" selector
on the device's page in Home Assistant (**Settings** > **Devices & services** > **ESPHome**).

## Installation

This add-on is already part of the default `nspanel_esphome.yaml` package,
so most users do not need to change anything.

If you are using granular package selection, add the reference to the
`addon_upload_tft` file in your ESPHome settings in the `packages` section,
after the core and standard packages:

```yaml
substitutions:
  # Settings - Editable values
  device_name: "YOUR_NSPANEL_NAME"
  friendly_name: "Your panel's friendly name"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
  ota_password: ""  # Optional: set OTA password, or use ${wifi_password} for backward compatibility (see migration guide)

  # Add-on configuration (if needed)
  ## Upload TFT
  upload_tft_automatically: true

# Customization area
##### My customization - Start #####
##### My customization - End #####

# Basic and optional configurations
packages:
  remote_package:
    url: https://github.com/edwardtfn/NSPanel-Easy
    ref: main
    refresh: 300s
    files:
      - nspanel_esphome.yaml # Basic package (already includes this add-on)
      # Optional advanced and add-on configurations
      # - esphome/nspanel_esphome_addon_climate_cool.yaml
      # - esphome/nspanel_esphome_addon_climate_heat.yaml
      # - esphome/nspanel_esphome_addon_climate_dual.yaml
      # - esphome/nspanel_esphome_addon_cover.yaml
      # - esphome/nspanel_esphome_addon_display_light.yaml
```

If you use granular package selection (individual files instead of `nspanel_esphome.yaml`),
add the Upload TFT package explicitly:

```yaml
packages:
  remote_package:
    url: https://github.com/edwardtfn/NSPanel-Easy
    ref: main
    refresh: 300s
    files:
      - esphome/nspanel_esphome_core.yaml
      - esphome/nspanel_esphome_standard.yaml
      - esphome/nspanel_esphome_addon_upload_tft.yaml  # Upload TFT add-on
```

## Configuration

The following keys are available to be used in your `substitutions`:

<!-- markdownlint-disable MD013 MD033 -->
Key|Required|Supported values|Default|Description
:-|:-:|:-:|:-:|:-
upload_tft_automatically|Optional|`true` or `false`|`true`|When enabled, the device will automatically upload the TFT file when a version mismatch is detected after boot. When disabled, you must manually press the "Update TFT display" button in Home Assistant.
upload_tft_baud_rate|Optional|Positive integer (bps)|`115200`|Baud rate used for the serial transfer to the Nextion display. Lower values are more reliable on noisy setups; higher values are faster. Common values: `9600`, `115200`, `921600`.
upload_tft_wait_ms_after_boot|Optional|Positive integer (milliseconds)|`300000` (5 min)|Time to wait after the first NTP time sync before starting an automatic TFT upload. This delay allows the system to stabilize after boot. Reduce for faster updates; increase if you experience boot instability.
nextion_update_url|Optional|Valid HTTP/HTTPS URL|GitHub raw URL for EU TFT|The URL from which the TFT file is downloaded. This is used as a fallback when "Use nextion\_update\_url" is selected in the model selector. For most users, the model selector in Home Assistant is the preferred way to choose the TFT source.
<!-- markdownlint-enable MD013 MD033 -->

### Example: Automatic updates with a shorter wait time

```yaml
substitutions:
  device_name: "YOUR_NSPANEL_NAME"
  friendly_name: "Your panel's friendly name"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
  ota_password: ""  # Optional: set OTA password, or use ${wifi_password} for backward compatibility (see migration guide)

  # Upload TFT configuration
  upload_tft_automatically: true
  upload_tft_wait_ms_after_boot: 120000  # Wait 2 minutes after boot instead of 5

packages:
  remote_package:
    url: https://github.com/edwardtfn/NSPanel-Easy
    ref: main
    refresh: 300s
    files:
      - nspanel_esphome.yaml
```

### Example: Manual updates only

```yaml
substitutions:
  device_name: "YOUR_NSPANEL_NAME"
  friendly_name: "Your panel's friendly name"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
  ota_password: ""  # Optional: set OTA password, or use ${wifi_password} for backward compatibility (see migration guide)

  # Disable automatic TFT upload
  upload_tft_automatically: false

packages:
  remote_package:
    url: https://github.com/edwardtfn/NSPanel-Easy
    ref: main
    refresh: 300s
    files:
      - nspanel_esphome.yaml
```

### Example: Using a local TFT file

```yaml
substitutions:
  device_name: "YOUR_NSPANEL_NAME"
  friendly_name: "Your panel's friendly name"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
  ota_password: ""  # Optional: set OTA password, or use ${wifi_password} for backward compatibility (see migration guide)

  # Use a locally hosted TFT file
  nextion_update_url: "http://homeassistant.local:8123/local/nspanel_eu.tft"

packages:
  remote_package:
    url: https://github.com/edwardtfn/NSPanel-Easy
    ref: main
    refresh: 300s
    files:
      - nspanel_esphome.yaml
```

> [!NOTE]
> When using `nextion_update_url`, make sure to select "Use nextion\_update\_url"
> in the "Update TFT display - Model" selector on the device's page in Home Assistant.

## Updating the TFT

### Automatic updates

When `upload_tft_automatically` is set to `true` (the default), the device will:

1. Boot and connect to Home Assistant.
2. Receive the current TFT version from the display.
3. Compare it with the expected version from the firmware.
4. If a mismatch is detected and a standard model is selected, wait for the configured delay
   (`upload_tft_wait_ms_after_boot`) after the first NTP time sync.
5. Automatically start the TFT upload.

> [!NOTE]
> The wait period exists to prevent the TFT upload from starting before the boot process
> has fully completed, as early uploads have been reported to cause issues on some devices.
> During this time the panel may not behave as expected, since the display is running
> an incompatible TFT version.
>
> If you prefer not to wait, you can press the "Update TFT display" button in Home Assistant
> at any time to trigger an immediate upload, bypassing the remaining wait.

### Manual updates

To manually trigger a TFT upload:

1. Go to **Settings** > **Devices & services** > **ESPHome** and select your panel.
2. Under **Configuration**, select the appropriate model in "Update TFT display - Model".
3. Press the "Update TFT display" button.
4. The display will start the update process and the device will restart when complete.

## Memory considerations

The TFT upload process requires a significant amount of free memory on the ESP32.
If you are running memory-intensive components such as Bluetooth Proxy alongside this add-on,
the TFT upload may fail due to insufficient memory.

In such cases, you can temporarily remove memory-heavy components, perform the TFT update,
and then re-add them. See the [Memory Optimization](install.md#memory-optimization-strategies)
section in the installation guide for detailed instructions.

## Troubleshooting

For common TFT transfer issues and solutions, see the
[TFT Transfer Troubleshooting Guide](tft_upload.md).
