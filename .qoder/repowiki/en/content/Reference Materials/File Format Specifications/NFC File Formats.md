# NFC File Formats

<cite>
**Referenced Files in This Document**   
- [nfc_device.c](file://lib/nfc/nfc_device.c#L0-L361)
- [Ultralight_C.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ultralight_C.nfc#L0-L72)
- [Ntag213_locked.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ntag213_locked.nfc#L0-L67)
- [Felica.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Felica.nfc#L0-L41)
- [Ntag215.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ntag215.nfc#L0-L157)
- [Ntag216.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ntag216.nfc#L0-L253)
- [Slix_cap_default.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Slix_cap_default.nfc#L0-L42)
- [Slix_cap_accept_all_pass.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Slix_cap_accept_all_pass.nfc#L0-L42)
- [Slix_cap_missed.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Slix_cap_missed.nfc#L0-L40)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Flipper Format Container Structure](#flipper-format-container-structure)
3. [Protocol-Specific Sections](#protocol-specific-sections)
4. [Metadata Fields](#metadata-fields)
5. [Binary Data Layout](#binary-data-layout)
6. [File Format Versioning](#file-format-versioning)
7. [Code Implementation](#code-implementation)
8. [Examples of Valid NFC Files](#examples-of-valid-nfc-files)
9. [Parsing and Generation](#parsing-and-generation)
10. [Conclusion](#conclusion)

## Introduction
The Flipper Zero NFC file format is a standardized container format used to store NFC tag data for various protocols. This document details the structure, metadata, and implementation of NFC files used by the Flipper Zero device. The format supports multiple NFC protocols including ISO14443-3A, ISO14443-4A, Mifare Classic, Mifare Ultralight, FeliCa, and SLIX. The format is designed to be human-readable while maintaining compatibility with the Flipper Format serialization system.

## Flipper Format Container Structure
The NFC file format is built on the Flipper Format container system, which uses a key-value structure with comments for documentation. Each NFC file begins with a header that identifies the file type and version.

```text
Filetype: Flipper NFC device
Version: 4
```

The container structure follows these principles:
- **Header**: Identifies the file as a Flipper NFC device with version number
- **Comments**: Provide documentation for fields using # prefix
- **Key-Value Pairs**: Store metadata and configuration data
- **Protocol-Specific Sections**: Contain data specific to the NFC protocol type

The format uses the `flipper_format` library for serialization and deserialization, ensuring consistency across different applications and devices.

**Section sources**
- [nfc_device.c](file://lib/nfc/nfc_device.c#L0-L361)

## Protocol-Specific Sections
The NFC file format includes different sections based on the specific protocol type. The `Device type` field determines which sections are present and how the data is interpreted.

### ISO14443-3A/4A
For ISO14443-3A and ISO14443-4A protocols, the file includes:
- **ATQA**: Answer to Request Type A (2 bytes)
- **SAK**: Select Acknowledgment (1 byte)
- **UID**: Unique Identifier (variable length)

### Mifare Classic
Mifare Classic tags use a block-based structure with 16-byte blocks organized into sectors. The file format includes:
- **UID**: Card identifier
- **ATQA/SAK**: Card response data
- **Block organization**: Data organized in 16-byte blocks
- **Sector trailers**: Special blocks containing access conditions and keys

### Mifare Ultralight
Mifare Ultralight and NTAG formats use a page-based structure with 4-byte pages. The file includes:
- **Data format version**: Version of the data format
- **Signature**: 32-byte signature data
- **Mifare version**: 8-byte version information
- **Counter and Tearing values**: Three counter/tearing pairs
- **Pages total/Pages read**: Total and read page counts
- **Page data**: Individual page contents (Page 0, Page 1, etc.)

### FeliCa
FeliCa protocol files contain:
- **Manufacture id**: 8-byte manufacturer identifier
- **Manufacture parameter**: 8-byte parameter data
- **Blocks total/Blocks read**: Total and read block counts
- **Block data**: 16-byte block contents (Block 0, Block 1, etc.)

### SLIX
SLIX protocol files include:
- **DSFID**: Data Storage Format Identifier (1 byte)
- **AFI**: Application Family Identifier (1 byte)
- **IC Reference**: IC reference number (1 byte)
- **Lock bits**: Lock DSFID, Lock AFI, Lock EAS, Lock PPL
- **Block Count/Block Size**: Memory configuration
- **Data Content**: Raw block data
- **Security Status**: Block security status
- **Capabilities**: Emulation capabilities (Default, AcceptAllPasswords)
- **Passwords**: Read, Write, Privacy, Destroy, and EAS passwords
- **Signature**: secp128r1 elliptic curve signature
- **Privacy Mode**: Boolean privacy setting
- **Protection Pointer/Condition**: Protection configuration

**Section sources**
- [Ultralight_C.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ultralight_C.nfc#L0-L72)
- [Felica.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Felica.nfc#L0-L41)
- [Slix_cap_default.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Slix_cap_default.nfc#L0-L42)

## Metadata Fields
The NFC file format includes several standard metadata fields that are common across all protocol types.

### UID (Unique Identifier)
The UID field contains the card's unique identifier in hexadecimal format. It is common to all NFC formats and is typically 4-10 bytes long. The UID is used for card identification and anti-collision procedures.

```text
UID: 04 BA FF CA 4D 5D 80
```

### ATQA (Answer to Request Type A)
ATQA is a 2-byte response from Type A cards when they are powered up. It provides information about the card's capabilities and is used in the anti-collision process.

```text
ATQA: 00 44
```

### SAK (Select Acknowledgment)
SAK is a 1-byte response that indicates the card type and capabilities after successful anti-collision. It helps determine the specific protocol to use for communication.

```text
SAK: 00
```

### Protocol Type
The Device type field specifies the NFC protocol used by the tag. This determines how the rest of the file is interpreted. Supported types include:
- NTAG/Ultralight
- Mifare Classic
- FeliCa
- SLIX
- ISO14443-3A
- ISO14443-4A

```text
Device type: NTAG/Ultralight
```

### Authentication Data
For protocols that support authentication (like Mifare Classic), the file may include key data and authentication attempts:
- **Failed authentication attempts**: Counter of failed authentication attempts
- **Key data**: Stored in sector trailers for Mifare Classic

```text
Failed authentication attempts: 0
```

**Section sources**
- [Ultralight_C.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ultralight_C.nfc#L0-L72)
- [Ntag213_locked.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ntag213_locked.nfc#L0-L67)

## Binary Data Layout
The binary data layout varies significantly between different NFC protocols, reflecting the physical structure of the actual tags.

### Mifare Ultralight/NTAG Page Organization
Mifare Ultralight and NTAG tags organize data in 4-byte pages. The file format represents this with sequential Page entries:

```text
Page 0: 04 BA FF C9
Page 1: CA 4D 5D 80
Page 2: 5A 48 00 00
Page 3: E1 10 12 00
```

Each page corresponds to a physical memory page on the tag. Pages 0-2 typically contain the UID and manufacturer data, while page 3 contains the capability container.

### Sector Trailers in Mifare Classic
Mifare Classic tags organize data into sectors, each ending with a sector trailer that contains access conditions and keys. Each sector trailer is 16 bytes and includes:
- **Key A**: 6 bytes
- **Access bits**: 4 bytes
- **User data**: 6 bytes
- **Key B**: 6 bytes

The access bits control read/write permissions for the three data blocks in the sector.

### Value Block Format
Some Mifare cards support value blocks that store numeric values with built-in security features. A value block contains:
- **Value**: 32-bit signed integer (stored three times for redundancy)
- **Address**: 32-bit address field (stored three times)
- **Value integrity**: The three copies must match

This format provides protection against unauthorized modification.

### FeliCa Block Structure
FeliCa blocks are 16 bytes each and are organized sequentially:

```text
Block 0: 00 00 DE AD BE AF 00 00 00 00 00 00 00 00 DE AD BE AF
Block 1: 00 00 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
```

Each block can be individually accessed and has its own access permissions.

**Section sources**
- [Ultralight_C.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ultralight_C.nfc#L0-L72)
- [Ntag215.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ntag215.nfc#L0-L157)
- [Felica.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Felica.nfc#L0-L41)

## File Format Versioning
The NFC file format includes versioning to ensure backward compatibility and support for future extensions.

### Version Field
The Version field indicates the format version:
```text
Version: 4
```

This allows the software to handle different versions appropriately and provides a mechanism for format evolution.

### Backward Compatibility
The format is designed to be backward compatible:
- Newer versions can read older format files
- Missing fields in older files are assigned default values
- Unknown fields in newer files are ignored by older software

### Extension Mechanisms
The format supports extensions through:
- **Comments**: For documentation and future field descriptions
- **Optional fields**: Fields that may not be present in all files
- **Protocol-specific sections**: Isolated sections that can evolve independently

This allows new protocols and features to be added without breaking existing implementations.

**Section sources**
- [Ultralight_C.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ultralight_C.nfc#L0-L72)
- [Slix_cap_default.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Slix_cap_default.nfc#L0-L42)

## Code Implementation
The NFC file format implementation is centered around the `nfc_device.c` file, which handles the saving and loading of NFC data.

### Saving Process
The `nfc_device_save` function in `nfc_device.c` implements the file saving process:

```c
bool nfc_device_save(NfcDevice* instance, const char* path) {
    // Open file and write header
    if(!flipper_format_buffered_file_open_always(ff, path)) break;
    
    // Write header
    if(!flipper_format_write_header_cstr(ff, NFC_FILE_HEADER, NFC_CURRENT_FORMAT_VERSION))
        break;
        
    // Write device type
    if(!flipper_format_write_string_cstr(
           ff, NFC_DEVICE_TYPE_KEY, nfc_devices[instance->protocol]->protocol_name))
        break;
        
    // Write UID
    size_t uid_len;
    const uint8_t* uid = nfc_device_get_uid(instance, &uid_len);
    if(!flipper_format_write_hex(ff, NFC_DEVICE_UID_KEY, uid, uid_len)) break;
        
    // Write protocol-dependent data
    if(!nfc_devices[instance->protocol]->save(instance->protocol_data, ff)) break;
}
```

The process follows these steps:
1. Open the file using `flipper_format_buffered_file_open_always`
2. Write the file header with format version
3. Write the device type
4. Write the UID and other common fields
5. Delegate protocol-specific data writing to the protocol's save function

### Protocol-Specific Save Functions
Each protocol implements its own save function that writes the protocol-specific data to the file. These functions are registered in the `nfc_devices` array and called dynamically based on the device protocol.

The use of function pointers allows for extensible architecture where new protocols can be added without modifying the core saving logic.

**Section sources**
- [nfc_device.c](file://lib/nfc/nfc_device.c#L0-L361)

## Examples of Valid NFC Files
This section provides examples of valid NFC files for different tag types.

### Mifare Ultralight C Example
```text
Filetype: Flipper NFC device
Version: 4
Device type: NTAG/Ultralight
UID: 04 BA FF CA 4D 5D 80
ATQA: 00 44
SAK: 00
Data format version: 2
NTAG/Ultralight type: Mifare Ultralight C
Signature: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Mifare version: 00 00 00 00 00 00 00 00
Counter 0: 0
Tearing 0: 00
Pages total: 48
Pages read: 48
Page 0: 04 BA FF C9
Page 1: CA 4D 5D 80
Page 2: 5A 48 00 00
Page 3: E1 10 12 00
```

### SLIX with AcceptAllPasswords Capability
```text
Filetype: Flipper NFC device
Version: 4
Device type: SLIX
UID: E0 04 01 08 49 D0 DC 81
DSFID: 01
AFI: 3D
IC Reference: 01
Lock DSFID: true
Lock AFI: true
Block Count: 80
Block Size: 04
Capabilities: AcceptAllPasswords
Password Read: 00 00 00 00
Password Write: 00 00 00 00
Password Privacy: 0F 0F 0F 0F
Password Destroy: 0F 0F 0F 0F
Password EAS: 00 00 00 00
Privacy Mode: false
Protection Pointer: 32
Protection Condition: 02
Lock EAS: true
Lock PPL: true
```

### FeliCa Example
```text
Filetype: Flipper NFC device
Version: 4
Device type: FeliCa
UID: 29 9F FA 53 AB 75 87 6E
Manufacture id: 29 9F FA 53 AB 75 87 6E
Manufacture parameter: 57 4E 10 2A 94 16 BC 8E
Blocks total: 28
Blocks read: 28
Block 0: 00 00 DE AD BE AF 00 00 00 00 00 00 00 00 DE AD BE AF
Block 1: 00 00 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
```

**Section sources**
- [Ultralight_C.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Ultralight_C.nfc#L0-L72)
- [Slix_cap_accept_all_pass.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Slix_cap_accept_all_pass.nfc#L0-L42)
- [Felica.nfc](file://applications/debug/unit_tests/resources/unit_tests/nfc/Felica.nfc#L0-L41)

## Parsing and Generation
### Parsing NFC Files
To parse an NFC file programmatically:
1. Open the file using `flipper_format_buffered_file_open_existing`
2. Verify the header using `flipper_format_read_header`
3. Read the device type to determine the protocol
4. Read the UID and other common fields
5. Delegate to the protocol-specific load function

### Generating NFC Files
To generate an NFC file:
1. Create an `NfcDevice` instance
2. Set the protocol type
3. Set the UID and other common data
4. Set protocol-specific data
5. Call `nfc_device_save` with the desired file path

### Programmatic Example
```c
// Create new NFC device
NfcDevice* device = nfc_device_alloc();
nfc_device_set_data(device, NfcProtocolNTAG213, &ntag_data);

// Set UID
uint8_t uid[] = {0x04, 0xAC, 0x6B, 0x72, 0xBA, 0x6C, 0x80};
nfc_device_set_uid(device, uid, 7);

// Save to file
nfc_device_save(device, "/ext/nfc/my_tag.nfc");

// Cleanup
nfc_device_free(device);
```

**Section sources**
- [nfc_device.c](file://lib/nfc/nfc_device.c#L0-L361)

## Conclusion
The Flipper Zero NFC file format provides a comprehensive and extensible system for storing NFC tag data across multiple protocols. The format combines human-readable text with structured data to create files that are both accessible to users and reliable for programmatic processing. By leveraging the Flipper Format container system, the implementation ensures consistency and backward compatibility while supporting a wide range of NFC protocols. The modular architecture with protocol-specific handlers allows for easy extension to support new NFC standards in the future.