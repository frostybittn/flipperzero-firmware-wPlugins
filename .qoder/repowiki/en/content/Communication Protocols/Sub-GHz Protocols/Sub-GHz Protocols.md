# Sub-GHz Protocols

<cite>
**Referenced Files in This Document**   
- [cc1101.c](file://lib/drivers/cc1101.c)
- [cc1101.h](file://lib/drivers/cc1101.h)
- [types.h](file://lib/subghz/types.h)
- [environment.h](file://lib/subghz/environment.h)
- [receiver.h](file://lib/subghz/receiver.h)
- [transmitter.h](file://lib/subghz/transmitter.h)
- [base.h](file://lib/subghz/protocols/base.h)
- [somfy_keytis.c](file://lib/subghz/protocols/somfy_keytis.c)
- [keeloq.c](file://lib/subghz/protocols/keeloq.c)
- [nice_flo.c](file://lib/subghz/protocols/nice_flo.c)
- [cc1101_configs.h](file://lib/subghz/devices/cc1101_configs.h)
- [preset.h](file://lib/subghz/devices/preset.h)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [CC1101 Radio Driver Implementation](#cc1101-radio-driver-implementation)
3. [Frequency Bands and Modulation Schemes](#frequency-bands-and-modulation-schemes)
4. [Protocol Architecture](#protocol-architecture)
5. [Specific Protocol Implementations](#specific-protocol-implementations)
6. [Transmission and Reception Workflows](#transmission-and-reception-workflows)
7. [Configuration Options](#configuration-options)
8. [Advanced Features](#advanced-features)
9. [Common Issues and Solutions](#common-issues-and-solutions)
10. [Performance Considerations](#performance-considerations)

## Introduction
The Sub-GHz wireless protocols implementation in the Flipper Zero firmware provides comprehensive support for various wireless communication standards operating in the sub-gigahertz frequency range. This documentation details the architecture, implementation, and usage of the Sub-GHz subsystem, focusing on the CC1101 radio driver, supported frequency bands, modulation schemes, and specific protocol implementations such as Somfy, KeeLoq, and Nice Flo. The system enables users to analyze, capture, and transmit wireless signals for various applications including garage door openers, security systems, and remote controls.

## CC1101 Radio Driver Implementation

The CC1101 radio driver implementation provides a robust interface for controlling the Texas Instruments CC1101 transceiver chip. The driver is implemented in C and provides both low-level and high-level APIs for interacting with the radio hardware.

The driver communicates with the CC1101 chip via SPI (Serial Peripheral Interface), handling all necessary timing and synchronization requirements. Key functions include:

- **SPI Transaction Handling**: The `cc1101_spi_trx` function manages SPI transactions with timeout protection, ensuring reliable communication with the radio chip.
- **Strobe Commands**: The `cc1101_strobe` function sends command strobes to control the radio's state (e.g., switching between idle, receive, and transmit modes).
- **Register Access**: Functions like `cc1101_write_reg` and `cc1101_read_reg` provide access to the CC1101's internal registers for configuration and status monitoring.
- **Frequency Control**: The `cc1101_set_frequency` function calculates and sets the desired operating frequency using the chip's frequency synthesizer.

The driver also implements essential radio operations such as:
- Resetting the device (`cc1101_reset`)
- Calibrating the oscillator (`cc1101_calibrate`)
- Switching between operational states (`cc1101_switch_to_idle`, `cc1101_switch_to_rx`, `cc1101_switch_to_tx`)
- Managing transmit and receive FIFO buffers (`cc1101_write_fifo`, `cc1101_read_fifo`)

**Section sources**
- [cc1101.c](file://lib/drivers/cc1101.c#L1-L189)
- [cc1101.h](file://lib/drivers/cc1101.h#L1-L196)

## Frequency Bands and Modulation Schemes

The Sub-GHz system supports multiple frequency bands and modulation schemes to accommodate various wireless protocols.

### Supported Frequency Bands
The system operates on the following frequency bands:
- **315MHz**: Commonly used in North America for automotive and security applications
- **433MHz**: Widely used globally for various wireless devices
- **868MHz**: Used in Europe for industrial, scientific, and medical applications
- **915MHz**: Used in North America for industrial, scientific, and medical applications

These frequencies are supported through the CC1101's programmable frequency synthesizer, which can be configured to operate at precise frequencies within these bands.

### Modulation Schemes
The system implements two primary modulation schemes:

#### ASK/OOK (Amplitude Shift Keying/On-Off Keying)
ASK/OOK modulation varies the amplitude of the carrier wave to represent data. In OOK, the presence of a carrier wave represents a binary 1, while its absence represents a binary 0. The system supports ASK/OOK modulation through predefined radio presets:
- `FuriHalSubGhzPresetOok270Async`: OOK with 270kHz bandwidth
- `FuriHalSubGhzPresetOok650Async`: OOK with 650kHz bandwidth

#### FSK (Frequency Shift Keying)
FSK modulation varies the frequency of the carrier wave to represent data. The system supports FSK modulation with different frequency deviations:
- `FuriHalSubGhzPreset2FSKDev238Async`: FSK with 2.38kHz deviation
- `FuriHalSubGhzPreset2FSKDev476Async`: FSK with 47.6kHz deviation

Additional modulation schemes supported include:
- MSK (Minimum Shift Keying) at 99.97Kb/s
- GFSK (Gaussian Frequency Shift Keying) at 9.99Kb/s

**Section sources**
- [cc1101.c](file://lib/drivers/cc1101.c#L123-L147)
- [cc1101_configs.h](file://lib/subghz/devices/cc1101_configs.h#L8-L14)
- [preset.h](file://lib/subghz/devices/preset.h#L4-L13)

## Protocol Architecture

The Sub-GHz protocol architecture is designed as a modular system that supports various wireless protocols through a common framework. The architecture consists of several key components that work together to handle signal encoding, decoding, and processing.

### Signal Encoding and Decoding
The system implements a flexible encoding and decoding framework that can handle various modulation schemes and data formats. The core components include:

- **Manchester Encoding/Decoding**: Used by protocols like Somfy to ensure clock synchronization and prevent DC bias in the transmitted signal.
- **Pulse Width Analysis**: The system analyzes the duration of signal levels to decode data, with configurable timing tolerances to accommodate variations in transmission.
- **Rolling Code Handling**: For security systems, the system implements rolling code mechanisms that increment a counter with each transmission to prevent replay attacks.

### Data Structures
The protocol architecture is built around several key data structures:

- **SubGhzProtocol**: Defines a protocol with its name, type, flags, and pointers to encoder and decoder implementations.
- **SubGhzProtocolDecoder** and **SubGhzProtocolEncoder**: Function pointer structures that define the interface for protocol-specific decoding and encoding operations.
- **SubGhzBlockGeneric**: A generic data structure that stores protocol-specific information such as serial numbers, button codes, and rolling counters.

### Protocol Registry
The system uses a protocol registry to manage available protocols. Each protocol is registered with:
- A unique name
- Type (static, dynamic, or RAW)
- Flags indicating supported frequency bands, modulation schemes, and capabilities
- Filter categories for organizing protocols

**Section sources**
- [types.h](file://lib/subghz/types.h#L34-L158)
- [base.h](file://lib/subghz/protocols/base.h#L9-L95)
- [environment.h](file://lib/subghz/environment.h#L12-L124)

## Specific Protocol Implementations

The firmware includes implementations for several specific wireless protocols, each with its own characteristics and timing parameters.

### Somfy Protocol
The Somfy protocol implementation supports the Somfy Keytis and Telis systems used in motorized window coverings and garage door openers.

Key characteristics:
- **Modulation**: OOK with Manchester encoding
- **Frequency**: Primarily 433.42MHz or 868.3MHz
- **Timing Parameters**:
  - Short pulse: 640μs
  - Long pulse: 1,280μs
  - Timing tolerance: ±250μs
- **Synchronization Pattern**: 
  - Wake-up pulse: 9,415μs high, 89,565μs low
  - Hardware sync: 12 repetitions of 2,560μs high/low pairs
  - Software sync: 4,550μs high, 640μs low
- **Data Structure**: 80-bit frame with button code, rolling counter, and device serial number
- **Security**: Rolling code with counter incrementation and CRC validation

The implementation handles the complex packet structure, including the wake-up sequence, synchronization patterns, and Manchester-encoded data payload.

### KeeLoq Protocol
The KeeLoq protocol is a proprietary encryption algorithm used in various security systems and garage door openers.

Key characteristics:
- **Modulation**: OOK
- **Frequency**: 315MHz, 433MHz, or 868MHz depending on region
- **Timing Parameters**:
  - Short pulse: 400μs
  - Long pulse: 800μs
  - Timing tolerance: ±140μs
- **Synchronization Pattern**: 11 repetitions of 400μs high/low pairs followed by a 400μs high and 4,000μs low gap
- **Data Structure**: 64-bit encrypted payload with button code, rolling counter, and device serial number
- **Security**: Rolling code with manufacturer-specific encryption algorithms

The implementation supports various KeeLoq variants and learning modes, including simple learning, normal learning, secure learning, and manufacturer-specific variants like BFT, Aprimatic, and Dea_Mio.

### Nice Flo Protocol
The Nice Flo protocol is used in Nice automation systems for gates and garage doors.

Key characteristics:
- **Modulation**: OOK
- **Frequency**: 315MHz or 433MHz
- **Timing Parameters**:
  - Short pulse: 700μs
  - Long pulse: 1,400μs
  - Timing tolerance: ±200μs
- **Synchronization Pattern**: 25,200μs low period followed by a 700μs high start bit
- **Data Structure**: Variable length (12-24 bits) with no rolling code (static code)
- **Security**: None (static code system)

The implementation handles the simple pulse-position modulation scheme used by this protocol.

**Section sources**
- [somfy_keytis.c](file://lib/subghz/protocols/somfy_keytis.c#L1-L814)
- [keeloq.c](file://lib/subghz/protocols/keeloq.c#L1-L1482)
- [nice_flo.c](file://lib/subghz/protocols/nice_flo.c#L1-L338)

## Transmission and Reception Workflows

The Sub-GHz system implements comprehensive workflows for both signal transmission and reception, providing a complete solution for wireless communication.

### Reception Workflow
The reception workflow processes incoming radio signals and decodes them into meaningful data:

1. **Signal Capture**: The CC1101 radio captures RF signals and converts them to digital pulses
2. **Level and Duration Extraction**: The system measures the duration of each high and low signal level
3. **Protocol Detection**: The receiver analyzes the timing patterns to identify potential protocols
4. **Decoding**: The appropriate protocol decoder processes the signal data
5. **Validation**: The decoded data is validated (e.g., CRC check, rolling code verification)
6. **Callback Execution**: If decoding is successful, a callback function is executed to handle the received data

The receiver system supports filtering to focus on specific protocol categories (e.g., cars, alarms, sensors) and can ignore certain protocols to improve performance.

### Transmission Workflow
The transmission workflow generates and sends wireless signals:

1. **Protocol Selection**: The user selects the desired protocol and configures its parameters
2. **Data Preparation**: The system prepares the data payload based on the protocol requirements
3. **Encoding**: The data is encoded according to the protocol's specifications (e.g., Manchester encoding for Somfy)
4. **Upload Generation**: The encoded data is converted to a sequence of level and duration pairs
5. **Transmission**: The CC1101 is configured for transmission mode, and the signal is sent
6. **Repetition**: The transmission is repeated according to the configured repeat count

The transmission system handles all necessary radio configuration, including frequency setting, modulation scheme selection, and power output control.

**Section sources**
- [receiver.h](file://lib/subghz/receiver.h#L1-L83)
- [transmitter.h](file://lib/subghz/transmitter.h#L1-L57)
- [somfy_keytis.c](file://lib/subghz/protocols/somfy_keytis.c#L215-L399)
- [keeloq.c](file://lib/subghz/protocols/keeloq.c#L477-L576)

## Configuration Options

The Sub-GHz system provides various configuration options for optimizing performance and compatibility.

### Power Output and Sensitivity
The system allows configuration of transmission power and receiver sensitivity:

- **Transmission Power**: Configured through the PA (Power Amplifier) table, which defines the output power levels for the CC1101
- **Receiver Sensitivity**: Adjusted through the radio preset configuration, which sets the bandwidth and filtering parameters

These settings can be optimized based on the specific use case, balancing range, battery life, and interference resistance.

### Radio Presets
The system supports multiple radio presets that configure the CC1101 for different modulation schemes and bandwidths:

- **OOK Presets**: Optimized for amplitude modulation with different bandwidths (270kHz and 650kHz)
- **FSK Presets**: Optimized for frequency modulation with different deviation values
- **MSK/GFSK Presets**: Optimized for more complex modulation schemes with specific data rates

These presets are defined in the `cc1101_configs.h` file and can be selected based on the target protocol's requirements.

**Section sources**
- [cc1101.c](file://lib/drivers/cc1101.c#L149-L160)
- [cc1101_configs.h](file://lib/subghz/devices/cc1101_configs.h#L8-L14)
- [preset.h](file://lib/subghz/devices/preset.h#L4-L13)

## Advanced Features

The Sub-GHz system implements several advanced features to enhance functionality and security.

### Frequency Hopping
While not explicitly implemented in the provided code, the CC1101's frequency agility could be leveraged to implement frequency hopping techniques. This would involve:
- Rapidly switching between multiple frequencies during transmission
- Using a predetermined sequence known to both transmitter and receiver
- Improving resistance to interference and jamming

### Signal Replay
The system supports signal replay functionality, allowing captured signals to be retransmitted. This is implemented through:
- Storing received signal data in memory
- Configuring the transmitter with the stored parameters
- Repeating the transmission as needed

This feature is particularly useful for testing and debugging wireless systems.

### Adaptive Frequency Selection
While not directly implemented, the system's architecture supports adaptive frequency selection through:
- Scanning for clear channels before transmission
- Selecting frequencies with minimal interference
- Dynamically adjusting to changing RF environments

**Section sources**
- [cc1101.c](file://lib/drivers/cc1101.c#L123-L147)
- [transmitter.h](file://lib/subghz/transmitter.h#L1-L57)

## Common Issues and Solutions

The Sub-GHz system addresses several common issues encountered in wireless communication.

### Signal Interference
Signal interference from other devices can disrupt communication. The system addresses this through:

- **Frequency Selection**: Allowing operation on multiple frequency bands to avoid crowded channels
- **Filtering**: Implementing protocol-specific filters to ignore irrelevant signals
- **Error Detection**: Using CRC and other validation methods to detect corrupted transmissions

### Regulatory Compliance
The system must comply with regional regulations for RF emissions. This is addressed through:

- **Power Limitation**: Configuring transmission power to comply with legal limits
- **Duty Cycle Management**: Limiting transmission duration to comply with regulations
- **Frequency Band Selection**: Operating only on approved frequency bands for each region

Solutions like adaptive frequency selection could further improve compliance by automatically avoiding frequencies with high interference or regulatory restrictions.

**Section sources**
- [cc1101.c](file://lib/drivers/cc1101.c#L123-L147)
- [preset.h](file://lib/subghz/devices/preset.h#L4-L13)

## Performance Considerations

The Sub-GHz system includes several performance considerations, particularly regarding battery life during signal analysis.

### Battery Life Optimization
When performing long-duration signal analysis, battery life can be significantly impacted. The system addresses this through:

- **Efficient Signal Processing**: Optimized algorithms minimize CPU usage during signal reception
- **Power Management**: The CC1101 can be put into low-power modes when not actively transmitting or receiving
- **Selective Monitoring**: Protocol filtering reduces unnecessary processing of irrelevant signals

### Signal Analysis Efficiency
The system is designed to efficiently process signals through:

- **Hardware Acceleration**: Leveraging the CC1101's built-in features for signal detection
- **Optimized Decoding Algorithms**: Efficient implementations of protocol decoders
- **Memory Management**: Careful allocation and deallocation of resources to minimize overhead

These considerations ensure that the system can perform extended signal analysis tasks while maintaining acceptable battery life.

**Section sources**
- [cc1101.c](file://lib/drivers/cc1101.c#L95-L97)
- [receiver.h](file://lib/subghz/receiver.h#L1-L83)
- [transmitter.h](file://lib/subghz/transmitter.h#L1-L57)