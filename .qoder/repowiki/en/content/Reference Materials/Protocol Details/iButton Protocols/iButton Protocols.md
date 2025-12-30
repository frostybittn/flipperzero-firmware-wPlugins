# iButton Protocols

<cite>
**Referenced Files in This Document**   
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [ibutton_key.h](file://lib/ibutton/ibutton_key.h)
- [ibutton_key_i.h](file://lib/ibutton/ibutton_key_i.h)
- [protocol_common.h](file://lib/ibutton/protocols/protocol_common.h)
- [protocol_common_i.h](file://lib/ibutton/protocols/protocol_common_i.h)
- [protocol_group_defs.h](file://lib/ibutton/protocols/protocol_group_defs.h)
- [protocol_group_base.h](file://lib/ibutton/protocols/protocol_group_base.h)
- [dallas_common.h](file://lib/ibutton/protocols/dallas/dallas_common.h)
- [dallas_common.c](file://lib/ibutton/protocols/dallas/dallas_common.c)
- [one_wire_host.h](file://lib/one_wire/one_wire_host.h)
- [one_wire_host.c](file://lib/one_wire/one_wire_host.c)
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Project Structure](#project-structure)
3. [Core Components](#core-components)
4. [Architecture Overview](#architecture-overview)
5. [Detailed Component Analysis](#detailed-component-analysis)
6. [Dallas 1-Wire Protocol Implementation](#dallas-1-wire-protocol-implementation)
7. [Protocol Registry System](#protocol-registry-system)
8. [Data Storage and File Format](#data-storage-and-file-format)
9. [Reading, Writing, and Emulation](#reading-writing-and-emulation)
10. [One-Wire Bus Operations](#one-wire-bus-operations)
11. [Error Detection and Validation](#error-detection-and-validation)
12. [Conclusion](#conclusion)

## Introduction

The iButton protocol implementation in the Flipper Zero firmware provides a comprehensive system for interacting with various iButton devices, primarily based on the Dallas 1-Wire protocol and its variants. This documentation details the technical specifications, architecture, and implementation of the iButton protocols supported by the Flipper Zero, including protocol handling, data storage, bus communication, and device emulation capabilities. The system supports multiple iButton types including DS1990, DS1992, DS1996, DS1971, DS1420, as well as non-Dallas protocols like Cyfral and Metakom.

## Project Structure

The iButton functionality is organized within the firmware's library structure, with dedicated components for protocol handling, data management, and hardware interaction. The core implementation resides in the `lib/ibutton` directory, which contains protocol-specific code, key data structures, and worker components.

```mermaid
graph TD
subgraph "iButton Library"
IB["lib/ibutton"]
IBProtocols["ibutton_protocols.*"]
IBKey["ibutton_key.*"]
IBWorker["ibutton_worker.*"]
subgraph "Protocols"
ProtocolsDir["protocols/"]
subgraph "Dallas"
Dallas["dallas/"]
DallasCommon["dallas_common.*"]
DS1990["protocol_ds1990.*"]
DS1992["protocol_ds1992.*"]
DS1996["protocol_ds1996.*"]
DS1971["protocol_ds1971.*"]
end
subgraph "Miscellaneous"
Misc["misc/"]
Cyfral["protocol_cyfral.*"]
Metakom["protocol_metakom.*"]
end
ProtocolCommon["protocol_common.*"]
ProtocolGroupBase["protocol_group_base.*"]
ProtocolGroupDefs["protocol_group_defs.*"]
end
end
subgraph "Dependencies"
OneWire["lib/one_wire"]
FlipperFormat["lib/flipper_format"]
Storage["services/storage"]
end
IB --> IBProtocols
IB --> IBKey
IB --> IBWorker
IB --> ProtocolsDir
ProtocolsDir --> Dallas
ProtocolsDir --> Misc
ProtocolsDir --> ProtocolCommon
ProtocolsDir --> ProtocolGroupBase
ProtocolsDir --> ProtocolGroupDefs
Dallas --> DallasCommon
Dallas --> DS1990
Dallas --> DS1992
Dallas --> DS1996
Dallas --> DS1971
Misc --> Cyfral
Misc --> Metakom
IBProtocols --> OneWire
IBProtocols --> FlipperFormat
IBProtocols --> Storage
```

**Diagram sources**
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [ibutton_key.h](file://lib/ibutton/ibutton_key.h)
- [protocol_group_defs.h](file://lib/ibutton/protocols/protocol_group_defs.h)
- [dallas_common.h](file://lib/ibutton/protocols/dallas/dallas_common.h)

**Section sources**
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [ibutton_key.h](file://lib/ibutton/ibutton_key.h)

## Core Components

The iButton system consists of several core components that work together to provide protocol support, data management, and hardware interaction. The main components include the protocol registry, key data structure, and common functionality for Dallas 1-Wire protocols.

The `iButtonProtocols` structure serves as the central registry for all supported iButton protocols, providing a unified interface for protocol operations. The `iButtonKey` structure represents an iButton key with its protocol-specific data, while the Dallas common module provides shared functionality for all Dallas 1-Wire based protocols.

**Section sources**
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_key.h](file://lib/ibutton/ibutton_key.h)
- [dallas_common.h](file://lib/ibutton/protocols/dallas/dallas_common.h)

## Architecture Overview

The iButton protocol architecture follows a modular design with a clear separation of concerns. The system is organized around a protocol registry that manages multiple protocol groups, each containing specific protocol implementations. This design allows for easy extension with new protocols while maintaining a consistent interface.

```mermaid
graph TD
App["Application Layer"] --> Protocols["iButtonProtocols"]
Protocols --> Registry["Protocol Registry"]
Protocols --> Key["iButtonKey"]
Registry --> DallasGroup["Dallas Protocol Group"]
Registry --> MiscGroup["Misc Protocol Group"]
DallasGroup --> DS1990["DS1990 Protocol"]
DallasGroup --> DS1992["DS1992 Protocol"]
DallasGroup --> DS1996["DS1996 Protocol"]
DallasGroup --> DS1971["DS1971 Protocol"]
MiscGroup --> Cyfral["Cyfral Protocol"]
MiscGroup --> Metakom["Metakom Protocol"]
DallasGroup --> DallasCommon["Dallas Common Functions"]
MiscGroup --> ProtocolCommon["Protocol Common Functions"]
DallasCommon --> OneWireHost["OneWireHost"]
ProtocolCommon --> OneWireHost
OneWireHost --> GPIO["FuriHal GPIO"]
Key --> Storage["FlipperFormat Storage"]
style App fill:#f9f,stroke:#333
style Protocols fill:#bbf,stroke:#333
style Registry fill:#bbf,stroke:#333
style Key fill:#bbf,stroke:#333
style DallasGroup fill:#f96,stroke:#333
style MiscGroup fill:#f96,stroke:#333
style DS1990 fill:#9f9,stroke:#333
style DS1992 fill:#9f9,stroke:#333
style DS1996 fill:#9f9,stroke:#333
style DS1971 fill:#9f9,stroke:#333
style Cyfral fill:#9f9,stroke:#333
style Metakom fill:#9f9,stroke:#333
style DallasCommon fill:#6cf,stroke:#333
style ProtocolCommon fill:#6cf,stroke:#333
style OneWireHost fill:#6cf,stroke:#333
style GPIO fill:#cfc,stroke:#333
style Storage fill:#cfc,stroke:#333
```

**Diagram sources**
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [protocol_group_defs.h](file://lib/ibutton/protocols/protocol_group_defs.h)
- [dallas_common.h](file://lib/ibutton/protocols/dallas/dallas_common.h)
- [one_wire_host.h](file://lib/one_wire/one_wire_host.h)

## Detailed Component Analysis

### iButton Protocols Registry

The iButton protocols registry provides a centralized interface for managing all supported iButton protocols. It implements a factory pattern with protocol groups, allowing for dynamic protocol discovery and operation dispatching.

```mermaid
classDiagram
class iButtonProtocols {
+group_datas : iButtonProtocolGroupData**
+ibutton_protocols_alloc()
+ibutton_protocols_free()
+ibutton_protocols_get_protocol_count()
+ibutton_protocols_get_max_data_size()
+ibutton_protocols_get_id_by_name()
+ibutton_protocols_get_manufacturer()
+ibutton_protocols_get_name()
+ibutton_protocols_get_features()
+ibutton_protocols_read()
+ibutton_protocols_write_id()
+ibutton_protocols_write_copy()
+ibutton_protocols_emulate_start()
+ibutton_protocols_emulate_stop()
+ibutton_protocols_save()
+ibutton_protocols_load()
+ibutton_protocols_render_uid()
+ibutton_protocols_render_data()
+ibutton_protocols_render_brief_data()
+ibutton_protocols_render_error()
+ibutton_protocols_is_valid()
+ibutton_protocols_get_editable_data()
+ibutton_protocols_apply_edits()
}
class iButtonProtocolGroupBase {
+protocol_count : uint32_t
+alloc : iButtonProtocolGroupAllocFunc
+free : iButtonProtocolGroupFreeFunc
+get_max_data_size : iButtonGropuGetSizeFunc
+get_id_by_name : iButtonProtocolGroupGetIdFunc
+get_features : iButtonProtocolGroupGetFeaturesFunc
+get_manufacturer : iButtonProtocolGroupGetStringFunc
+get_name : iButtonProtocolGroupGetStringFunc
+read : iButtonProtocolGroupReadFunc
+write_id : iButtonProtocolGroupWriteFunc
+write_copy : iButtonProtocolGroupWriteFunc
+emulate_start : iButtonProtocolGroupApplyFunc
+emulate_stop : iButtonProtocolGroupApplyFunc
+save : iButtonProtocolGroupSaveFunc
+load : iButtonProtocolGroupLoadFunc
+render_uid : iButtonProtocolGroupRenderFunc
+render_data : iButtonProtocolGroupRenderFunc
+render_brief_data : iButtonProtocolGroupRenderFunc
+render_error : iButtonProtocolGroupRenderFunc
+is_valid : iButtonProtocolGroupIsValidFunc
+get_editable_data : iButtonProtocolGroupGetDataFunc
+apply_edits : iButtonProtocolGroupApplyFunc
}
class iButtonProtocolGroupInfo {
+base : iButtonProtocolGroupBase*
+group : iButtonProtocolGroupData*
+id : iButtonProtocolLocalId
}
iButtonProtocols --> iButtonProtocolGroupBase : "uses"
iButtonProtocols --> iButtonProtocolGroupInfo : "uses"
```

**Diagram sources**
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [protocol_group_base.h](file://lib/ibutton/protocols/protocol_group_base.h)

**Section sources**
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)

### iButton Key Data Structure

The iButton key data structure provides a flexible container for storing iButton key information, supporting multiple protocol types with varying data requirements.

```mermaid
classDiagram
class iButtonKey {
+protocol_id : iButtonProtocolId
+protocol_data : iButtonProtocolData*
+ibutton_key_alloc()
+ibutton_key_free()
+ibutton_key_get_protocol_id()
+ibutton_key_set_protocol_id()
+ibutton_key_reset()
}
class iButtonProtocolData {
<<abstract>>
}
class iButtonEditableData {
+ptr : uint8_t*
+size : size_t
}
class iButtonProtocolId {
<<enumeration>>
iButtonProtocolIdInvalid = -1
}
iButtonKey --> iButtonProtocolData : "contains"
iButtonKey --> iButtonProtocolId : "references"
iButtonKey --> iButtonEditableData : "provides access to"
```

**Diagram sources**
- [ibutton_key.h](file://lib/ibutton/ibutton_key.h)
- [ibutton_key_i.h](file://lib/ibutton/ibutton_key_i.h)
- [protocol_common.h](file://lib/ibutton/protocols/protocol_common.h)

**Section sources**
- [ibutton_key.h](file://lib/ibutton/ibutton_key.h)
- [ibutton_key_i.h](file://lib/ibutton/ibutton_key_i.h)

## Dallas 1-Wire Protocol Implementation

The Dallas 1-Wire protocol implementation provides shared functionality for all Dallas-based iButton devices. This common module handles the fundamental operations required by the 1-Wire protocol, including bus commands, data transfer, and error detection.

### Dallas 1-Wire Command Structure

The Dallas 1-Wire protocol uses a standardized command structure for device communication. The common implementation provides functions for all standard commands used across Dallas iButton devices.

```mermaid
classDiagram
class DallasCommonRomData {
+family_code : uint8_t
+serial_number[6] : uint8_t
+checksum : uint8_t
+bytes[8] : uint8_t
}
class DallasCommonAddressRegs {
+address_lo : uint8_t
+address_hi : uint8_t
+status : uint8_t
+bytes[3] : uint8_t
}
class DallasCommonCommandState {
<<enumeration>>
DallasCommonCommandStateIdle
DallasCommonCommandStateRomCmd
DallasCommonCommandStateMemCmd
}
class DallasCommonFunctions {
+dallas_common_skip_rom()
+dallas_common_read_rom()
+dallas_common_write_scratchpad()
+dallas_common_read_scratchpad()
+dallas_common_copy_scratchpad()
+dallas_common_read_mem()
+dallas_common_write_mem()
+dallas_common_emulate_search_rom()
+dallas_common_emulate_read_rom()
+dallas_common_emulate_read_mem()
+dallas_common_save_rom_data()
+dallas_common_load_rom_data()
+dallas_common_is_valid_crc()
+dallas_common_render_uid()
+dallas_common_render_brief_data()
+dallas_common_render_crc_error()
+dallas_common_apply_edits()
}
DallasCommonFunctions --> DallasCommonRomData
DallasCommonFunctions --> DallasCommonAddressRegs
DallasCommonFunctions --> DallasCommonCommandState
```

**Diagram sources**
- [dallas_common.h](file://lib/ibutton/protocols/dallas/dallas_common.h)
- [dallas_common.c](file://lib/ibutton/protocols/dallas/dallas_common.c)

**Section sources**
- [dallas_common.h](file://lib/ibutton/protocols/dallas/dallas_common.h)
- [dallas_common.c](file://lib/ibutton/protocols/dallas/dallas_common.c)

### Dallas 1-Wire Command Flow

The sequence of operations for reading from and writing to a Dallas iButton device follows a specific protocol flow that ensures reliable communication.

```mermaid
sequenceDiagram
participant App as "Application"
participant Protocols as "iButtonProtocols"
participant DallasCommon as "DallasCommon"
participant OneWireHost as "OneWireHost"
participant Device as "iButton Device"
App->>Protocols : ibutton_protocols_read()
Protocols->>DallasCommon : dallas_common_skip_rom()
DallasCommon->>OneWireHost : onewire_host_write(SKIP_ROM)
OneWireHost->>Device : Send 0xCC command
Device-->>OneWireHost : Acknowledge
OneWireHost-->>DallasCommon : Success
DallasCommon->>DallasCommon : dallas_common_read_rom()
DallasCommon->>OneWireHost : onewire_host_write(READ_ROM)
OneWireHost->>Device : Send 0x33 command
Device-->>OneWireHost : Send ROM data (8 bytes)
OneWireHost-->>DallasCommon : Return ROM data
DallasCommon->>DallasCommon : dallas_common_is_valid_crc()
alt CRC Valid
DallasCommon-->>Protocols : Return success
Protocols-->>App : Return success
else CRC Invalid
DallasCommon-->>Protocols : Return failure
Protocols-->>App : Return failure
end
```

**Diagram sources**
- [dallas_common.c](file://lib/ibutton/protocols/dallas/dallas_common.c)
- [one_wire_host.c](file://lib/one_wire/one_wire_host.c)

## Protocol Registry System

The protocol registry system in the Flipper Zero iButton implementation uses a hierarchical structure with protocol groups to organize and manage different iButton protocols. This design allows for extensibility and efficient protocol lookup.

### Protocol Group Architecture

The protocol registry is organized into groups, with each group containing related protocols. The current implementation includes Dallas and miscellaneous protocol groups.

```mermaid
classDiagram
class iButtonProtocolGroup {
<<enumeration>>
iButtonProtocolGroupDallas
iButtonProtocolGroupMisc
iButtonProtocolGroupMax
}
class iButtonProtocolGroupBase {
+protocol_count : uint32_t
+alloc : iButtonProtocolGroupAllocFunc
+free : iButtonProtocolGroupFreeFunc
+get_max_data_size : iButtonGropuGetSizeFunc
+get_id_by_name : iButtonProtocolGroupGetIdFunc
+get_features : iButtonProtocolGroupGetFeaturesFunc
+get_manufacturer : iButtonProtocolGroupGetStringFunc
+get_name : iButtonProtocolGroupGetStringFunc
+read : iButtonProtocolGroupReadFunc
+write_id : iButtonProtocolGroupWriteFunc
+write_copy : iButtonProtocolGroupWriteFunc
+emulate_start : iButtonProtocolGroupApplyFunc
+emulate_stop : iButtonProtocolGroupApplyFunc
+save : iButtonProtocolGroupSaveFunc
+load : iButtonProtocolGroupLoadFunc
+render_uid : iButtonProtocolGroupRenderFunc
+render_data : iButtonProtocolGroupRenderFunc
+render_brief_data : iButtonProtocolGroupRenderFunc
+render_error : iButtonProtocolGroupRenderFunc
+is_valid : iButtonProtocolGroupIsValidFunc
+get_editable_data : iButtonProtocolGroupGetDataFunc
+apply_edits : iButtonProtocolGroupApplyFunc
}
class iButtonProtocolGroupData {
<<abstract>>
}
class iButtonProtocolLocalId {
<<type>>
int32_t
}
class iButtonProtocolGroupId {
<<type>>
int32_t
}
iButtonProtocolGroupBase --> iButtonProtocolGroupData : "manages"
iButtonProtocolGroupBase --> iButtonProtocolLocalId : "uses"
iButtonProtocolGroupBase --> iButtonProtocolGroupId : "uses"
```

**Diagram sources**
- [protocol_group_defs.h](file://lib/ibutton/protocols/protocol_group_defs.h)
- [protocol_group_base.h](file://lib/ibutton/protocols/protocol_group_base.h)

**Section sources**
- [protocol_group_defs.h](file://lib/ibutton/protocols/protocol_group_defs.h)
- [protocol_group_base.h](file://lib/ibutton/protocols/protocol_group_base.h)

### Protocol Registration and Lookup

The protocol registry system uses a global array of protocol group pointers to manage protocol registration and lookup. This allows for efficient protocol discovery and operation dispatching.

```mermaid
flowchart TD
Start([Initialize System]) --> Alloc["iButtonProtocols* ibutton_protocols_alloc()"]
Alloc --> Loop["For each protocol group"]
Loop --> Check{"Group < Max?"}
Check --> |Yes| Create["protocols->group_datas[i] = ibutton_protocol_groups[i]->alloc()"]
Create --> Increment["i++"]
Increment --> Loop
Check --> |No| Return["Return protocols"]
Return --> End([Ready for use])
subgraph "Protocol Lookup"
LookupStart([Get Protocol by Name]) --> Search["iButtonProtocols* protocols, const char* name"]
Search --> GroupLoop["For each protocol group"]
GroupLoop --> HasProtocol{"Group has protocol?"}
HasProtocol --> |Yes| ReturnId["Return local_id + offset"]
HasProtocol --> |No| NextGroup["offset += protocol_count"]
NextGroup --> GroupLoop
ReturnId --> LookupEnd([Return protocol ID])
end
style Start fill:#f9f,stroke:#333
style End fill:#f9f,stroke:#333
style LookupStart fill:#f9f,stroke:#333
style LookupEnd fill:#f9f,stroke:#333
```

**Diagram sources**
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)

## Data Storage and File Format

The iButton system uses a standardized file format for storing iButton key data, allowing for persistence and sharing of key information.

### iButton File Format Structure

The iButton file format is a text-based format that stores key information in a human-readable form while maintaining compatibility with the Flipper Zero's file system.

```mermaid
erDiagram
IBTN_FILE {
string Filetype PK
uint32 Version
string Protocol
hex RomData
hex SramData
hex EepromData
hex Data
}
IBTN_FILE ||--o{ PROTOCOL : "references"
class PROTOCOL {
DS1990
DS1992
DS1996
DS1971
DS1420
DSGeneric
Cyfral
Metakom
}
note right of IBTN_FILE.Filetype
"Flipper iButton key"
end note
note right of IBTN_FILE.Version
1 or 2
Version 1 is deprecated
end note
note right of IBTN_FILE.Protocol
Determines which data fields are present
end note
```

**Diagram sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)

### File Save and Load Sequence

The process of saving and loading iButton key data involves serialization and deserialization through the FlipperFormat system.

```mermaid
sequenceDiagram
participant App as "Application"
participant Protocols as "iButtonProtocols"
participant FlipperFormat as "FlipperFormat"
participant Storage as "Storage"
App->>Protocols : ibutton_protocols_save(key, file_name)
Protocols->>Storage : furi_record_open(RECORD_STORAGE)
Protocols->>FlipperFormat : flipper_format_buffered_file_alloc()
Protocols->>FlipperFormat : flipper_format_buffered_file_open_always()
Protocols->>FlipperFormat : flipper_format_write_header_cstr()
Protocols->>FlipperFormat : flipper_format_write_string_cstr()
Protocols->>Protocol : GROUP_BASE->save()
Protocol->>FlipperFormat : Write protocol-specific data
FlipperFormat-->>Protocol : Success
Protocol-->>Protocols : Success
Protocols->>FlipperFormat : flipper_format_free()
Protocols->>Storage : furi_record_close()
Protocols-->>App : Return success
App->>Protocols : ibutton_protocols_load(key, file_name)
Protocols->>Storage : furi_record_open(RECORD_STORAGE)
Protocols->>FlipperFormat : flipper_format_buffered_file_alloc()
Protocols->>FlipperFormat : flipper_format_buffered_file_open_existing()
Protocols->>FlipperFormat : flipper_format_read_header()
alt Version 1
Protocols->>FlipperFormat : flipper_format_read_string(IBUTTON_PROTOCOL_KEY_V1)
else Version 2
Protocols->>FlipperFormat : flipper_format_read_string(IBUTTON_PROTOCOL_KEY_V2)
end
Protocols->>Protocols : ibutton_protocols_get_id_by_name()
Protocols->>Protocol : GROUP_BASE->load()
Protocol->>FlipperFormat : Read protocol-specific data
FlipperFormat-->>Protocol : Success
Protocol-->>Protocols : Success
Protocols->>FlipperFormat : flipper_format_free()
Protocols->>Storage : furi_record_close()
Protocols-->>App : Return success
```

**Diagram sources**
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)

**Section sources**
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)

## Reading, Writing, and Emulation

The iButton system supports three primary operations: reading from physical devices, writing to blank or existing devices, and emulation of iButton devices.

### Operation Flowchart

The decision process for iButton operations depends on the protocol features and device state.

```mermaid
flowchart TD
Start([Operation Start]) --> CheckProtocol["Check Protocol Features"]
subgraph "Read Operation"
CheckProtocol --> CanRead{"Supports Read?"}
CanRead --> |Yes| ExecuteRead["ibutton_protocols_read()"]
ExecuteRead --> ProcessData["Process Key Data"]
ProcessData --> End
end
subgraph "Write Operation"
CheckProtocol --> CanWrite{"Supports Write?"}
CanWrite --> |Yes| SelectWriteType["Choose Write Type"]
SelectWriteType --> WriteId{"Write to Blank?"}
WriteId --> |Yes| ExecuteWriteId["ibutton_protocols_write_id()"]
WriteId --> |No| ExecuteWriteCopy["ibutton_protocols_write_copy()"]
ExecuteWriteId --> VerifyWrite["Verify Write Success"]
ExecuteWriteCopy --> VerifyWrite
VerifyWrite --> End
end
subgraph "Emulation Operation"
CheckProtocol --> CanEmulate{"Supports Emulate?"}
CanEmulate --> |Yes| StartEmulate["ibutton_protocols_emulate_start()"]
StartEmulate --> Wait["Wait for Interaction"]
Wait --> StopEmulate["ibutton_protocols_emulate_stop()"]
StopEmulate --> End
end
CanRead --> |No| Error["Operation Not Supported"]
CanWrite --> |No| Error
CanEmulate --> |No| Error
Error --> End([Operation Complete])
style Start fill:#f9f,stroke:#333
style End fill:#f9f,stroke:#333
```

**Section sources**
- [ibutton_protocols.h](file://lib/ibutton/ibutton_protocols.h)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)

## One-Wire Bus Operations

The One-Wire bus operations are implemented at the hardware level using bit-banging techniques to control the GPIO pin according to the 1-Wire protocol specifications.

### One-Wire Timing Parameters

The One-Wire protocol has strict timing requirements for reliable communication. The implementation supports standard and overdrive modes with different timing parameters.

```mermaid
classDiagram
class OneWireHostTimings {
+a : uint16_t "Write 1 low time"
+b : uint16_t "Write 1 high time"
+c : uint16_t "Write 0 low time"
+d : uint16_t "Write 0 high time"
+e : uint16_t "Read low time"
+f : uint16_t "Read high time"
+g : uint16_t "Reset pre-delay"
+h : uint16_t "Reset pulse"
+i : uint16_t "Presence detect"
+j : uint16_t "Reset post-delay"
}
class OneWireHost {
+gpio_pin : GpioPin*
+timings : OneWireHostTimings*
+saved_rom[8] : unsigned char
+last_discrepancy : uint8_t
+last_family_discrepancy : uint8_t
+last_device_flag : bool
+onewire_host_alloc()
+onewire_host_free()
+onewire_host_reset()
+onewire_host_read_bit()
+onewire_host_read()
+onewire_host_read_bytes()
+onewire_host_write_bit()
+onewire_host_write()
+onewire_host_write_bytes()
+onewire_host_start()
+onewire_host_stop()
+onewire_host_reset_search()
+onewire_host_target_search()
+onewire_host_search()
+onewire_host_set_overdrive()
+onewire_host_set_timings_default()
+onewire_host_set_timings_tm01x()
}
OneWireHost --> OneWireHostTimings
```

**Diagram sources**
- [one_wire_host.h](file://lib/one_wire/one_wire_host.h)
- [one_wire_host.c](file://lib/one_wire/one_wire_host.c)

### One-Wire Reset and Presence Detection

The reset and presence detection sequence is the first step in any 1-Wire communication, ensuring the bus is in a known state.

```mermaid
sequenceDiagram
participant Host as "OneWireHost"
participant Bus as "1-Wire Bus"
participant Device as "iButton Device"
Host->>Bus : furi_hal_gpio_write(true)
loop Wait for bus high
Host->>Bus : Check furi_hal_gpio_read()
alt Bus is low
Host->>Host : retries--
Host->>Host : furi_delay_us(2)
end
end
Host->>Bus : furi_delay_us(timings->g)
Host->>Bus : furi_hal_gpio_write(false)
Host->>Bus : furi_delay_us(timings->h)
Host->>Bus : furi_hal_gpio_write(true)
Host->>Bus : furi_delay_us(timings->i)
Host->>Bus : r = !furi_hal_gpio_read()
Host->>Bus : furi_delay_us(timings->j)
alt r == true
Host-->>Host : Return true (presence detected)
else
Host-->>Host : Return false (no presence)
end
```

**Diagram sources**
- [one_wire_host.c](file://lib/one_wire/one_wire_host.c)

**Section sources**
- [one_wire_host.h](file://lib/one_wire/one_wire_host.h)
- [one_wire_host.c](file://lib/one_wire/one_wire_host.c)

## Error Detection and Validation

The iButton system implements multiple layers of error detection and validation to ensure data integrity and proper device operation.

### CRC Validation Process

The Dallas 1-Wire protocol uses CRC-8 for error detection in the ROM data. The implementation includes functions to validate and generate CRC values.

```mermaid
flowchart TD
Start([Start CRC Validation]) --> Extract["Extract first 7 bytes of ROM data"]
Extract --> Calculate["Calculate CRC-8 using Maxim algorithm"]
Calculate --> Compare["Compare with 8th byte (checksum)"]
Compare --> Valid{"CRC Matches?"}
Valid --> |Yes| Success["Return true"]
Valid --> |No| Failure["Return false"]
Success --> End([Validation Complete])
Failure --> End
style Start fill:#f9f,stroke:#333
style End fill:#f9f,stroke:#333
```

**Section sources**
- [dallas_common.c](file://lib/ibutton/protocols/dallas/dallas_common.c)
- [dallas_common.h](file://lib/ibutton/protocols/dallas/dallas_common.h)

### Error Handling in Protocol Operations

The system implements comprehensive error handling throughout the protocol operations to ensure robust operation.

```mermaid
flowchart TD
Start([Protocol Operation]) --> CheckInput["Validate Input Parameters"]
CheckInput --> Valid{"Input Valid?"}
Valid --> |No| ReturnError["Return false"]
Valid --> |Yes| ExecuteOperation["Execute Protocol Operation"]
ExecuteOperation --> Success{"Operation Successful?"}
Success --> |Yes| ReturnSuccess["Return true"]
Success --> |No| Cleanup["Free allocated resources"]
Cleanup --> ReturnError
ReturnSuccess --> End([Operation Complete])
ReturnError --> End
style Start fill:#f9f,stroke:#333
style End fill:#f9f,stroke:#333
```

**Section sources**
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [dallas_common.c](file://lib/ibutton/protocols/dallas/dallas_common.c)

## Conclusion

The iButton protocol implementation in the Flipper Zero firmware provides a comprehensive and extensible system for working with various iButton devices. The architecture is designed with modularity in mind, separating concerns between protocol management, data storage, and hardware interaction.

Key features of the implementation include:
- A flexible protocol registry system that supports multiple protocol groups
- Standardized file format for persistent storage of iButton key data
- Comprehensive support for Dallas 1-Wire protocol operations
- Robust error detection and validation mechanisms
- Support for reading, writing, and emulation of iButton devices

The system leverages the One-Wire host implementation for low-level bus communication, providing reliable bit-level operations with precise timing control. The protocol registry pattern allows for easy extension with new protocols while maintaining a consistent interface for applications.

This documentation provides a detailed understanding of the iButton protocols supported by the Flipper Zero, enabling developers to effectively use and extend the system for various applications involving iButton technology.