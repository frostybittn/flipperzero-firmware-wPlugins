# iButton File Formats

<cite>
**Referenced Files in This Document**   
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)
- [ibutton_key.c](file://lib/ibutton/ibutton_key.c)
- [ibutton_worker.c](file://lib/ibutton/ibutton_worker.c)
- [ibutton_worker_modes.c](file://lib/ibutton/ibutton_worker_modes.c)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Flipper Format Container Structure](#flipper-format-container-structure)
3. [Protocol-Specific Sections and Key Data](#protocol-specific-sections-and-key-data)
4. [Metadata Fields](#metadata-fields)
5. [Binary Data Structure and Memory Organization](#binary-data-structure-and-memory-organization)
6. [File Format Versioning and Compatibility](#file-format-versioning-and-compatibility)
7. [Programmatic File Creation and Parsing](#programmatic-file-creation-and-parsing)
8. [Code Examples from ibutton_protocols.c](#code-examples-from-ibutton_protocolsc)
9. [Conclusion](#conclusion)

## Introduction
The iButton file format is used by the Flipper Zero device to store and manage iButton key data. This document provides a comprehensive overview of the file format structure, including the Flipper Format container, protocol-specific sections, metadata fields, and binary data organization. The format supports multiple iButton protocols such as DS1990, DS1992, DS1996, DS1971, DS1420, DSGeneric, Cyfral, and Metakom. The documentation also covers file format versioning, backward compatibility, and programmatic creation and parsing of iButton files.

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)

## Flipper Format Container Structure
The iButton file format is based on the Flipper Format container, which provides a structured way to store key data. The container includes a header with metadata and a body with protocol-specific data.

### Header Fields
The header of the iButton file contains the following fields:
- **Filetype**: A string indicating the file type, which is "Flipper iButton key".
- **Version**: An integer indicating the format version, currently 2.
- **Protocol**: A string indicating the iButton protocol used, such as DS1990, DS1992, etc.

### Example File
```
Filetype: Flipper iButton key
Version: 2
Protocol: DS1992
Rom Data: 08 DE AD BE EF FA CE 4E
Sram Data: 4E 65 76 65 72 47 6F 6E 6E 61 47 69 76 65 59 6F 75 55 70 4E 65 76 65 72 47 6F 6E 6E 61 4C 65 74 59 6F 75 44 6F 77 6E 4E 65 76 65 72 47 6F 6E 6E 61 52 75 6E 41 72 6F 75 6E 64 41 6E 64 44 65 73 65 72 74 59 6F 75 4E 65 76 65 72 47 6F 6E 6E 61 4D 61 6B 65 59 6F 75 43 72 79 4E 65 76 65 72 47 6F 6E 6E 61 53 61 79 47 6F 6F 64 62 79 65 4E 65 76 65 72 47 6F 6E 6E 61 54 65 6C 6C 41 4C 69 65
```

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)

## Protocol-Specific Sections and Key Data
The iButton file format includes protocol-specific sections that store key data based on the iButton protocol used. The fields after the `Protocol` field are protocol-dependent.

### Supported Protocols
- **DS1990**: Dallas Semiconductor 1-Wire protocol.
- **DS1992**: Dallas Semiconductor 1-Wire protocol with SRAM.
- **DS1996**: Dallas Semiconductor 1-Wire protocol with SRAM.
- **DS1971**: Dallas Semiconductor 1-Wire protocol with EEPROM.
- **DS1420**: Dallas Semiconductor 1-Wire protocol, compatible with DS1990.
- **DSGeneric**: Catch-all protocol for unknown 1-Wire devices.
- **Cyfral**: Russian iButton protocol.
- **Metakom**: Russian iButton protocol.

### Protocol-Specific Fields
- **Rom Data**: Read-only memory data for Dallas protocols.
- **Sram Data**: Static RAM data for DS1992 and DS1996.
- **Eeprom Data**: EEPROM data for DS1971.
- **Data**: Key data for Cyfral and Metakom.

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)

## Metadata Fields
The metadata fields in the iButton file format provide essential information about the iButton key.

### Button Type
The `Protocol` field specifies the type of iButton, such as DS1990, DS1992, etc.

### Serial Number
The serial number is part of the `Rom Data` field for Dallas protocols.

### Page Data
Page data is stored in the `Sram Data` or `Eeprom Data` fields, depending on the protocol.

### Authentication Information
Authentication information is protocol-specific and may be included in the `Data` field for Cyfral and Metakom protocols.

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)

## Binary Data Structure and Memory Organization
The binary structure of the iButton data section includes memory page organization and cryptographic data for secure iButtons.

### Memory Page Organization
- **DS1992 and DS1996**: 256 bytes of SRAM organized in pages.
- **DS1971**: 1024 bytes of EEPROM organized in pages.

### Cryptographic Data
Cryptographic data for secure iButtons is stored in the `Data` field and may include encryption keys and authentication codes.

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)

## File Format Versioning and Compatibility
The iButton file format supports versioning to ensure backward compatibility and extension mechanisms.

### Version History
- **Version 2**: Current version with support for different Dallas protocols and protocol-dependent fields.
- **Version 1**: Initial version, deprecated and converted to version 2 upon saving.

### Backward Compatibility
The format ensures backward compatibility by supporting both version 1 and version 2 files. Version 1 files are automatically converted to version 2 when saved.

### Extension Mechanisms
New protocols can be added without increasing the format version by defining the format in the protocol implementation.

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)

## Programmatic File Creation and Parsing
The iButton file format can be created and parsed programmatically using the Flipper Zero firmware libraries.

### Creating iButton Files
The `ibutton_protocols_save` function in `ibutton_protocols.c` is used to create iButton files. It writes the header and protocol-specific data to a file.

### Parsing iButton Files
The `ibutton_protocols_load` function in `ibutton_protocols.c` is used to parse iButton files. It reads the header and protocol-specific data from a file.

**Section sources**
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)

## Code Examples from ibutton_protocols.c
The following code examples from `ibutton_protocols.c` demonstrate how iButton files are created and saved.

### Saving an iButton File
```c
bool ibutton_protocols_save(
    iButtonProtocols* protocols,
    const iButtonKey* key,
    const char* file_name) {
    furi_check(protocols);
    furi_check(key);
    furi_check(file_name);

    const iButtonProtocolId id = ibutton_key_get_protocol_id(key);
    const iButtonProtocolData* data = ibutton_key_get_protocol_data(key);

    bool success = false;
    Storage* storage = furi_record_open(RECORD_STORAGE);

    FlipperFormat* ff = flipper_format_buffered_file_alloc(storage);

    do {
        const char* protocol_name = ibutton_protocols_get_name(protocols, id);

        if(!flipper_format_buffered_file_open_always(ff, file_name)) break;

        if(!flipper_format_write_header_cstr(ff, IBUTTON_FILE_TYPE, IBUTTON_CURRENT_FORMAT_VERSION))
            break;
        if(!flipper_format_write_string_cstr(ff, IBUTTON_PROTOCOL_KEY_V2, protocol_name)) break;

        GET_PROTOCOL_GROUP(id);
        if(!GROUP_BASE->save(GROUP_DATA, data, PROTOCOL_ID, ff)) break;

        success = true;
    } while(false);

    flipper_format_free(ff);
    furi_record_close(RECORD_STORAGE);

    return success;
}
```

### Loading an iButton File
```c
bool ibutton_protocols_load(iButtonProtocols* protocols, iButtonKey* key, const char* file_name) {
    furi_check(protocols);
    furi_check(key);
    furi_check(file_name);

    iButtonProtocolData* data = ibutton_key_get_protocol_data(key);

    bool success = false;
    Storage* storage = furi_record_open(RECORD_STORAGE);

    FlipperFormat* ff = flipper_format_buffered_file_alloc(storage);
    FuriString* tmp = furi_string_alloc();

    do {
        if(!flipper_format_buffered_file_open_existing(ff, file_name)) break;

        uint32_t version;

        if(!flipper_format_read_header(ff, tmp, &version)) break;
        if(!furi_string_equal(tmp, IBUTTON_FILE_TYPE)) break;

        if(version == 1) {
            if(!flipper_format_read_string(ff, IBUTTON_PROTOCOL_KEY_V1, tmp)) break;
        } else if(version == 2) {
            if(!flipper_format_read_string(ff, IBUTTON_PROTOCOL_KEY_V2, tmp)) break;
        } else {
            break;
        }

        const iButtonProtocolId id =
            ibutton_protocols_get_id_by_name(protocols, furi_string_get_cstr(tmp));
        ibutton_key_set_protocol_id(key, id);

        GET_PROTOCOL_GROUP(id);
        if(!GROUP_BASE->load(GROUP_DATA, data, PROTOCOL_ID, version, ff)) break;

        success = true;
    } while(false);

    flipper_format_free(ff);
    furi_string_free(tmp);
    furi_record_close(RECORD_STORAGE);

    return success;
}
```

**Section sources**
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)

## Conclusion
The iButton file format used by the Flipper Zero is a structured and extensible format for storing iButton key data. It supports multiple protocols and ensures backward compatibility through versioning. The format can be created and parsed programmatically using the provided firmware libraries, making it easy to integrate into applications.

**Section sources**
- [iButtonFileFormat.md](file://documentation/file_formats/iButtonFileFormat.md)
- [ibutton_protocols.c](file://lib/ibutton/ibutton_protocols.c)