# Reference Materials

<cite>
**Referenced Files in This Document**   
- [NfcFileFormats.md](file://documentation/file_formats/NfcFileFormats.md)
- [LfRfidFileFormat.md](file://documentation/file_formats/LfRfidFileFormat.md)
- [SubGhzFileFormats.md](file://documentation/file_formats/SubGhzFileFormats.md)
- [InfraredFileFormats.md](file://documentation/file_formats/InfraredFileFormats.md)
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)
- [public_api.h](file://lib/subghz/protocols/public_api.h)
</cite>

## Table of Contents
1. [API Documentation](#api-documentation)
2. [File Format Specifications](#file-format-specifications)
3. [Hardware Details](#hardware-details)

## API Documentation

The Sub-GHz protocol library provides a public API for generating encoded data for various wireless protocols. These functions abstract the complexity of protocol-specific encoding and allow for key generation from simple input parameters.

### Sub-GHz Protocol API

The public API is defined in `public_api.h` and provides functions for creating encoded data for various Sub-GHz protocols. All functions return a boolean value indicating success (true) or failure (false).

#### Function Signatures

**subghz_protocol_secplus_v2_create_data**
- **Purpose**: Generate data for SecPlus v2 protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderSecPlus_v2 instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (32-bit)
  - `btn`: Button number (8-bit)
  - `cnt`: Counter value (28-bit)
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_keeloq_create_data**
- **Purpose**: Generate data for Keeloq protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderKeeloq instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (28-bit)
  - `btn`: Button number (4-bit)
  - `cnt`: Counter value (16-bit)
  - `manufacture_name`: Name of manufacturer's key
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_keeloq_bft_create_data**
- **Purpose**: Generate data for Keeloq BFT protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderKeeloq instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (28-bit)
  - `btn`: Button number (4-bit)
  - `cnt`: Counter value (16-bit)
  - `seed`: Seed value (32-bit)
  - `manufacture_name`: Name of manufacturer's key
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_faac_slh_create_data**
- **Purpose**: Generate data for FAAC SLH protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderFaacSLH instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (28-bit)
  - `btn`: Button number (4-bit)
  - `cnt`: Counter value (16-bit)
  - `seed`: Seed value (32-bit)
  - `manufacture_name`: Name of manufacturer's key
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_alutech_at_4n_create_data**
- **Purpose**: Generate data for Alutech AT-4N protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderAlutech_at_4n instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (24-bit)
  - `btn`: Button number (8-bit)
  - `cnt`: Counter value (16-bit)
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_came_atomo_create_data**
- **Purpose**: Generate data for CAME Atomo protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderCameAtomo instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (24-bit)
  - `cnt`: Counter value (16-bit)
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_phoenix_v2_create_data**
- **Purpose**: Generate data for Phoenix V2 protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderPhoenix_V2 instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (32-bit)
  - `cnt`: Counter value (16-bit)
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_nice_flor_s_create_data**
- **Purpose**: Generate data for Nice Flor S protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderNiceFlorS instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (32-bit)
  - `btn`: Button number (4-bit)
  - `cnt`: Counter value (16-bit)
  - `preset`: Modulation settings (SubGhzRadioPreset)
  - `nice_one`: Boolean flag (true for Nice One, false for Nice Flor S)
- **Return**: true on success, false on failure

**subghz_protocol_star_line_create_data**
- **Purpose**: Generate data for Star Line protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderStarLine instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (24-bit)
  - `btn`: Button number (8-bit)
  - `cnt`: Counter value (16-bit)
  - `manufacture_name`: Name of manufacturer's key
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_somfy_telis_create_data**
- **Purpose**: Generate data for Somfy Telis protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderSomfyTelis instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (24-bit)
  - `btn`: Button number (8-bit)
  - `cnt`: Counter value (16-bit)
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

**subghz_protocol_somfy_keytis_create_data**
- **Purpose**: Generate data for Somfy Keytis protocol
- **Parameters**:
  - `context`: Pointer to a SubGhzProtocolEncoderSomfyKeytis instance
  - `flipper_format`: Pointer to a FlipperFormat instance for output
  - `serial`: Serial number (24-bit)
  - `btn`: Button number (8-bit)
  - `cnt`: Counter value (16-bit)
  - `preset`: Modulation settings (SubGhzRadioPreset)
- **Return**: true on success, false on failure

#### Utility Functions

**subghz_protocol_decoder_bin_raw_data_input_rssi**
- **Purpose**: Input RSSI data for BinRAW protocol decoding
- **Parameters**:
  - `instance`: Pointer to a SubGhzProtocolDecoderBinRAW instance
  - `rssi`: RSSI value as a float
- **Return**: void

**subghz_protocol_secplus_v1_check_fixed**
- **Purpose**: Validate fixed parts of SecPlus v1 protocol
- **Parameters**:
  - `fixed`: Fixed parts value (32-bit)
- **Return**: true if valid, false if invalid

**faac_slh_reset_prog_mode**
- **Purpose**: Reset programming mode variables for FAAC SLH protocol
- **Parameters**: None
- **Return**: void

**Section sources**
- [public_api.h](file://lib/subghz/protocols/public_api.h#L1-L238)

## File Format Specifications

This section documents the specifications for all file formats used by the system, including binary layouts and metadata fields for NFC, RFID, Sub-GHz, Infrared, iButton, and BadUSB formats.

### NFC File Formats

The NFC subsystem uses a standardized file format for storing various types of NFC device data. All NFC files use the `.nfc` extension and follow a common header structure.

#### Common Header Structure

All NFC file formats begin with a common header that includes metadata about the file type and version:

```
Filetype: Flipper NFC device
Version: 4
Device type: [protocol-specific type]
UID: [hex bytes]
```

- **Filetype**: Always "Flipper NFC device"
- **Version**: Current version is 4 (MSB ATQA format)
- **Device type**: Specifies the NFC protocol type
- **UID**: Unique Identifier (4 or 7 bytes)

#### Protocol-Specific Formats

**ISO14443-3A**
- **Device type**: ISO14443-3A
- **Fields**:
  - UID: 4 or 7 bytes
  - ATQA: 2 bytes
  - SAK: 1 byte

**ISO14443-3B**
- **Device type**: ISO14443-3B
- **Fields**:
  - UID: 4 bytes
  - Application data: 4 bytes
  - Protocol info: 3 bytes

**ISO14443-4A**
- **Device type**: ISO14443-4A
- **Fields**:
  - UID: 4 or 7 bytes
  - ATQA: 2 bytes
  - SAK: 1 byte
  - ATS: Answer to Select (minimum 5 bytes)

**NTAG/Ultralight**
- **Device type**: NTAG/Ultralight
- **Fields**:
  - UID: 4 or 7 bytes
  - ATQA: 2 bytes
  - SAK: 1 byte
  - Data format version: 2
  - NTAG/Ultralight type: Device model (e.g., NTAG216)
  - Signature: 32 bytes (READ_SIG command response)
  - Mifare version: 8 bytes (GET_VERSION command response)
  - Counter 0-2: 32-bit counter values
  - Tearing 0-2: 1 byte each
  - Pages total: Total number of pages
  - Pages read: Number of pages successfully read
  - Page 0 to Page N: Page data (4 bytes each)
  - Failed authentication attempts: Count of failed auth attempts

**Mifare Classic**
- **Device type**: Mifare Classic
- **Fields**:
  - UID: 4 bytes
  - ATQA: 2 bytes
  - SAK: 1 byte
  - Mifare Classic type: 1K or 4K
  - Data format version: 2
  - Block 0 to Block N: Block data (16 bytes each), with '??' for unknown data

**Mifare DESFire**
- **Device type**: Mifare DESFire
- **Fields**:
  - UID: 4 or 7 bytes
  - ATQA: 2 bytes
  - SAK: 1 byte
  - ATS: Answer to Select
  - PICC Version: 25 bytes
  - PICC Free Memory: Integer value
  - PICC Change Key ID: 1 byte
  - PICC Config Changeable: Boolean
  - PICC Free Create Delete: Boolean
  - PICC Free Directory List: Boolean
  - PICC Key Changeable: Boolean
  - PICC Max Keys: 1 byte
  - PICC Key 0 Version: 1 byte
  - Application Count: Integer
  - Application IDs: Hex bytes
  - Application [ID] fields: Various application-specific properties
  - Application [ID] File IDs: Hex bytes
  - Application [ID] File [N] fields: File-specific properties
  - Application [ID] File [N]: File data

**Mifare Classic Dictionary**
- **Extension**: `.dict`
- **Format**: Plain text with one key per line
- **Comments**: Lines starting with '#' are ignored
- **Keys**: Hexadecimal strings (12 characters for 6-byte keys)

**Mifare Ultralight C Dictionary**
- **Extension**: `.dict`
- **Format**: Plain text with one key per line
- **Comments**: Lines starting with '#' are ignored
- **Keys**: Hexadecimal strings (32 characters for 16-byte keys)

**EMV Resources**
- **Extension**: `.emv`
- **Format**: Key-value pairs separated by colon and space
- **Example**: `0997: USN` (currency code and name)

**Section sources**
- [NfcFileFormats.md](file://documentation/file_formats/NfcFileFormats.md#L1-L348)

### LF RFID File Format

The LF RFID subsystem uses a simple file format for storing low-frequency RFID key data.

#### File Structure

```
Filetype: Flipper RFID key
Version: 1
Key type: [protocol type]
Data: [hex bytes]
```

- **Filetype**: Always "Flipper RFID key"
- **Version**: Current version is 1
- **Key type**: Protocol type (e.g., EM4100, H10301)
- **Data**: Key data in hexadecimal format

#### Supported Key Types

| Type | Full Name |
|------|-----------|
| EM4100 | EM-Micro EM4100 |
| H10301 | HID H10301 |
| Idteck | IDTECK |
| Indala26 | Motorola Indala26 |
| IOProxXSF | Kantech IOProxXSF |
| AWID | AWID |
| FDX-A | FECAVA FDX-A |
| FDX-B | ISO FDX-B |
| HIDProx | Generic HIDProx |
| HIDExt | Generic HIDExt |
| Pyramid | Farpointe Pyramid |
| Viking | Viking |
| Jablotron | Jablotron |
| Paradox | Paradox |
| PAC/Stanley | PAC/Stanley |
| Keri | Keri |
| Gallagher | Gallagher |
| GProxII | Guardall GProx II |

**Section sources**
- [LfRfidFileFormat.md](file://documentation/file_formats/LfRfidFileFormat.md#L1-L51)

### Sub-GHz File Format

The Sub-GHz subsystem uses `.sub` files to store wireless signals, which can contain either encoded protocol data or raw signal timings.

#### .sub File Structure

A `.sub` file consists of three parts:
1. Header
2. Preset information
3. Protocol and data

#### Header Format

```
Filetype: Flipper SubGhz Key File
Version: 1
Frequency: [frequency in Hz]
```

- **Filetype**: Must be "Flipper SubGhz Key File" or "Flipper SubGhz RAW File"
- **Version**: Current version is 1
- **Frequency**: Operating frequency in Hertz

#### Preset Information

Preset information configures the radio transceiver modulation, bandwidth, and filters.

**Standard Preset:**
```
Preset: [preset name]
```

**Custom Preset:**
```
Preset: FuriHalSubGhzPresetCustom
Custom_preset_module: CC1101
Custom_preset_data: [hex bytes]
```

**Built-in Presets:**
- FuriHalSubGhzPresetOok270Async: OOK, 270kHz bandwidth, async
- FuriHalSubGhzPresetOok650Async: OOK, 650kHz bandwidth, async
- FuriHalSubGhzPreset2FSKDev238Async: 2FSK, 2kHz deviation, 270kHz bandwidth, async
- FuriHalSubGhzPreset2FSKDev476Async: 2FSK, 47kHz deviation, 270kHz bandwidth, async

**Transceiver Configuration Data:**
- Format: `XX YY XX YY .. 00 00 ZZ ZZ ZZ ZZ ZZ ZZ ZZ ZZ`
- XX: Register address
- YY: Register value
- 00 00: Register block end marker
- ZZ: 8-byte PA table (Power amplifier ramp table)

#### Key Files

Key files contain protocol-specific encoded data:

```
Protocol: [protocol name]
[protocol-specific fields]
```

**Example (Princeton):**
```
Protocol: Princeton
Bit: 24
Key: 00 00 00 00 00 95 D5 D4
TE: 400
```

**Common Fields:**
- Protocol: Protocol name
- Bit: Payload length in bits
- Key: Payload data
- TE: Quantization interval in microseconds

#### RAW Files

RAW files contain unprocessed signal timings:

```
Protocol: RAW
RAW_Data: [timing values]
```

- **RAW_Data**: Array of timings in microseconds
- Values must be non-zero, start with positive number, and alternate sign
- Up to 512 values per line
- Multiple RAW_Data lines can be used

#### BinRAW Files

BinRAW files contain encoded repeating sequences:

```
Protocol: BinRAW
Bit: [total payload length in bits]
TE: [quantization interval in us]
Bit_RAW: [block payload length in bits]
Data_RAW: [encoded sequence]
```

- **Bit**: Total payload length (max 4096 bits)
- **TE**: Quantization interval in microseconds
- **Bit_RAW**: Block payload length in bits
- **Data_RAW**: Encoded sequence where 1=high level, 0=low level

#### Configuration Files

**SubGhz Keystore File (`keeloq_mfcodes_user`):**
```
Filetype: Flipper SubGhz Keystore File
Version: 0
Encryption: 0
[manufacturer_key]:[encryption_method]:[name]
```

**SubGhz Setting File (`setting_user`):**
```
Filetype: Flipper SubGhz Setting File
Version: 1
Add_standard_frequencies: [true/false]
Default_frequency: [frequency in Hz]
Frequency: [additional frequency]
Hopper_frequency: [hopping frequency]
Custom_preset_name: [preset name]
Custom_preset_module: CC1101
Custom_preset_data: [hex bytes]
```

**Section sources**
- [SubGhzFileFormats.md](file://documentation/file_formats/SubGhzFileFormats.md#L1-L304)

### Infrared File Formats

The infrared subsystem uses `.ir` files to store remote control signals in either parsed or raw format.

#### Infrared Remote File Format

```
Filetype: IR signals file
Version: 1
#
name: [button name]
type: [parsed|raw]
[protocol-specific or raw fields]
#
```

- **Filetype**: Always "IR signals file"
- **Version**: Current version is 1
- **Buttons**: Separated by comment lines (#)

#### Parsed Signal Fields

For known protocols:
- **name**: Button name (printable ASCII)
- **type**: "parsed"
- **protocol**: Protocol name (e.g., NEC, RC5, SIRC)
- **address**: 4-byte payload address
- **command**: 4-byte payload command

**Supported Protocols:**
- NEC, NECext, NEC42, NEC42ext
- Samsung32
- RC6, RC5, RC5X
- SIRC, SIRC15, SIRC20
- Kaseikyo, RCA

#### Raw Signal Fields

For unknown signals:
- **name**: Button name (printable ASCII)
- **type**: "raw"
- **frequency**: Carrier frequency in Hz (typically 38000)
- **duty_cycle**: Carrier duty cycle (typically 0.33)
- **data**: Raw signal timings in microseconds (space-separated)

#### Infrared Library Format

Identical to the remote format but used for universal libraries with predefined button names.

#### Infrared Test Format (.irtest)

Used for unit testing with additional fields:
- **type**: "parsed_array" or "raw"
- **count**: Number of parsed signals in array
- **repeat**: Boolean indicating repeated button press
- **Signal names**: Follow pattern `<name><test_number>` where name is:
  - decoder_input: Raw signal for decoder input
  - decoder_expected: Parsed array for expected decoder output
  - encoder_decoder_input: Parsed array for encoder-decoder input/output

**Section sources**
- [InfraredFileFormats.md](file://documentation/file_formats/InfraredFileFormats.md#L1-L146)

### iButton File Format

The iButton subsystem uses `.ibtn` files to store iButton key data.

#### File Structure

```
Filetype: Flipper iButton key
Version: 2
Protocol: [protocol type]
[protocol-specific data fields]
```

- **Filetype**: Always "Flipper iButton key"
- **Version**: Current version is 2
- **Protocol**: iButton protocol type

#### Protocol-Specific Fields

**DS1990, DS1992, DS1996, DS1971, DS1420, DSGeneric:**
- **Rom Data**: Read-only memory data (Dallas protocols only)

**DS1992, DS1996:**
- **Sram Data**: Static RAM data

**DS1971:**
- **Eeprom Data**: EEPROM data

**Cyfral, Metakom:**
- **Data**: Key data

**Supported Protocols:**
- DS1990, DS1992, DS1996, DS1971, DS1420
- DSGeneric (catch-all for unknown 1-Wire devices)
- Cyfral, Metakom

**Note**: DSGeneric reads only ROM data without validation and can be used for keys with invalid family codes or checksums.

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md#L1-L54)

### BadUSB Script Format

The BadUSB subsystem uses `.txt` files to store USB HID attack scripts in an extended Duckyscript syntax.

#### Script Structure

- **Extension**: `.txt`
- **Line endings**: `\n` or `\r\n` supported
- **Comments**: Lines starting with REM are ignored
- **Indentation**: Spaces or tabs allowed

#### Command Set

**Comment:**
- REM [comment text]

**Delay:**
- DELAY [milliseconds]
- DEFAULT_DELAY [milliseconds] (applies before every command)
- DEFAULTDELAY [milliseconds] (same as DEFAULT_DELAY)

**Special Keys:**
- Arrow keys: UP, DOWN, LEFT, RIGHT (or UPARROW, etc.)
- ENTER, DELETE, BACKSPACE, END, HOME, ESCAPE (or ESC)
- INSERT, PAGEUP, PAGEDOWN
- CAPSLOCK, NUMLOCK, SCROLLLOCK
- PRINTSCREEN, BREAK (or PAUSE)
- SPACE, TAB
- MENU (or APP)
- F1-F12

**Modifier Keys:**
- CTRL (or CONTROL)
- SHIFT
- ALT
- GUI (or WINDOWS)
- Can be chained with hyphens or spaces (e.g., CTRL-ALT-DEL)

**Key Hold/Release:**
- HOLD [key]
- RELEASE [key]
- Up to 5 keys can be held simultaneously

**String Commands:**
- STRING [text]
- STRINGLN [text] (adds ENTER after text)
- STRING_FROM_FILE [file path]

**String Delay:**
- STRING_DELAY [milliseconds] (applies to next STRING)
- STRINGDELAY [milliseconds] (same as STRING_DELAY)
- DEFAULT_STRING_DELAY [milliseconds] (applies to all STRING commands)
- DEFAULTSTRINGDELAY [milliseconds] (same as DEFAULT_STRING_DELAY)

**Repeat:**
- REPEAT [number of additional repeats]

**ALT+Numpad Input:**
- ALTCHAR [character code]
- ALTSTRING [text] (uses ALT+Numpad method)
- ALTCODE [text] (same as ALTSTRING)

**SysRq:**
- SYSRQ [single character]

**Media Keys:**
- MEDIA [key name]
- Supported keys: POWER, REBOOT, SLEEP, LOGOFF, EXIT, HOME, BACK, FORWARD, REFRESH, SNAPSHOT, PLAY, PAUSE, PLAY_PAUSE, NEXT_TRACK, PREV_TRACK, STOP, EJECT, MUTE, VOLUME_UP, VOLUME_DOWN, FN, BRIGHT_UP, BRIGHT_DOWN

**Fn/Globe Key (Mac):**
- GLOBE [key]

**User Interaction:**
- WAIT_FOR_BUTTON_PRESS (waits indefinitely for button press)

**USB Device ID:**
- ID [VID:PID] [Manufacturer:Product]
- Must be on first line of script
- Example: `ID 1234:abcd Flipper Devices:Flipper Zero`

**Mouse Commands:**
- LEFTCLICK (or LEFT_CLICK)
- RIGHTCLICK (or RIGHT_CLICK)
- MOUSEMOVE [x] [y] (or MOUSE_MOVE)
- MOUSESCROLL [delta] (or MOUSE_SCROLL)

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md#L1-L194)

## Hardware Details

This section provides detailed hardware specifications, pinouts, and electrical characteristics for the Flipper Zero device.

### Hardware Specifications

The Flipper Zero is a multi-tool device with various wireless communication capabilities. The hardware specifications include:

- **Main Processor**: STM32WB55RGV6 (ARM Cortex-M4 + Cortex-M0+)
- **Display**: 1.54" 128x64 monochrome LCD
- **Connectivity**:
  - Sub-GHz radio (CC1101 transceiver)
  - NFC/RFID reader (ST25R3916)
  - Infrared transmitter
  - Bluetooth Low Energy
- **Power**: Rechargeable lithium-ion battery
- **Storage**: Internal flash + microSD card slot
- **Expansion**: GPIO header for external modules

### Pinouts

The expansion header provides access to various hardware interfaces:

- **GPIO Pins**: Multiple general-purpose input/output pins
- **Power Pins**: 3.3V, 5V, and GND
- **Communication Interfaces**:
  - UART (TX, RX)
  - I2C (SCL, SDA)
  - SPI (SCK, MOSI, MISO, CS)
- **Special Function Pins**:
  - Button inputs
  - LED control
  - External antenna connection

### Electrical Characteristics

- **Operating Voltage**: 3.3V
- **Current Consumption**:
  - Idle: ~10mA
  - Active (display + processor): ~30-50mA
  - Wireless transmission: ~80-100mA peak
- **Battery**: 450mAh lithium-ion
- **Charging**: Micro-USB, 5V input

### Wireless Protocol Details

**Sub-GHz Radio (CC1101):**
- Frequency range: 300-348 MHz, 378-486 MHz, 779-928 MHz
- Modulation: OOK, 2-FSK
- Data rate: Up to 500 kbps
- Output power: Up to +10 dBm

**NFC/RFID (ST25R3916):**
- Supports ISO14443 A/B, ISO15693
- Read range: Up to 5 cm
- Supports Mifare Classic, NTAG, DESFire protocols

**Infrared:**
- Carrier frequency: Typically 38 kHz
- Modulation: PWM
- Range: Up to 10 meters

### Version Compatibility

**API Compatibility:**
- Sub-GHz protocol API is stable across firmware versions
- NFC and RFID APIs may change with protocol additions
- Infrared protocol support expands with firmware updates

**File Format Compatibility:**
- NFC file format version 4 is current standard
- RFID file format version 1 is current standard
- Sub-GHz .sub files maintain backward compatibility
- Infrared .ir files are version 1
- iButton .ibtn files use version 2 format
- BadUSB scripts maintain Duckyscript compatibility

**Section sources**
- [NfcFileFormats.md](file://documentation/file_formats/NfcFileFormats.md#L1-L348)
- [LfRfidFileFormat.md](file://documentation/file_formats/LfRfidFileFormat.md#L1-L51)
- [SubGhzFileFormats.md](file://documentation/file_formats/SubGhzFileFormats.md#L1-L304)
- [InfraredFileFormats.md](file://documentation/file_formats/InfraredFileFormats.md#L1-L146)
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md#L1-L54)
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md#L1-L194)
- [public_api.h](file://lib/subghz/protocols/public_api.h#L1-L238)