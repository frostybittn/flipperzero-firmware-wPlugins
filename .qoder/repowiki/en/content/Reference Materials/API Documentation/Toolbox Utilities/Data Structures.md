# Data Structures

<cite>
**Referenced Files in This Document**   
- [bit_buffer.h](file://lib/toolbox/bit_buffer.h)
- [bit_buffer.c](file://lib/toolbox/bit_buffer.c)
- [simple_array.h](file://lib/toolbox/simple_array.h)
- [simple_array.c](file://lib/toolbox/simple_array.c)
- [varint.h](file://lib/toolbox/varint.h)
- [varint.c](file://lib/toolbox/varint.c)
- [value_index.h](file://lib/toolbox/value_index.h)
- [value_index.c](file://lib/toolbox/value_index.c)
- [keys_dict.h](file://lib/toolbox/keys_dict.h)
- [keys_dict.c](file://lib/toolbox/keys_dict.c)
- [protocol_dict.h](file://lib/toolbox/protocols/protocol_dict.h)
- [protocol_dict.c](file://lib/toolbox/protocols/protocol_dict.c)
- [protocol.h](file://lib/toolbox/protocols/protocol.h)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Bit Buffer](#bit-buffer)
3. [Simple Array](#simple-array)
4. [Varint](#varint)
5. [Value Index](#value-index)
6. [Keys Dictionary](#keys-dictionary)
7. [Protocol Dictionary](#protocol-dictionary)
8. [Performance and Optimization](#performance-and-optimization)
9. [Best Practices and Common Pitfalls](#best-practices-and-common-pitfalls)

## Introduction
The Toolbox library in the Flipper Zero firmware provides a suite of efficient data structure utilities designed for embedded systems with constrained resources. These utilities enable developers to perform bit-level operations, manage dynamic arrays, encode integers compactly, map values to indices, and maintain key-value dictionaries. This document details the implementation, usage, performance characteristics, and best practices for these core data structures: `bit_buffer`, `simple_array`, `varint`, `value_index`, `keys_dict`, and `protocol_dict`. Each structure is optimized for low memory overhead and high performance in resource-limited environments.

## Bit Buffer
The `bit_buffer` utility provides a flexible way to manipulate data at the bit level, which is essential for protocols and encoding schemes that do not align with byte boundaries. It supports dynamic allocation, copying, and writing of bit streams with optional parity bit handling.

### Structure and Initialization
The `BitBuffer` structure encapsulates a dynamically allocated byte array (`data`) and a separate parity bit array (`parity`). It tracks the buffer's capacity in bytes and the current size in bits. The structure is allocated using `bit_buffer_alloc()`, which initializes both the data and parity buffers based on the specified capacity.

```c
BitBuffer* bit_buffer_alloc(size_t capacity_bytes);
```

This function ensures that the buffer can hold up to `capacity_bytes` of data, with an additional parity buffer sized to accommodate one parity bit per byte of data.

### Memory Management
Memory is managed through explicit allocation and deallocation functions:
- `bit_buffer_free()` releases all allocated memory.
- `bit_buffer_reset()` clears the buffer contents without deallocating memory, setting the size to zero.

These functions use `furi_check()` for input validation, ensuring robust error handling in embedded contexts.

### Data Operations
The `bit_buffer` supports various data operations:
- **Copying**: `bit_buffer_copy()` duplicates another buffer's contents.
- **Slicing**: `bit_buffer_copy_right()` and `bit_buffer_copy_left()` copy portions of a buffer.
- **Writing**: `bit_buffer_write_bytes()` outputs data to a specified memory location.

Parity-aware operations like `bit_buffer_copy_bytes_with_parity()` and `bit_buffer_write_bytes_with_parity()` handle data with embedded parity bits, useful in error-detecting protocols.

### Performance Characteristics
- **Time Complexity**: O(n) for copy and write operations, where n is the number of bits.
- **Memory Overhead**: Approximately 12.5% additional overhead for parity storage (1 bit per byte).
- **Use Cases**: Ideal for RFID, infrared, and sub-GHz signal processing where bit-level precision is required.

**Section sources**
- [bit_buffer.h](file://lib/toolbox/bit_buffer.h#L1-L199)
- [bit_buffer.c](file://lib/toolbox/bit_buffer.c#L1-L199)

## Simple Array
The `simple_array` is a generic, non-type-safe dynamic array implementation that supports in-place construction and destruction of elements. It is designed for scenarios where type safety is less critical than memory efficiency and performance.

### Configuration and Initialization
The array is configured via a `SimpleArrayConfig` structure that specifies initialization, reset, and copy functions, along with the element size. This allows customization for different data types.

```c
typedef struct {
    SimpleArrayInit init;
    SimpleArrayReset reset;
    SimpleArrayCopy copy;
    const size_t type_size;
} SimpleArrayConfig;
```

Arrays are allocated with `simple_array_alloc()` and initialized with `simple_array_init()`, which reserves space for a specified number of elements.

### Element Access
Elements are accessed via index-based functions:
- `simple_array_get()` returns a pointer to an element.
- `simple_array_cget()` provides const access.
- `simple_array_get_data()` returns a pointer to the entire data block.

These functions include bounds checking via `furi_check()`.

### Memory Management
- `simple_array_reset()` clears all elements, calling the reset function if provided.
- `simple_array_free()` deallocates the array and its data.
- `simple_array_copy()` duplicates another array's contents, using the copy function if available.

### Performance Characteristics
- **Time Complexity**: O(1) for access, O(n) for initialization and copying.
- **Memory Overhead**: Minimal; only the size of the structure and data array.
- **Use Cases**: Suitable for storing collections of homogeneous data, such as sensor readings or configuration parameters.

**Section sources**
- [simple_array.h](file://lib/toolbox/simple_array.h#L1-L148)
- [simple_array.c](file://lib/toolbox/simple_array.c#L1-L129)

## Varint
The `varint` utility implements variable-length integer encoding, which compresses integers by using fewer bytes for smaller values. This is particularly useful for reducing storage and transmission overhead.

### Encoding and Decoding
- `varint_uint32_pack()` encodes a 32-bit unsigned integer into a variable-length byte array.
- `varint_uint32_unpack()` decodes a varint back to an integer.
- `varint_int32_pack()` and `varint_int32_unpack()` handle signed integers using zigzag encoding.

Each byte in the encoded form uses the most significant bit (MSB) as a continuation flag, with the remaining 7 bits storing data.

### Length Calculation
- `varint_uint32_length()` and `varint_int32_length()` compute the encoded size without performing the actual encoding, useful for buffer sizing.

### Performance Characteristics
- **Time Complexity**: O(log n) for encoding and decoding, where n is the integer value.
- **Space Efficiency**: 1 byte for values 0–127, 2 bytes for 128–16,383, etc.
- **Use Cases**: Ideal for encoding protocol messages, file formats, and network packets where size efficiency is critical.

**Section sources**
- [varint.h](file://lib/toolbox/varint.h#L1-L35)
- [varint.c](file://lib/toolbox/varint.c#L1-L76)

## Value Index
The `value_index` utility provides functions to find the index of an element in an array that is closest to a given value. It supports multiple data types, including integers, floats, and booleans.

### Search Functions
- `value_index_int32()`, `value_index_uint32()`, and `value_index_bool()` perform exact matches.
- `value_index_float()` uses a relative epsilon (1%) for floating-point comparisons, accommodating precision issues.

All functions return the index of the first matching element or 0 if no match is found.

### Implementation
The search is implemented as a linear scan with early termination upon finding a match. For floats, the comparison uses `fabsf()` to compute the absolute difference and compares it against an epsilon derived from the value.

### Performance Characteristics
- **Time Complexity**: O(n) in the worst case, where n is the array size.
- **Use Cases**: Useful for mapping sensor values to predefined ranges or finding configuration settings.

**Section sources**
- [value_index.h](file://lib/toolbox/value_index.h#L1-L65)
- [value_index.c](file://lib/toolbox/value_index.c#L1-L55)

## Keys Dictionary
The `keys_dict` is a file-based key-value store that manages hexadecimal keys stored as text. It is designed for persistent storage of cryptographic keys or identifiers.

### File Format and Access
Keys are stored in a plain text file, one per line, in uppercase hexadecimal format. Comments (lines starting with '#') are ignored. The dictionary supports two modes:
- `KeysDictModeOpenExisting`: Opens an existing file.
- `KeysDictModeOpenAlways`: Creates the file if it does not exist.

### Operations
- `keys_dict_alloc()` opens or creates the dictionary.
- `keys_dict_add_key()` appends a key to the file.
- `keys_dict_is_key_present()` checks for key existence via linear search.
- `keys_dict_get_next_key()` iterates through keys.
- `keys_dict_delete_key()` removes a key (not implemented in provided code).

Keys are converted between binary and string representations using helper functions.

### Performance Characteristics
- **Time Complexity**: O(n) for searches and deletions due to linear file scanning.
- **Memory Usage**: Keys are not loaded into memory; only one key is processed at a time.
- **Use Cases**: Storing RFID or NFC card keys, where persistence and simplicity are prioritized over speed.

**Section sources**
- [keys_dict.h](file://lib/toolbox/keys_dict.h#L1-L103)
- [keys_dict.c](file://lib/toolbox/keys_dict.c#L1-L199)

## Protocol Dictionary
The `protocol_dict` manages a collection of protocol handlers, enabling dynamic selection and execution of encoding/decoding logic for various communication protocols.

### Structure and Initialization
The dictionary is initialized with an array of `ProtocolBase` structures, each defining a protocol's behavior:
- `alloc` and `free` for instance management.
- `decoder` and `encoder` for signal processing.
- `render` functions for string representation.

```c
ProtocolDict* protocol_dict_alloc(const ProtocolBase** protocols, size_t protocol_count);
```

### Protocol Operations
- `protocol_dict_decoders_feed()` processes signal levels and durations, returning the ID of a protocol that has successfully decoded a signal.
- `protocol_dict_encoder_start()` and `protocol_dict_encoder_yield()` generate output signals.
- `protocol_dict_get_data()` and `protocol_dict_set_data()` access protocol-specific data.

### Feature-Based Filtering
The dictionary supports feature-based decoding via `protocol_dict_decoders_feed_by_feature()`, allowing selective processing of protocols with specific capabilities.

### Performance Characteristics
- **Time Complexity**: O(n) for decoding, where n is the number of protocols.
- **Memory Usage**: One instance per protocol, with data stored in a flexible array member.
- **Use Cases**: Sub-GHz and infrared remote control applications, where multiple protocols must be supported.

**Section sources**
- [protocol_dict.h](file://lib/toolbox/protocols/protocol_dict.h#L1-L75)
- [protocol_dict.c](file://lib/toolbox/protocols/protocol_dict.c#L1-L199)
- [protocol.h](file://lib/toolbox/protocols/protocol.h#L1-L47)

## Performance and Optimization
Each data structure is optimized for embedded use:
- **Memory Efficiency**: Minimal overhead, with dynamic allocation only when necessary.
- **Speed**: Linear time operations dominate, suitable for real-time processing.
- **Persistence**: File-based dictionaries avoid loading large datasets into RAM.

Optimization strategies include:
- Pre-sizing buffers to avoid reallocation.
- Using varint encoding for compact data storage.
- Iterating through keys_dict without loading all keys into memory.

## Best Practices and Common Pitfalls
### Best Practices
- **Initialize Properly**: Always check return values from allocation functions.
- **Manage Memory**: Use `free()` functions to prevent leaks.
- **Validate Inputs**: Ensure buffer sizes and indices are within bounds.
- **Use Appropriate Structures**: Choose varint for small integers, bit_buffer for bit streams.

### Common Pitfalls
- **Ignoring Parity**: Failing to handle parity bits correctly in bit_buffer operations.
- **Buffer Overflows**: Not checking capacity before copying data.
- **File Corruption**: Improper handling of keys_dict files can lead to data loss.
- **Linear Search Overhead**: keys_dict performance degrades with large key sets.

By adhering to these guidelines, developers can effectively leverage the Toolbox data structures for robust and efficient embedded applications.