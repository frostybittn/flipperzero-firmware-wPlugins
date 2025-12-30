# MIFARE DESFire

<cite>
**Referenced Files in This Document**   
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h)
- [mf_desfire.c](file://lib/nfc/protocols/mf_desfire/mf_desfire.c)
- [mf_desfire_i.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.h)
- [mf_desfire_i.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.c)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h)
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c)
- [nfc_scene_mf_desfire_app.c](file://applications/main/nfc/scenes/nfc_scene_mf_desfire_app.c)
- [mf_desfire_render.c](file://applications/main/nfc/helpers/protocol_support/mf_desfire/mf_desfire_render.c)
- [iso14443_4a.h](file://lib/nfc/protocols/iso14443_4a/iso14443_4a.h)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Protocol Architecture](#protocol-architecture)
3. [Hierarchical File System](#hierarchical-file-system)
4. [Security Features](#security-features)
5. [Authentication Process](#authentication-process)
6. [Cryptographic Algorithms](#cryptographic-algorithms)
7. [Secure Messaging and MAC Verification](#secure-messaging-and-mac-verification)
8. [Application Management](#application-management)
9. [Use Cases](#use-cases)
10. [Advantages Over Other MIFARE Variants](#advantages-over-other-mifare-variants)

## Introduction
The MIFARE DESFire protocol implementation in the Flipper Zero provides advanced security features for contactless smart card applications. This document details the implementation of ISO/IEC 14443-4 compliant communication, support for multiple cryptographic algorithms including DES, 3DES, and AES, and secure messaging with Message Authentication Code (MAC) verification. The Flipper Zero's implementation enables users to interact with MIFARE DESFire cards used in high-security environments such as electronic passports and banking systems.

## Protocol Architecture
The MIFARE DESFire implementation in Flipper Zero follows a layered architecture that ensures compliance with ISO/IEC 14443-4 standards while providing a robust interface for application development.

```mermaid
graph TB
subgraph "Flipper Zero Application Layer"
A[NFC Application] --> B[NFC Protocol Support]
end
subgraph "NFC Protocol Layer"
B --> C[MIFARE DESFire Protocol]
C --> D[ISO/IEC 14443-4A]
D --> E[ISO/IEC 14443-3A]
end
subgraph "Hardware Layer"
E --> F[NFC Reader IC]
end
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L3-225)
- [iso14443_4a.h](file://lib/nfc/protocols/iso14443_4a/iso14443_4a.h#L1-42)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L3-225)
- [mf_desfire.c](file://lib/nfc/protocols/mf_desfire/mf_desfire.c#L1-420)

## Hierarchical File System
MIFARE DESFire cards implement a hierarchical file system structure consisting of applications, files, and diversified keys. This structure provides a secure and organized way to store and manage data.

### Application Structure
The application structure in MIFARE DESFire is defined by the `MfDesfireApplication` struct, which contains key settings, key versions, file IDs, file settings, and file data.

```mermaid
classDiagram
class MfDesfireApplication {
+MfDesfireKeySettings key_settings
+SimpleArray* key_versions
+SimpleArray* file_ids
+SimpleArray* file_settings
+SimpleArray* file_data
}
class MfDesfireFileSettings {
+MfDesfireFileType type
+MfDesfireFileCommunicationSettings comm
+MfDesfireFileAccessRights access_rights[14]
+uint8_t access_rights_len
+union { data, value, record, transaction_mac }
}
class MfDesfireFileData {
+SimpleArray* data
}
class MfDesfireApplicationId {
+uint8_t data[3]
}
MfDesfireApplication --> MfDesfireFileSettings : "contains"
MfDesfireApplication --> MfDesfireFileData : "contains"
MfDesfireApplication --> MfDesfireApplicationId : "identified by"
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L151-161)
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L119-145)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L119-161)
- [mf_desfire_i.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.c#L327-332)

### File Types
MIFARE DESFire supports multiple file types, each designed for specific use cases:

```mermaid
graph TD
A[File Types] --> B[Standard Data File]
A --> C[Backup Data File]
A --> D[Value File]
A --> E[Linear Record File]
A --> F[Cyclic Record File]
A --> G[Transaction MAC File]
B --> H[General data storage]
C --> I[Redundant data storage]
D --> J[Monetary value storage]
E --> K[Sequential record storage]
F --> L[Rotating record storage]
G --> M[Transaction security]
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L101-108)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L101-108)

## Security Features
The MIFARE DESFire implementation in Flipper Zero incorporates multiple advanced security features to protect against various attack vectors.

### Communication Settings
Files can be configured with different communication settings to control the level of security:

```mermaid
stateDiagram-v2
[*] --> Plaintext
[*] --> Authenticated
[*] --> Enciphered
Plaintext --> Authenticated : Authentication required
Authenticated --> Enciphered : Encryption enabled
Enciphered --> Authenticated : Decryption
Authenticated --> Plaintext : No authentication
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L110-114)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L110-114)

### Key Management
The key management system supports up to 14 keys per application with version tracking:

```mermaid
erDiagram
APPLICATION ||--o{ KEY_VERSION : contains
APPLICATION ||--o{ ACCESS_RIGHTS : controls
KEY_VERSION ||--o{ CRYPTO_ALGORITHM : uses
APPLICATION {
string app_id PK
uint8_t max_keys
bool master_key_changeable
}
KEY_VERSION {
uint8_t key_number PK
uint8_t version
uint8_t algorithm
}
ACCESS_RIGHTS {
uint8_t file_id PK
uint16_t rights
}
CRYPTO_ALGORITHM {
string name PK
uint8_t key_length
}
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L89-97)
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L99)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L89-99)
- [mf_desfire_i.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.c#L84-92)

## Authentication Process
The authentication process in MIFARE DESFire involves establishing a secure channel through a challenge-response mechanism using diversified keys.

```mermaid
sequenceDiagram
participant Flipper as Flipper Zero
participant Card as MIFARE DESFire Card
Flipper->>Card : SELECT APPLICATION
Card-->>Flipper : ACK
Flipper->>Card : AUTHENTICATE (key number)
Card-->>Flipper : Challenge (RndB)
Flipper->>Flipper : Generate RndA, calculate response
Flipper->>Card : Response (RndA encrypted)
Card-->>Flipper : Challenge (RndB' encrypted)
Flipper->>Flipper : Verify response, calculate final response
Flipper->>Card : Final Response (RndB' encrypted)
Card-->>Flipper : Authentication Success
```

**Diagram sources**
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)
- [mf_desfire.c](file://lib/nfc/protocols/mf_desfire/mf_desfire.c#L63-83)

**Section sources**
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)
- [mf_desfire.c](file://lib/nfc/protocols/mf_desfire/mf_desfire.c#L63-83)

## Cryptographic Algorithms
The Flipper Zero's MIFARE DESFire implementation supports multiple cryptographic algorithms for secure communication.

### Supported Algorithms
The system supports the following cryptographic algorithms:

| Algorithm | Key Length | Security Level | Use Case |
|---------|-----------|--------------|---------|
| DES | 56 bits | Basic | Legacy systems |
| 3DES | 112/168 bits | High | Secure transactions |
| AES | 128/192/256 bits | Very High | High-security applications |

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L110-114)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L1-290)

### Algorithm Selection
The cryptographic algorithm is determined by the key settings and file communication settings:

```mermaid
flowchart TD
A[Start] --> B{Key Settings}
B --> C[DES Key]
B --> D[3DES Key]
B --> E[AES Key]
C --> F[Use DES Algorithm]
D --> G[Use 3DES Algorithm]
E --> H[Use AES Algorithm]
F --> I[Set Communication Settings]
G --> I
H --> I
I --> J{Communication Type}
J --> K[Plaintext]
J --> L[Authenticated]
J --> M[Enciphered]
K --> N[No Encryption]
L --> O[MAC Verification]
M --> P[Full Encryption]
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L110-114)
- [mf_desfire_i.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.c#L84-92)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L110-114)
- [mf_desfire_i.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.c#L84-92)

## Secure Messaging and MAC Verification
Secure messaging in MIFARE DESFire ensures data integrity and authenticity through Message Authentication Code (MAC) verification.

### MAC Verification Process
The MAC verification process ensures that data has not been tampered with during transmission:

```mermaid
sequenceDiagram
participant Reader as Flipper Zero
participant Card as MIFARE DESFire Card
Reader->>Card : Command with MAC
Card->>Card : Verify MAC using session key
alt MAC Valid
Card->>Card : Process command
Card->>Reader : Response with MAC
else MAC Invalid
Card->>Reader : Error Response
end
Reader->>Reader : Verify response MAC
alt MAC Valid
Reader->>User : Display data
else MAC Invalid
Reader->>User : Show error
end
```

**Diagram sources**
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)
- [mf_desfire_i.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.c#L1-800)

**Section sources**
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)
- [mf_desfire_i.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_i.c#L1-800)

### Secure Channel Establishment
The secure channel establishment process creates a protected communication path:

```mermaid
flowchart TD
A[Select Application] --> B[Authenticate with Key]
B --> C{Authentication Success?}
C --> |Yes| D[Establish Session Key]
C --> |No| E[Authentication Failed]
D --> F[Set Communication Settings]
F --> G{Plaintext?}
G --> |Yes| H[Normal Communication]
G --> |No| I{Authenticated?}
I --> |Yes| J[MAC Verification]
I --> |No| K[Full Encryption]
H --> L[Data Exchange]
J --> L
K --> L
L --> M[End Session]
```

**Diagram sources**
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)
- [mf_desfire.c](file://lib/nfc/protocols/mf_desfire/mf_desfire.c#L63-83)

**Section sources**
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)
- [mf_desfire.c](file://lib/nfc/protocols/mf_desfire/mf_desfire.c#L63-83)

## Application Management
The Flipper Zero provides comprehensive tools for managing MIFARE DESFire applications, including creating applications, reading transaction logs, and managing access rights.

### Application Creation
Creating a new application on a MIFARE DESFire card:

```mermaid
sequenceDiagram
participant Flipper as Flipper Zero
participant Card as MIFARE DESFire Card
Flipper->>Card : CREATE APPLICATION
Card->>Card : Validate permissions
alt Permissions OK
Card->>Card : Allocate memory
Card->>Card : Initialize application structure
Card->>Flipper : Success
Flipper->>Flipper : Store application data
else Permissions Denied
Card->>Flipper : Error Response
end
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L20-26)
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L20-26)
- [mf_desfire_poller.c](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.c#L1-272)

### Reading Transaction Logs
Accessing transaction logs from a MIFARE DESFire card:

```mermaid
sequenceDiagram
participant Flipper as Flipper Zero
participant Card as MIFARE DESFire Card
Flipper->>Card : SELECT APPLICATION
Card-->>Flipper : ACK
Flipper->>Card : AUTHENTICATE (log access key)
Card-->>Flipper : Challenge
Flipper->>Flipper : Calculate response
Flipper->>Card : Response
Card-->>Flipper : Authentication Success
Flipper->>Card : READ RECORDS (transaction file)
Card-->>Flipper : Transaction data with MAC
Flipper->>Flipper : Verify MAC, parse records
Flipper->>User : Display transaction log
```

**Diagram sources**
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L236-242)
- [nfc_scene_mf_desfire_app.c](file://applications/main/nfc/scenes/nfc_scene_mf_desfire_app.c#L1-110)

**Section sources**
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L236-242)
- [nfc_scene_mf_desfire_app.c](file://applications/main/nfc/scenes/nfc_scene_mf_desfire_app.c#L1-110)

### Access Rights Management
Managing access rights for files within an application:

```mermaid
flowchart TD
A[Select Application] --> B[Authenticate with Change Key]
B --> C{Authentication Success?}
C --> |Yes| D[READ KEY SETTINGS]
C --> |No| E[Access Denied]
D --> F{Key Changeable?}
F --> |Yes| G[Modify Access Rights]
F --> |No| H[Cannot Modify]
G --> I[WRITE KEY SETTINGS]
I --> J{Write Success?}
J --> |Yes| K[Access Rights Updated]
J --> |No| L[Write Failed]
```

**Diagram sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L89-97)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L91-93)

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L89-97)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L91-93)

## Use Cases
The MIFARE DESFire implementation in Flipper Zero has several high-security use cases.

### Electronic Passports
Electronic passports use MIFARE DESFire for secure storage of biometric data:

```mermaid
graph TD
A[Electronic Passport] --> B[Chip with DESFire]
B --> C[Store Biometric Data]
B --> D[Store Personal Information]
B --> E[Store Digital Signature]
F[Border Control] --> G[Read Passport Data]
G --> H[Verify Digital Signature]
H --> I{Signature Valid?}
I --> |Yes| J[Grant Entry]
I --> |No| K[Deny Entry]
```

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L1-225)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L1-290)

### Banking Applications
Banking applications use MIFARE DESFire for secure financial transactions:

```mermaid
graph TD
A[Bank Card] --> B[DESFire Chip]
B --> C[Store Account Information]
B --> D[Store Transaction History]
B --> E[Store Cryptographic Keys]
F[Payment Terminal] --> G[Authenticate Card]
G --> H[Verify Balance]
H --> I[Process Transaction]
I --> J[Update Transaction Log]
J --> K[Sign Transaction]
```

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L1-225)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L1-290)

## Advantages Over Other MIFARE Variants
The MIFARE DESFire protocol offers several advantages over other MIFARE variants like MIFARE Classic.

### Security Comparison
Comparing security features across MIFARE variants:

| Feature | MIFARE Classic | MIFARE DESFire | Advantage |
|-------|---------------|---------------|---------|
| Cryptographic Algorithm | Proprietary Crypto1 | DES, 3DES, AES | Stronger encryption |
| Key Management | Static keys | Diversified keys | Better key security |
| Authentication | One-way | Mutual authentication | Enhanced security |
| File System | Flat | Hierarchical | Better organization |
| Memory Protection | None | Access rights per file | Granular control |
| Transaction Security | None | MAC verification | Data integrity |

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L1-225)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L1-290)

### Performance Comparison
Performance characteristics of MIFARE variants:

| Characteristic | MIFARE Classic | MIFARE DESFire | Advantage |
|--------------|---------------|---------------|---------|
| Data Transfer Rate | 106 kbps | Up to 848 kbps | Faster communication |
| Memory Capacity | 716 bytes | Up to 32 KB | More storage |
| File Operations | Limited | Comprehensive | More functionality |
| Power Consumption | Low | Moderate | Trade-off for security |
| Transaction Speed | Fast | Moderate | Security vs. speed |

**Section sources**
- [mf_desfire.h](file://lib/nfc/protocols/mf_desfire/mf_desfire.h#L50-59)
- [mf_desfire_poller.h](file://lib/nfc/protocols/mf_desfire/mf_desfire_poller.h#L1-290)