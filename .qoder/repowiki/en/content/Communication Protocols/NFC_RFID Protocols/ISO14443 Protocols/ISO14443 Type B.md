# ISO14443 Type B

<cite>
**Referenced Files in This Document**   
- [iso14443_3b_poller.c](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_poller.c)
- [iso14443_3b_poller_i.c](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_poller_i.c)
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h)
- [iso14443_4b_poller_i.c](file://lib/nfc/protocols/iso14443_4b/iso14443_4b_poller_i.c)
- [nfc_poller.c](file://lib/nfc/nfc_poller.c)
- [nfc_listener.c](file://lib/nfc/nfc_listener.c)
- [nfc.h](file://lib/nfc/nfc.h)
- [iso14443_4_layer.c](file://lib/nfc/helpers/iso14443_4_layer.c)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Modulation and Encoding](#modulation-and-encoding)
3. [Initialization Sequence](#initialization-sequence)
4. [Anti-Collision Procedures](#anti-collision-procedures)
5. [Protocol Activation](#protocol-activation)
6. [ISO/IEC 14443-4B Implementation](#isoiec-14443-4b-implementation)
7. [Poller Implementation](#poller-implementation)
8. [Listener Implementation](#listener-implementation)
9. [Signal Interference and Solutions](#signal-interference-and-solutions)
10. [Configuration Options](#configuration-options)
11. [Performance Considerations](#performance-considerations)
12. [Conclusion](#conclusion)

## Introduction
The ISO14443 Type B protocol implementation in the Flipper Zero firmware provides comprehensive support for contactless smart card communication following the ISO/IEC 14443-3B and ISO/IEC 14443-4B standards. This documentation details the technical implementation of the protocol stack, focusing on the physical layer characteristics, initialization procedures, anti-collision mechanisms, and transport protocol implementation. The Flipper Zero's NFC subsystem is designed to operate as both a poller (reader) and listener (tag emulator) for Type B communications, enabling a wide range of applications from card cloning to security research.

**Section sources**
- [iso14443_3b_poller.c](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_poller.c#L1-L121)
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h#L1-L38)

## Modulation and Encoding
The ISO14443 Type B protocol employs 10% ASK (Amplitude Shift Keying) modulation for data transmission from the reader to the card (downlink) and Manchester encoding for data representation. In the 10% ASK modulation scheme, the carrier signal is reduced by 90% (not completely turned off) during the transmission of a logic '0', while it remains at full amplitude for a logic '1'. This shallow modulation depth presents challenges for signal detection but allows for continuous power transfer to the passive card.

Manchester encoding is used to ensure clock recovery and DC balance in the transmitted data. In this encoding scheme, each bit period is divided into two halves, with a transition occurring in the middle of each bit period. A logic '0' is represented by a high-to-low transition, while a logic '1' is represented by a low-to-high transition. This encoding guarantees at least one transition per bit period, facilitating clock synchronization between the communicating devices.

The implementation in the Flipper Zero firmware handles these physical layer characteristics through the ST25R3916 NFC controller, which manages the modulation and demodulation processes. The firmware configures the appropriate modulation index and decoding parameters to ensure reliable communication with Type B cards.

**Section sources**
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h#L8-L10)
- [nfc.h](file://lib/nfc/nfc.h#L151-L185)

## Initialization Sequence
The initialization sequence for ISO14443 Type B communication begins with the REQB (Request Type B) command sent by the poller to detect available cards in the field. The REQB command consists of a frame with the command byte 0x05, followed by two additional bytes: 0x00 (Application Family Identifier, AFI) and 0x08 (number of slots, PARAM). This command initiates the inventory process and requests that compliant cards respond.

Upon receiving the REQB command, compliant cards respond with an ATQB (Answer to Request Type B) frame containing their identification and capability information. The ATQB response includes the card's 4-byte PUPI (Pseudo-Unique PICC Identifier), 4-byte application data, and protocol information field. The protocol information field contains details about the card's communication capabilities, including supported bit rates and frame sizes.

Following successful detection, the poller may send a WUPB (Wake-Up Type B) command, which has the same structure as REQB but is used to re-activate cards that have been placed in hibernation state. This allows for efficient power management in scenarios where multiple cards are present but only specific ones need to be activated.

The implementation in the Flipper Zero firmware handles this initialization sequence through the `iso14443_3b_poller_activate` function, which manages the transmission of REQB/WUPB commands and processing of ATQB responses.

**Section sources**
- [iso14443_3b_poller_i.c](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_poller_i.c#L75-L105)
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h#L5-L6)

## Anti-Collision Procedures
The anti-collision procedure in ISO14443 Type B uses a slot-based protocol to resolve conflicts when multiple cards are present in the reader's field. The process begins with the poller sending a REQB command with a specified number of slots (PARAM field). Each compliant card randomly selects a slot number and responds only if its selected slot matches the current slot being polled.

The implementation in the Flipper Zero firmware supports this anti-collision mechanism through the state machine in the `Iso14443_3bPoller` structure. When multiple cards are detected, the poller iterates through the available slots, allowing each card to respond in its assigned time slot. The PUPI (Pseudo-Unique PICC Identifier) serves as a temporary identifier during the anti-collision process, allowing the poller to distinguish between different cards.

In cases where collisions occur (multiple cards respond in the same slot), the poller can adjust the number of slots or use additional selection procedures to resolve the conflict. The firmware implementation includes error handling for collision scenarios, with appropriate timeout and retry mechanisms to ensure reliable card detection.

The anti-collision procedure is integrated with the higher-level protocol activation, ensuring that once a card is successfully selected, the communication can proceed to the protocol activation phase without requiring a complete restart of the initialization process.

**Section sources**
- [iso14443_3b_poller_i.c](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_poller_i.c#L75-L105)
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h#L34-L35)

## Protocol Activation
Protocol activation in ISO14443 Type B occurs after successful card detection and anti-collision resolution. The process begins with the poller sending an ATTRIB command to the selected card, which includes the card's PUPI, communication parameters, and a Card Identifier (CID). The ATTRIB command enables parameter negotiation between the poller and the card, establishing the communication settings for the session.

The Flipper Zero firmware implements protocol activation at multiple speeds, starting with the default 106 kbps and supporting higher speeds up to 848 kbps. The speed selection is negotiated through the bit rate capability field in the protocol information, which indicates the supported communication speeds for both directions (PCD to PICC and PICC to PCD).

The activation process includes setting appropriate timing parameters, such as the Frame Delay Time (FDT) and Guard Time, to ensure reliable communication. The firmware configures these parameters based on the card's capabilities and the selected communication speed. The guard time of 5000 microseconds (ISO14443_3B_GUARD_TIME_US) is used to separate consecutive frames and prevent interference.

Once the ATTRIB command is successfully processed by the card (indicated by a matching CID in the response), the protocol is considered activated, and higher-layer communication can begin. The firmware maintains the activated state in the `Iso14443_3bPollerState` enumeration, transitioning from ActivationInProgress to Activated state upon successful completion.

**Section sources**
- [iso14443_3b_poller_i.c](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_poller_i.c#L120-L153)
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h#L12-L19)

## ISO/IEC 14443-4B Implementation
The ISO/IEC 14443-4B implementation in the Flipper Zero firmware provides the transport protocol layer for Type B communications, enabling the exchange of APDUs (Application Protocol Data Units) between the reader and card. This layer is built on top of the ISO14443-3B physical and data link layers, adding protocol control and error handling capabilities.

The implementation includes support for the ATTRIB command and parameter negotiation, allowing the poller and card to agree on communication parameters such as frame size, bit rate, and protocol options. The firmware handles the encoding and decoding of protocol data units, including the addition of Protocol Control Bytes (PCB) to manage the communication flow.

APDU exchange mechanisms are implemented through the `iso14443_4_layer` module, which manages the segmentation and reassembly of APDUs that exceed the maximum frame size. The implementation supports both command and response APDUs, with appropriate error handling for protocol violations and communication errors.

The transport protocol also includes support for the WTX (Waiting Time Extension) mechanism, allowing either party to request additional time to process a command or prepare a response. This is particularly important for operations that require significant processing time on the card side, such as cryptographic operations.

**Section sources**
- [iso14443_4b_poller_i.c](file://lib/nfc/protocols/iso14443_4b/iso14443_4b_poller_i.c#L26-L47)
- [iso14443_4_layer.c](file://lib/nfc/helpers/iso14443_4_layer.c#L170-L220)

## Poller Implementation
The nfc_poller implementation for Type B communications in the Flipper Zero firmware follows a modular architecture with a clear separation between the generic poller interface and the protocol-specific implementation. The `nfc_poller_iso14443_3b` structure defines the interface for the ISO14443-3B poller, including function pointers for allocation, deallocation, callback setting, execution, detection, and data retrieval.

The poller state machine manages the various phases of communication, including Idle, ColResInProgress, ColResFailed, ActivationInProgress, ActivationFailed, and Activated states. This state machine ensures proper sequencing of operations and appropriate error handling. The implementation uses callback functions to notify the higher-level application of events such as protocol detection, data reception, and errors.

Key functions in the poller implementation include `iso14443_3b_poller_activate` for handling the initialization sequence, `iso14443_3b_poller_send_frame` for transmitting data frames, and `iso14443_3b_poller_halt` for terminating communication. These functions handle the low-level details of frame construction, CRC calculation, and timing management, providing a clean interface for higher-level applications.

The poller also integrates with the generic NFC infrastructure through the `nfc_poller_start` and `nfc_poller_stop` functions, which manage the lifecycle of the poller instance and coordinate with the underlying NFC hardware.

**Section sources**
- [iso14443_3b_poller.c](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_poller.c#L114-L121)
- [nfc_poller.c](file://lib/nfc/nfc_poller.c#L77-L286)

## Listener Implementation
The listener implementation in the Flipper Zero firmware enables the device to emulate a Type B card, responding to reader commands as if it were a physical smart card. The listener architecture follows a similar modular design to the poller, with the `nfc_listener_iso14443_3b` structure defining the interface for the ISO14443-3B listener.

The listener state machine handles the various phases of card emulation, including initialization, activation by the reader, and data exchange. When the reader sends a REQB command, the listener responds with a pre-configured ATQB frame containing the emulated card's PUPI, application data, and protocol information. The implementation supports configurable parameters, allowing users to customize the emulated card's characteristics.

The listener captures frames with strict adherence to the guard time requirements specified in the ISO14443-3B standard. The firmware ensures that responses are transmitted within the required timing constraints, using hardware timers to maintain precise timing. The `nfc_set_guard_time_us` function configures the guard time parameter, which is critical for maintaining compatibility with reader devices.

The listener implementation also handles the ATTRIB command from the reader, responding with the appropriate CID and establishing the communication parameters for the session. Once activated, the listener can exchange APDUs with the reader, emulating the behavior of a real Type B card.

**Section sources**
- [nfc_listener.c](file://lib/nfc/nfc_listener.c#L75-L145)
- [nfc.h](file://lib/nfc/nfc.h#L193-L194)

## Signal Interference and Solutions
Signal interference in ISO14443 Type B communications can occur due to the lower modulation index (10% ASK), which makes the signal more susceptible to noise and interference from other RF sources. The shallow modulation depth reduces the signal-to-noise ratio, potentially leading to decoding errors and communication failures.

The Flipper Zero firmware addresses these challenges through several mechanisms. Adaptive gain control is implemented in the ST25R3916 NFC controller, which automatically adjusts the receiver gain based on the signal strength. This helps maintain optimal sensitivity across different operating conditions and distances.

The firmware also implements robust error detection and correction mechanisms, including CRC verification for all received frames. When a CRC error is detected, the implementation can request retransmission or initiate appropriate recovery procedures. The timeout mechanisms in the poller and listener state machines help prevent indefinite waiting for responses in noisy environments.

Additionally, the implementation includes configurable receive sensitivity settings, allowing users to optimize performance for specific use cases. In environments with high RF interference, reducing the receive sensitivity can help filter out noise, while in low-signal environments, increasing sensitivity can improve detection range.

**Section sources**
- [nfc.h](file://lib/nfc/nfc.h#L159-L167)
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h#L8-L9)

## Configuration Options
The Flipper Zero firmware provides several configuration options for receive sensitivity and timing parameters to optimize Type B communication performance. These options are exposed through the NFC API and can be adjusted based on the specific requirements of the application.

Receive sensitivity can be configured through the NFC hardware settings, allowing users to balance between detection range and noise immunity. The firmware provides default settings optimized for typical use cases, but advanced users can modify these parameters for specialized applications.

Timing parameters such as guard time, frame delay time, and frame wait time are configurable to ensure compatibility with different reader devices and card types. The `nfc_set_guard_time_us`, `nfc_set_fdt_poll_fc`, and `nfc_set_fdt_poll_poll_us` functions allow precise control over these timing parameters.

The implementation also supports configuration of communication parameters such as bit rate and frame size through the ATTRIB command during protocol activation. These parameters are negotiated between the poller and card, with the firmware providing sensible defaults while allowing customization when needed.

For battery-powered operation, the firmware includes power management features that can be configured to balance performance and battery life. These include options for reducing the polling frequency during prolonged scanning sessions and entering low-power modes when no cards are detected.

**Section sources**
- [nfc.h](file://lib/nfc/nfc.h#L183-L194)
- [iso14443_3b_i.h](file://lib/nfc/protocols/iso14443_3b/iso14443_3b_i.h#L8-L10)

## Performance Considerations
Performance considerations for ISO14443 Type B communications in the Flipper Zero firmware include battery usage during prolonged scanning sessions, processing overhead, and communication reliability. The implementation is designed to balance these factors to provide optimal performance across different use cases.

Battery usage is a critical consideration, as NFC operations can be power-intensive. The firmware implements several power-saving measures, including adaptive polling rates, automatic sleep modes when no cards are detected, and optimized hardware configurations. During prolonged scanning sessions, the device can dynamically adjust its power consumption based on the detection rate and user activity.

Processing overhead is minimized through efficient state machine design and hardware acceleration of critical operations such as CRC calculation and modulation/demodulation. The implementation uses interrupt-driven I/O to reduce CPU utilization during communication, allowing the processor to handle other tasks concurrently.

Communication reliability is ensured through robust error handling, timeout mechanisms, and retry strategies. The firmware includes diagnostic features that can help identify and troubleshoot communication issues, such as signal strength monitoring and error logging.

For applications requiring high throughput, the implementation supports higher communication speeds up to 848 kbps, significantly reducing transaction times compared to the default 106 kbps rate. The firmware automatically negotiates the highest supported speed during protocol activation to maximize performance.

**Section sources**
- [nfc_poller.c](file://lib/nfc/nfc_poller.c#L203-L210)
- [nfc_listener.c](file://lib/nfc/nfc_listener.c#L126-L130)

## Conclusion
The ISO14443 Type B implementation in the Flipper Zero firmware provides a comprehensive and robust solution for contactless smart card communication. The modular architecture, with clear separation between the generic NFC infrastructure and protocol-specific implementations, enables reliable operation as both a poller and listener. The implementation faithfully follows the ISO/IEC 14443-3B and ISO/IEC 14443-4B standards, supporting all required features including 10% ASK modulation, Manchester encoding, initialization sequences, anti-collision procedures, and transport protocol mechanisms.

The firmware's attention to timing parameters, error handling, and power management ensures reliable operation across a wide range of use cases and environmental conditions. Advanced features such as adaptive gain control and configurable receive sensitivity help overcome the challenges associated with the lower modulation index of Type B communications.

For developers and security researchers, the implementation provides a powerful platform for analyzing and interacting with Type B smart cards, with extensive configuration options and diagnostic capabilities. The open architecture and well-documented interfaces make it suitable for both practical applications and educational purposes in the field of contactless smart card technology.