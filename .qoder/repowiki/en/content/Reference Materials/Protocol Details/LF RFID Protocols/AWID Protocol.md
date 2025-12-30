# AWID Protocol

<cite>
**Referenced Files in This Document**   
- [protocol_awid.h](file://lib/lfrfid/protocols/protocol_awid.h)
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c)
- [lfrfid_protocols.h](file://lib/lfrfid/protocols/lfrfid_protocols.h)
- [lfrfid_worker.c](file://lib/lfrfid/lfrfid_worker.c)
- [lfrfid_worker_modes.c](file://lib/lfrfid/lfrfid_worker_modes.c)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Project Structure](#project-structure)
3. [Core Components](#core-components)
4. [Architecture Overview](#architecture-overview)
5. [Detailed Component Analysis](#detailed-component-analysis)
6. [Dependency Analysis](#dependency-analysis)
7. [Performance Considerations](#performance-considerations)
8. [Troubleshooting Guide](#troubleshooting-guide)
9. [Conclusion](#conclusion)

## Introduction
The AWID (American Wide Identification) protocol is a proprietary low-frequency RFID protocol used in access control systems. This document provides a comprehensive analysis of the AWID protocol implementation in the Flipper Zero firmware, covering its data structure, encoding scheme, and integration with the device's RFID subsystem. The implementation supports multiple AWID format types including 26-bit, 34-bit, 37-bit, and others, using FSK modulation for data transmission.

## Project Structure
The AWID protocol implementation is organized within the Flipper Zero firmware's low-frequency RFID (LF RFID) subsystem. The code is structured in a modular fashion with clear separation between protocol-specific logic and the general RFID worker framework.

```mermaid
graph TD
subgraph "LF RFID Protocols"
AWID[protocol_awid.c]
Protocols[lfrfid_protocols.h]
end
subgraph "LF RFID Worker"
Worker[lfrfid_worker.c]
WorkerModes[lfrfid_worker_modes.c]
end
AWID --> Protocols
AWID --> WorkerModes
Worker --> WorkerModes
Protocols --> WorkerModes
style AWID fill:#f9f,stroke:#333
style Protocols fill:#bbf,stroke:#333
style Worker fill:#f96,stroke:#333
style WorkerModes fill:#6f9,stroke:#333
```

**Diagram sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c)
- [lfrfid_protocols.h](file://lib/lfrfid/protocols/lfrfid_protocols.h)
- [lfrfid_worker.c](file://lib/lfrfid/lfrfid_worker.c)
- [lfrfid_worker_modes.c](file://lib/lfrfid/lfrfid_worker_modes.c)

**Section sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c)
- [lfrfid_protocols.h](file://lib/lfrfid/protocols/lfrfid_protocols.h)

## Core Components
The AWID protocol implementation consists of several key components that work together to handle reading, decoding, encoding, and emulation of AWID RFID tags. The core functionality is implemented in the `protocol_awid.c` file, which defines the protocol structure, decoder, and encoder logic.

The implementation uses FSK (Frequency Shift Keying) modulation with specific timing parameters for data transmission. The protocol supports multiple format types with different bit lengths, including 26-bit, 34-bit, 37-bit, and others. Each format type has specific data structure characteristics for facility code and card number representation.

**Section sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c)
- [protocol_awid.h](file://lib/lfrfid/protocols/protocol_awid.h)

## Architecture Overview
The AWID protocol is integrated into the Flipper Zero's LF RFID subsystem through a well-defined architecture that separates protocol-specific logic from the general RFID worker framework. The architecture follows a modular design pattern where each RFID protocol is implemented as a self-contained module that conforms to a common protocol interface.

```mermaid
classDiagram
class ProtocolBase {
+const char* name
+const char* manufacturer
+size_t data_size
+uint32_t features
+uint8_t validate_count
+ProtocolAlloc alloc()
+ProtocolFree free()
+ProtocolGetData get_data()
+ProtocolDecoder decoder
+ProtocolEncoder encoder
+ProtocolRenderData render_data()
+ProtocolWriteData write_data()
}
class ProtocolAwid {
+ProtocolAwidDecoder decoder
+ProtocolAwidEncoder encoder
+uint8_t encoded_data[13]
+uint8_t data[9]
}
class LFRFIDWorker {
+LFRFIDWorkerMode mode_index
+LFRFIDWorkerReadCallback read_cb
+LFRFIDWorkerWriteCallback write_cb
+void* cb_ctx
+ProtocolDict* protocols
+FuriThread* thread
}
class ProtocolDict {
+const ProtocolBase** protocols
+size_t protocol_count
}
ProtocolBase <|-- ProtocolAwid
LFRFIDWorker --> ProtocolDict : "uses"
ProtocolDict --> ProtocolAwid : "contains"
ProtocolAwid --> FSKDemod : "uses"
ProtocolAwid --> FSKOsc : "uses"
```

**Diagram sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c)
- [lfrfid_worker.c](file://lib/lfrfid/lfrfid_worker.c)
- [lfrfid_protocols.h](file://lib/lfrfid/protocols/lfrfid_protocols.h)

## Detailed Component Analysis

### AWID Protocol Implementation
The AWID protocol implementation provides comprehensive support for reading, decoding, and emulating AWID RFID tags. The implementation handles the proprietary encoding scheme used by AWID access control systems, including data structure, bit encoding, and transmission format.

#### Data Structure and Encoding
The AWID protocol uses a 96-bit encoded format with specific structure and error checking mechanisms. The data structure includes preamble, format length, facility code, card number, and parity bits. The implementation supports multiple format types with different bit lengths.

```mermaid
flowchart TD
Start([AWID Protocol Initialization]) --> Allocate["Allocate ProtocolAwid structure"]
Allocate --> Initialize["Initialize FSKDemod and FSKOsc"]
Initialize --> Return["Return protocol instance"]
subgraph "Decoding Process"
DecoderStart["Decoder Start: Reset encoded_data"] --> Feed["Decoder Feed: Process level/duration"]
Feed --> Demod["FSK Demodulate signal"]
Demod --> Push["Push bit to encoded_data"]
Push --> Validate["Validate can_be_decoded()"]
Validate --> |True| Decode["Decode data using protocol_awid_decode()"]
Validate --> |False| Continue["Continue processing"]
Decode --> Store["Store decoded data in protocol->data"]
Store --> Complete["Return true (decoding complete)"]
end
subgraph "Encoding Process"
EncoderStart["Encoder Start: Encode data to encoded_data"] --> Reset["Reset FSKOsc"]
Reset --> SetIndex["Set encoded_index to 0"]
SetIndex --> Yield["Encoder Yield: Get next level/duration"]
Yield --> GetBit["Get bit from encoded_data"]
GetBit --> Osc["FSKOsc generate signal"]
Osc --> Advance["Advance to next bit"]
Advance --> ReturnSignal["Return LevelDuration"]
end
Complete --> End([Function Complete])
ReturnSignal --> End
```

**Diagram sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c#L50-L266)

**Section sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c#L50-L266)

#### Protocol Features and Constants
The AWID protocol implementation defines several constants and features that characterize its behavior:

- **JITTER_TIME**: 20 - Tolerance for timing variations
- **MIN_TIME**: 64 - Minimum time unit for FSK demodulation
- **MAX_TIME**: 80 - Maximum time unit for FSK demodulation
- **AWID_DECODED_DATA_SIZE**: 9 bytes - Size of decoded data
- **AWID_ENCODED_BIT_SIZE**: 96 bits - Size of encoded data
- **AWID_ENCODED_DATA_SIZE**: 13 bytes - Size of encoded data buffer

The protocol uses FSK modulation with specific timing parameters to ensure reliable data transmission and reception. The implementation includes error checking through odd parity bits and preamble validation.

### AWID Format Types
The AWID protocol implementation supports multiple format types with different bit lengths:

- **26-bit format**: Standard format with 8-bit facility code and 16-bit card number
- **34-bit format**: Extended format with additional data bits
- **37-bit format**: Extended format with additional data bits
- **50-bit format**: Extended format with additional data bits
- **36-bit format**: Extended format with additional data bits

Each format type shares the same basic structure but differs in the number of data bits and their organization. The implementation validates the format length and processes the data accordingly.

```mermaid
flowchart LR
FormatCheck["Check format length"] --> |26-bit| Process26["Process 26-bit format"]
FormatCheck --> |34-bit| Process34["Process 34-bit format"]
FormatCheck --> |37-bit| Process37["Process 37-bit format"]
FormatCheck --> |50-bit| Process50["Process 50-bit format"]
FormatCheck --> |36-bit| Process36["Process 36-bit format"]
FormatCheck --> |Other| ProcessHex["Process as hex data"]
Process26 --> ExtractFC["Extract facility code (8 bits)"]
Process26 --> ExtractCard["Extract card number (16 bits)"]
Process26 --> Render26["Render FC and Card in human-readable format"]
Process34 --> RenderHex["Render as hex data"]
Process37 --> RenderHex
Process50 --> RenderHex
Process36 --> RenderHex
ProcessHex --> RenderHex
Render26 --> Output
RenderHex --> Output
subgraph "26-bit Format Structure"
Preamble["Preamble: 0x01"]
FormatLen["Format Length: 26"]
Parity1["Odd Parity"]
Facility["Facility Code: 8 bits"]
Parity2["Odd Parity"]
CardHigh["Card Number High: 8 bits"]
Parity3["Odd Parity"]
CardLow["Card Number Low: 8 bits"]
Parity4["Odd Parity"]
WiegandParity["Wiegand Parity"]
Padding["Padding: 0x01"]
Preamble --> FormatLen
FormatLen --> Parity1
Parity1 --> Facility
Facility --> Parity2
Parity2 --> CardHigh
CardHigh --> Parity3
Parity3 --> CardLow
CardLow --> Parity4
Parity4 --> WiegandParity
WiegandParity --> Padding
end
```

**Diagram sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c#L100-L150)

**Section sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c#L100-L150)

### Integration with LF RFID Worker
The AWID protocol is integrated into the Flipper Zero's LF RFID worker system, which manages the reading, writing, and emulation of RFID tags. The integration follows a plugin architecture where each protocol is registered with the worker system.

```mermaid
sequenceDiagram
participant Worker as "LFRFIDWorker"
participant Modes as "lfrfid_worker_modes"
participant Protocol as "ProtocolAwid"
participant Dict as "ProtocolDict"
Worker->>Modes : Start thread
activate Worker
activate Modes
loop Process Events
Modes->>Modes : Wait for event flags
alt Read Event
Modes->>Protocol : protocol_dict_decoders_start()
Modes->>Protocol : protocol_dict_decoders_feed_by_feature()
alt Decoding Success
Protocol->>Modes : Return PROTOCOL_AWID
Modes->>Worker : Call read_cb(LFRFIDWorkerReadDone)
end
end
alt Write Event
Modes->>Protocol : protocol_dict_get_write_data()
Modes->>Protocol : protocol_dict_get_data()
Protocol->>Modes : Return encoded data
Modes->>T5577 : t5577_write()
Modes->>Modes : Verify write success
Modes->>Worker : Call write_cb()
end
alt Emulate Event
Modes->>Protocol : protocol_dict_encoder_start()
loop Generate Signal
Protocol->>Modes : protocol_dict_encoder_yield()
Modes->>RF : furi_hal_rfid_tim_emulate_dma_start()
end
Modes->>Modes : Check for stop
end
end
deactivate Modes
deactivate Worker
```

**Diagram sources**
- [lfrfid_worker.c](file://lib/lfrfid/lfrfid_worker.c)
- [lfrfid_worker_modes.c](file://lib/lfrfid/lfrfid_worker_modes.c)
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c)

**Section sources**
- [lfrfid_worker.c](file://lib/lfrfid/lfrfid_worker.c)
- [lfrfid_worker_modes.c](file://lib/lfrfid/lfrfid_worker_modes.c)

## Dependency Analysis
The AWID protocol implementation has several dependencies on other components within the Flipper Zero firmware. These dependencies enable the protocol to function within the broader RFID subsystem.

```mermaid
graph TD
AWID[AWID Protocol] --> FSKDemod["FSKDemod (lfrfid/tools/fsk_demod.h)"]
AWID --> FSKOsc["FSKOsc (lfrfid/tools/fsk_osc.h)"]
AWID --> BitLib["BitLib (bit_lib/bit_lib.h)"]
AWID --> ProtocolBase["ProtocolBase (toolbox/protocols/protocol.h)"]
FSKDemod --> SignalProcessing["Signal Processing"]
FSKOsc --> SignalGeneration["Signal Generation"]
BitLib --> BitManipulation["Bit Manipulation Utilities"]
ProtocolBase --> ProtocolInterface["Common Protocol Interface"]
style AWID fill:#f9f,stroke:#333
style FSKDemod fill:#bbf,stroke:#333
style FSKOsc fill:#bbf,stroke:#333
style BitLib fill:#bbf,stroke:#333
style ProtocolBase fill:#bbf,stroke:#333
```

**Diagram sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c#L1-L10)
- [protocol_awid.h](file://lib/lfrfid/protocols/protocol_awid.h)

**Section sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c#L1-L10)

## Performance Considerations
The AWID protocol implementation is designed with performance in mind, particularly for real-time signal processing. The use of efficient bit manipulation functions and optimized FSK demodulation/oscillation algorithms ensures reliable operation within the constraints of the Flipper Zero's hardware.

Key performance considerations include:
- Efficient bit manipulation using the BitLib library
- Optimized FSK demodulation with jitter tolerance
- Minimal memory allocation during signal processing
- Efficient data encoding and decoding algorithms
- Proper timing for signal generation and reception

The implementation uses a streaming approach to process RFID signals, allowing it to handle continuous data without excessive memory usage. The use of DMA (Direct Memory Access) for signal generation further improves performance by offloading work from the CPU.

## Troubleshooting Guide
When working with the AWID protocol implementation, several common issues may arise. This section provides guidance on troubleshooting these issues.

**Section sources**
- [protocol_awid.c](file://lib/lfrfid/protocols/protocol_awid.c)
- [lfrfid_worker_modes.c](file://lib/lfrfid/lfrfid_worker_modes.c)

### Common Issues and Solutions
1. **Signal Not Detected**
   - Ensure proper coil alignment with the reader
   - Check that the signal strength is sufficient
   - Verify that the correct protocol type is selected
   - Confirm that the timing parameters are appropriate for the specific AWID format

2. **Decoding Failures**
   - Check for interference from other electronic devices
   - Verify that the preamble and spacing are correct
   - Ensure that the odd parity bits are properly validated
   - Confirm that the format length is supported

3. **Emulation Issues**
   - Verify that the FSK modulation parameters are correct
   - Check that the DMA buffer is properly configured
   - Ensure that the signal timing is within acceptable tolerances
   - Confirm that the encoded data matches the expected format

4. **Writing Failures**
   - Verify that the target tag (T5577) is present and functional
   - Check that the write mask and block configuration are correct
   - Ensure that the password protection is properly handled
   - Confirm that the verification read succeeds after writing

## Conclusion
The AWID protocol implementation in the Flipper Zero firmware provides comprehensive support for reading, decoding, and emulating AWID RFID tags. The implementation follows a modular architecture that integrates seamlessly with the device's LF RFID subsystem. By supporting multiple format types and providing robust error checking, the implementation enables reliable interaction with AWID-based access control systems. The use of efficient algorithms and proper hardware integration ensures optimal performance for both reading and emulation tasks.