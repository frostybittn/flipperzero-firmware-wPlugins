# Storage Service

<cite>
**Referenced Files in This Document**   
- [storage.h](file://applications/services/storage/storage.h)
- [storage.c](file://applications/services/storage/storage.c)
- [storage_i.h](file://applications/services/storage/storage_i.h)
- [storage_int.c](file://applications/services/storage/storages/storage_int.c)
- [storage_ext.c](file://applications/services/storage/storages/storage_ext.c)
- [lfs.h](file://lib/littlefs/lfs.h)
- [ff.h](file://lib/fatfs/ff.h)
- [storage_glue.h](file://applications/services/storage/storage_glue.h)
- [storage_processing.h](file://applications/services/storage/storage_processing.h)
- [filesystem_api_defines.h](file://applications/services/storage/filesystem_api_defines.h)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Project Structure](#project-structure)
3. [Core Components](#core-components)
4. [Architecture Overview](#architecture-overview)
5. [Detailed Component Analysis](#detailed-component-analysis)
6. [File System Drivers](#file-system-drivers)
7. [API Interface](#api-interface)
8. [Component Interactions](#component-interactions)
9. [Thread Safety and Asynchronous Operations](#thread-safety-and-asynchronous-operations)
10. [Cross-Cutting Concerns](#cross-cutting-concerns)
11. [System Context and Data Flow](#system-context-and-data-flow)

## Introduction
The Storage Service provides a unified abstraction layer for file system operations on the Flipper Zero device, enabling consistent access to both internal and external storage. This documentation details the architecture, API interface, and implementation of the storage service that unifies access to LittleFS for internal storage and FatFs for external SD card storage. The service provides a thread-safe, asynchronous interface for file operations, directory management, and storage information queries, serving as the foundation for all file system interactions in the Flipper Zero firmware.

## Project Structure
The storage service is organized in a modular architecture with clear separation between the abstraction layer, file system drivers, and client applications. The core implementation resides in the `applications/services/storage/` directory, with driver-specific code in the `storages/` subdirectory.

```mermaid
graph TD
subgraph "Storage Service"
A[storage.h] --> B[storage.c]
B --> C[storage_i.h]
B --> D[storage_processing.c]
B --> E[storage_glue.c]
F[storages/] --> G[storage_int.c]
F --> H[storage_ext.c]
F --> I[storage_int.h]
F --> J[storage_ext.h]
end
subgraph "File System Libraries"
K[lib/littlefs/] --> L[lfs.h]
M[lib/fatfs/] --> N[ff.h]
end
subgraph "Client Applications"
O[applications/main/]
P[applications/debug/]
Q[applications/examples/]
end
A --> K
A --> M
B --> F
O --> A
P --> A
Q --> A
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)
- [storage.c](file://applications/services/storage/storage.c)
- [storages/storage_int.c](file://applications/services/storage/storages/storage_int.c)
- [storages/storage_ext.c](file://applications/services/storage/storages/storage_ext.c)
- [lib/littlefs/lfs.h](file://lib/littlefs/lfs.h)
- [lib/fatfs/ff.h](file://lib/fatfs/ff.h)

**Section sources**
- [storage.h](file://applications/services/storage/storage.h)
- [storage.c](file://applications/services/storage/storage.c)

## Core Components
The storage service consists of several core components that work together to provide a unified interface for file system operations. The main components include the storage abstraction layer, internal storage driver (LittleFS), external storage driver (FatFs), message processing system, and glue layer that connects the components.

The service follows a service-oriented architecture where client applications interact with the storage service through a well-defined API, while the service handles the complexity of interacting with different file system drivers. The design emphasizes thread safety, power loss protection, and data integrity.

**Section sources**
- [storage.h](file://applications/services/storage/storage.h)
- [storage.c](file://applications/services/storage/storage.c)
- [storage_i.h](file://applications/services/storage/storage_i.h)

## Architecture Overview
The storage service implements a layered architecture that abstracts the differences between internal and external storage systems. The architecture follows a service pattern with message-based communication between components, ensuring thread safety and asynchronous operation.

```mermaid
graph TD
subgraph "Client Applications"
A[Application 1]
B[Application 2]
C[Application N]
end
subgraph "Storage Service"
D[Storage API]
E[Message Queue]
F[Storage Processing]
G[Storage Glue]
end
subgraph "File System Drivers"
H[Internal Storage Driver<br/>LittleFS]
I[External Storage Driver<br/>FatFs]
J[Virtual Mount Driver<br/>FatFs]
end
subgraph "Hardware"
K[Internal Flash]
L[SD Card]
M[VFS Image]
end
A --> D
B --> D
C --> D
D --> E
E --> F
F --> G
G --> H
G --> I
G --> J
H --> K
I --> L
J --> M
style D fill:#f9f,stroke:#333
style G fill:#ff9,stroke:#333
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)
- [storage.c](file://applications/services/storage/storage.c)
- [storage_processing.c](file://applications/services/storage/storage_processing.c)
- [storage_glue.c](file://applications/services/storage/storage_glue.c)

## Detailed Component Analysis

### Storage Abstraction Layer
The storage abstraction layer provides a unified API for file system operations regardless of the underlying storage type. It handles path resolution, error translation, and request queuing.

```mermaid
classDiagram
class Storage {
+File* storage_file_alloc()
+void storage_file_free()
+FuriPubSub* storage_get_pubsub()
+bool storage_file_open()
+bool storage_file_close()
+size_t storage_file_read()
+size_t storage_file_write()
+bool storage_dir_open()
+bool storage_dir_read()
+FS_Error storage_common_remove()
+FS_Error storage_common_rename()
+FS_Error storage_common_mkdir()
}
class File {
+bool storage_file_is_open()
+bool storage_file_is_dir()
+size_t storage_file_read()
+size_t storage_file_write()
+bool storage_file_seek()
+uint64_t storage_file_tell()
+bool storage_file_eof()
}
class FileInfo {
+uint64_t size
+uint32_t timestamp
+bool is_dir
+char name[256]
}
class StorageEvent {
+StorageEventType type
}
enum StorageEventType {
CardMount
CardUnmount
CardMountError
FileClose
DirClose
}
Storage --> File : "creates/manages"
Storage --> FileInfo : "uses for stat"
Storage --> StorageEvent : "publishes via PubSub"
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)
- [filesystem_api_defines.h](file://applications/services/storage/filesystem_api_defines.h)

### Message Processing System
The message processing system handles asynchronous operations and ensures thread safety through a message queue architecture.

```mermaid
sequenceDiagram
participant App as "Client Application"
participant API as "Storage API"
participant Queue as "Message Queue"
participant Processor as "Storage Processor"
participant Driver as "Storage Driver"
App->>API : storage_file_open()
API->>Queue : Post message
Queue-->>Processor : Message available
Processor->>Processor : Process message
Processor->>Driver : Execute operation
Driver-->>Processor : Return result
Processor->>API : Update result
API-->>App : Return success/failure
```

**Diagram sources**
- [storage.c](file://applications/services/storage/storage.c)
- [storage_processing.c](file://applications/services/storage/storage_processing.c)
- [storage_message.h](file://applications/services/storage/storage_message.h)

## File System Drivers

### Internal Storage Driver (LittleFS)
The internal storage driver implements the LittleFS file system for the device's internal flash memory. It provides wear leveling, power loss protection, and efficient storage utilization.

```mermaid
classDiagram
class LFSData {
+size_t start_address
+size_t start_page
+lfs_config config
+lfs_t lfs
}
class LFSHandle {
+void* data
+bool open
}
class lfs_config {
+void* context
+int (*read)()
+int (*prog)()
+int (*erase)()
+int (*sync)()
+lfs_size_t read_size
+lfs_size_t prog_size
+lfs_size_t block_size
+lfs_size_t block_count
+int32_t block_cycles
+lfs_size_t cache_size
+lfs_size_t lookahead_size
}
class lfs_t {
+lfs_cache_t rcache
+lfs_cache_t pcache
+lfs_block_t root[2]
+lfs_gstate_t gstate
+const lfs_config* cfg
}
LFSData --> lfs_config : "contains"
LFSData --> lfs_t : "contains"
LFSHandle --> lfs_file_t : "data points to"
LFSHandle --> lfs_dir_t : "data points to"
lfs_t --> lfs_config : "references"
```

**Diagram sources**
- [storages/storage_int.c](file://applications/services/storage/storages/storage_int.c)
- [lib/littlefs/lfs.h](file://lib/littlefs/lfs.h)

### External Storage Driver (FatFs)
The external storage driver implements the FatFs file system for SD card storage, providing compatibility with standard FAT file systems.

```mermaid
classDiagram
class SDData {
+FATFS* fs
+const char* path
+bool sd_was_present
}
class FATFS {
+BYTE fs_type
+BYTE drv
+BYTE n_fats
+WORD id
+WORD n_rootdir
+WORD csize
+DWORD n_fatent
+DWORD fsize
+DWORD volbase
+DWORD fatbase
+DWORD dirbase
+DWORD database
+DWORD winsect
+BYTE win[_MAX_SS]
}
class FIL {
+_FDID obj
+BYTE flag
+BYTE err
+FSIZE_t fptr
+DWORD clust
+DWORD sect
+BYTE buf[_MAX_SS]
}
class DIR {
+_FDID obj
+DWORD dptr
+DWORD clust
+DWORD sect
+BYTE* dir
+BYTE fn[12]
}
class FILINFO {
+FSIZE_t fsize
+WORD fdate
+WORD ftime
+BYTE fattrib
+TCHAR fname[13]
}
enum FRESULT {
FR_OK
FR_DISK_ERR
FR_NOT_READY
FR_NO_FILE
FR_NO_PATH
FR_DENIED
FR_EXIST
FR_INVALID_OBJECT
FR_WRITE_PROTECTED
}
SDData --> FATFS : "contains pointer"
FATFS --> FIL : "manages files"
FATFS --> DIR : "manages directories"
FIL --> FILINFO : "returns info"
DIR --> FILINFO : "returns info"
```

**Diagram sources**
- [storages/storage_ext.c](file://applications/services/storage/storages/storage_ext.c)
- [lib/fatfs/ff.h](file://lib/fatfs/ff.h)

## API Interface

### File Operations
The storage service provides a comprehensive API for file operations, including opening, reading, writing, and closing files.

```mermaid
flowchart TD
Start([File Operation]) --> Open["storage_file_open()"]
Open --> Check{"Success?"}
Check --> |Yes| ReadWrite["storage_file_read()/write()"]
Check --> |No| Error["Handle Error"]
ReadWrite --> Seek["storage_file_seek()"]
ReadWrite --> Tell["storage_file_tell()"]
ReadWrite --> Sync["storage_file_sync()"]
ReadWrite --> Truncate["storage_file_truncate()"]
Seek --> ReadWrite
Tell --> ReadWrite
Sync --> ReadWrite
Truncate --> ReadWrite
ReadWrite --> Close["storage_file_close()"]
Close --> End([Operation Complete])
Error --> Close
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)

### Directory Management
The API provides functions for directory creation, traversal, and manipulation.

```mermaid
flowchart TD
Start([Directory Operation]) --> Create["storage_common_mkdir()"]
Start --> Open["storage_dir_open()"]
Create --> Check1{"Success?"}
Check1 --> |Yes| Success1["Directory Created"]
Check1 --> |No| Error1["Handle Error"]
Open --> Check2{"Success?"}
Check2 --> |Yes| Read["storage_dir_read()"]
Check2 --> |No| Error2["Handle Error"]
Read --> More{"More Entries?"}
More --> |Yes| Read
More --> |No| Rewind["storage_dir_rewind()"]
Rewind --> Close["storage_dir_close()"]
Success1 --> End([Operation Complete])
Error1 --> End
Error2 --> End
Close --> End
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)

### Storage Information Queries
The service provides functions to query storage status, capacity, and file metadata.

```mermaid
flowchart TD
Start([Storage Query]) --> Info["storage_common_fs_info()"]
Start --> Stat["storage_common_stat()"]
Start --> Exists["storage_common_exists()"]
Start --> Timestamp["storage_common_timestamp()"]
Info --> Process["Retrieve storage capacity"]
Stat --> Process2["Retrieve file/directory info"]
Exists --> Process3["Check path existence"]
Timestamp --> Process4["Get last access time"]
Process --> Format["Return total/free space"]
Process2 --> Format2["Return FileInfo structure"]
Process3 --> Format3["Return boolean"]
Process4 --> Format4["Return timestamp"]
Format --> End([Query Complete])
Format2 --> End
Format3 --> End
Format4 --> End
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)

## Component Interactions
The storage service components interact through a well-defined message passing system that ensures thread safety and asynchronous operation.

```mermaid
sequenceDiagram
participant App as "Client Application"
participant StorageAPI as "Storage API"
participant MessageQueue as "Message Queue"
participant StorageProcessor as "Storage Processor"
participant InternalDriver as "Internal Storage Driver"
participant ExternalDriver as "External Storage Driver"
App->>StorageAPI : Request file operation
StorageAPI->>MessageQueue : Enqueue request message
loop Message Processing
MessageQueue-->>StorageProcessor : Message available
StorageProcessor->>StorageProcessor : Dequeue message
StorageProcessor->>StorageProcessor : Determine storage type
alt Internal Storage
StorageProcessor->>InternalDriver : Execute operation
InternalDriver-->>StorageProcessor : Return result
else External Storage
StorageProcessor->>ExternalDriver : Execute operation
ExternalDriver-->>StorageProcessor : Return result
end
StorageProcessor->>StorageAPI : Update operation result
end
StorageAPI-->>App : Return operation result
```

**Diagram sources**
- [storage.c](file://applications/services/storage/storage.c)
- [storage_processing.c](file://applications/services/storage/storage_processing.c)
- [storage_glue.c](file://applications/services/storage/storage_glue.c)

## Thread Safety and Asynchronous Operations
The storage service implements a thread-safe architecture using message queuing and a dedicated service thread.

```mermaid
stateDiagram-v2
[*] --> Idle
Idle --> Processing : "Message received"
Processing --> InternalOp : "Internal storage request"
Processing --> ExternalOp : "External storage request"
Processing --> VirtualOp : "Virtual mount request"
InternalOp --> Complete : "Operation finished"
ExternalOp --> Complete : "Operation finished"
VirtualOp --> Complete : "Operation finished"
Complete --> Idle : "Return to idle"
Processing --> Error : "Operation failed"
Error --> Idle : "Return to idle"
note right of Processing
All operations are
processed sequentially
in a single thread
end
```

**Diagram sources**
- [storage.c](file://applications/services/storage/storage.c)
- [storage_message.h](file://applications/services/storage/storage_message.h)

## Cross-Cutting Concerns

### Power Loss Protection
The storage service implements multiple mechanisms to protect against data corruption during power loss events.

```mermaid
flowchart TD
Start([Power Loss Event]) --> LittleFS["LittleFS Wear Leveling"]
Start --> FatFs["FatFs Write Caching"]
Start --> Sync["Explicit Sync Operations"]
Start --> Journaling["Metadata Journaling"]
LittleFS --> |Block Allocation| WearLeveling["Wear Leveling Across Blocks"]
FatFs --> |Cluster Allocation| Fragmentation["Minimize Fragmentation"]
Sync --> |f_sync()| CacheFlush["Flush Write Cache"]
Journaling --> |Atomic Updates| Consistency["File System Consistency"]
WearLeveling --> Protection["Data Integrity"]
Fragmentation --> Protection
CacheFlush --> Protection
Consistency --> Protection
Protection --> End([Protected Storage State])
```

**Diagram sources**
- [storages/storage_int.c](file://applications/services/storage/storages/storage_int.c)
- [storages/storage_ext.c](file://applications/services/storage/storages/storage_ext.c)

### Data Integrity Checks
The service implements comprehensive data integrity checks at multiple levels.

```mermaid
flowchart TD
Start([Data Operation]) --> PreCheck["Pre-operation Validation"]
PreCheck --> Execution["Execute Operation"]
Execution --> PostCheck["Post-operation Verification"]
PostCheck --> Logging["Error Logging"]
PreCheck --> |Path Validation| ValidatePath["Validate Path Format"]
PreCheck --> |Bounds Check| ValidateBounds["Check Size Limits"]
PreCheck --> |Permission| ValidateAccess["Check Access Rights"]
Execution --> |LittleFS| LFSIntegrity["LittleFS CRC Checks"]
Execution --> |FatFs| FATIntegrity["FAT Table Validation"]
PostCheck --> |Read Verification| VerifyRead["Compare Written Data"]
PostCheck --> |Metadata| VerifyMetadata["Validate Directory Entries"]
Logging --> |FURI_LOG| LogErrors["Log Error Details"]
Logging --> |PubSub| NotifyEvents["Publish Storage Events"]
ValidatePath --> Execution
ValidateBounds --> Execution
ValidateAccess --> Execution
LFSIntegrity --> PostCheck
FATIntegrity --> PostCheck
VerifyRead --> Logging
VerifyMetadata --> Logging
LogErrors --> End([Operation Complete])
NotifyEvents --> End
```

**Diagram sources**
- [storages/storage_int.c](file://applications/services/storage/storages/storage_int.c)
- [storages/storage_ext.c](file://applications/services/storage/storages/storage_ext.c)
- [storage.c](file://applications/services/storage/storage.c)

### Permission Management
The storage service implements a path-based permission system to control access to different storage areas.

```mermaid
flowchart TD
Start([Access Request]) --> Resolve["Resolve Path Aliases"]
Resolve --> Validate["Validate Path Prefix"]
Validate --> |Internal| CheckInternal["Check /int/ access"]
Validate --> |External| CheckExternal["Check /ext/ access"]
Validate --> |Config| CheckConfig["Check /cfg/ access"]
Validate --> |Data| CheckData["Check /data/ access"]
Validate --> |Assets| CheckAssets["Check /assets/ access"]
CheckInternal --> |System Only| RestrictInternal["Restrict to System Apps"]
CheckExternal --> |User Access| AllowExternal["Allow User Modifications"]
CheckConfig --> |Protected| ProtectConfig["Protect Configuration Files"]
CheckData --> |App Specific| IsolateData["Isolate App Data"]
CheckAssets --> |Read Only| ReadOnlyAssets["Prevent Modifications"]
RestrictInternal --> Decision["Access Decision"]
AllowExternal --> Decision
ProtectConfig --> Decision
IsolateData --> Decision
ReadOnlyAssets --> Decision
Decision --> End([Grant/Deny Access])
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)
- [storage.c](file://applications/services/storage/storage.c)

## System Context and Data Flow

### Storage Stack Architecture
The complete storage stack architecture shows all layers from client applications to hardware.

```mermaid
graph TD
subgraph "Application Layer"
A[Client Applications]
end
subgraph "Service Layer"
B[Storage API]
C[Message Queue]
D[Request Processor]
end
subgraph "Abstraction Layer"
E[Storage Glue]
F[Path Resolver]
G[Error Translator]
end
subgraph "Driver Layer"
H[Internal Driver<br/>LittleFS]
I[External Driver<br/>FatFs]
J[Virtual Driver<br/>FatFs]
end
subgraph "Hardware Layer"
K[Internal Flash<br/>Memory Mapped]
L[SD Card<br/>SPI Interface]
M[VFS Image<br/>File-based]
end
A --> B
B --> C
C --> D
D --> E
E --> F
E --> G
F --> H
F --> I
F --> J
G --> H
G --> I
G --> J
H --> K
I --> L
J --> M
style B fill:#f9f,stroke:#333
style E fill:#ff9,stroke:#333
style H fill:#9ff,stroke:#333
style I fill:#9ff,stroke:#333
```

**Diagram sources**
- [storage.h](file://applications/services/storage/storage.h)
- [storage.c](file://applications/services/storage/storage.c)
- [storage_glue.c](file://applications/services/storage/storage_glue.c)
- [storages/storage_int.c](file://applications/services/storage/storages/storage_int.c)
- [storages/storage_ext.c](file://applications/services/storage/storages/storage_ext.c)

### Data Flow Patterns
The data flow patterns illustrate how data moves through the storage system during common operations.

```mermaid
sequenceDiagram
participant App as "Application"
participant API as "Storage API"
participant Queue as "Message Queue"
participant Processor as "Processor"
participant Glue as "Storage Glue"
participant Driver as "File System Driver"
participant Hardware as "Storage Hardware"
title File Read Operation Data Flow
App->>API : storage_file_open(path)
API->>Queue : Enqueue open request
Queue-->>Processor : Dequeue request
Processor->>Glue : Resolve path
Glue->>Driver : Open file
Driver->>Hardware : Read file metadata
Hardware-->>Driver : Return metadata
Driver-->>Glue : Return file handle
Glue-->>Processor : Return result
Processor->>API : Update result
API-->>App : Return file handle
App->>API : storage_file_read(buffer, size)
API->>Queue : Enqueue read request
Queue-->>Processor : Dequeue request
Processor->>Driver : Read data
Driver->>Hardware : Read sectors
Hardware-->>Driver : Return data
Driver-->>Processor : Return data
Processor->>API : Update buffer
API-->>App : Return bytes read
App->>API : storage_file_close()
API->>Queue : Enqueue close request
Queue-->>Processor : Dequeue request
Processor->>Driver : Close file
Driver-->>Processor : Confirm close
Processor->>API : Update status
API-->>App : Return success
```

**Diagram sources**
- [storage.c](file://applications/services/storage/storage.c)
- [storage_processing.c](file://applications/services/storage/storage_processing.c)
- [storage_glue.c](file://applications/services/storage/storage_glue.c)
- [storages/storage_int.c](file://applications/services/storage/storages/storage_int.c)
- [storages/storage_ext.c](file://applications/services/storage/storages/storage_ext.c)