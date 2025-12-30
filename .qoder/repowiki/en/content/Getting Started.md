# Getting Started

<cite>
**Referenced Files in This Document**   
- [ReadMe.md](file://ReadMe.md)
- [documentation/HowToBuild.md](file://documentation/HowToBuild.md)
- [documentation/HowToInstall.md](file://documentation/HowToInstall.md)
- [documentation/HardwareTargets.md](file://documentation/HardwareTargets.md)
- [documentation/fbt.md](file://documentation/fbt.md)
- [SConstruct](file://SConstruct)
- [scripts/toolchain/fbtenv.sh](file://scripts/toolchain/fbtenv.sh)
- [scripts/toolchain/windows-toolchain-download.ps1](file://scripts/toolchain/windows-toolchain-download.ps1)
- [fbt](file://fbt)
- [fbt.cmd](file://fbt.cmd)
- [targets/f7/target.json](file://targets/f7/target.json)
- [targets/f18/target.json](file://targets/f18/target.json)
- [documentation/devboard/Get started with the Dev Board.md](file://documentation/devboard/Get started with the Dev Board.md)
- [documentation/devboard/Debugging via the Devboard.md](file://documentation/devboard/Debugging via the Devboard.md)
- [scripts/debug/README.md](file://scripts/debug/README.md)
</cite>

## Table of Contents
1. [Hardware Requirements](#hardware-requirements)
2. [Development Environment Setup](#development-environment-setup)
3. [Building the Firmware](#building-the-firmware)
4. [Configuration Options](#configuration-options)
5. [Verification and Diagnostics](#verification-and-diagnostics)
6. [Troubleshooting](#troubleshooting)

## Hardware Requirements

The Flipper Zero firmware development environment supports multiple hardware variants, with the primary target being the standard Flipper Zero device (f7) and the development-only variant (f18). The f7 target corresponds to the production Flipper Zero hardware with full peripheral support, while the f18 target is a minimal configuration designed for development and testing purposes, excluding certain hardware modules like NFC, Sub-GHz, and infrared.

The target configuration is defined in JSON files within the `targets` directory, where `f7/target.json` specifies the complete set of hardware capabilities and dependencies for the standard device. Developers can build firmware for specific hardware variants by specifying the target during the build process. For debugging and development, the Wi-Fi Developer Board is recommended as it provides debugging capabilities through DAP Link or Black Magic probe modes, enabling firmware flashing, debugging, and log reading via USB or Wi-Fi connections.

**Section sources**
- [documentation/HardwareTargets.md](file://documentation/HardwareTargets.md#hardware_targets)
- [targets/f7/target.json](file://targets/f7/target.json)
- [targets/f18/target.json](file://targets/f18/target.json)

## Development Environment Setup

To set up the development environment for building Flipper Zero firmware, you need to install Git and ensure Python 3.8 or later is available on your system. The build system, called Flipper Build Tool (fbt), automatically manages the toolchain and dependencies. On Linux and macOS, run `source scripts/toolchain/fbtenv.sh` to configure the environment, while Windows users should execute `scripts/toolchain/fbtenv.cmd`. This script downloads and configures the ARM GCC toolchain (version 12.3) automatically, adding it to the PATH without affecting the global system environment.

The fbt tool also handles git submodule initialization, automatically running `git submodule update --init --recursive` on invocation unless `FBT_NO_SYNC=1` is set. For IDE integration, Visual Studio Code is supported through `./fbt vscode_dist`, which configures the workspace with recommended extensions and debugging settings. On Linux systems, udev rules must be installed to allow non-root access to USB devices by copying `scripts/debug/41-flipper.rules` to `/etc/udev/rules.d/` and reloading the rules.

**Section sources**
- [documentation/HowToBuild.md](file://documentation/HowToBuild.md)
- [documentation/fbt.md](file://documentation/fbt.md)
- [scripts/toolchain/fbtenv.sh](file://scripts/toolchain/fbtenv.sh)
- [scripts/toolchain/windows-toolchain-download.ps1](file://scripts/toolchain/windows-toolchain-download.ps1)
- [fbt](file://fbt)
- [fbt.cmd](file://fbt.cmd)
- [scripts/debug/README.md](file://scripts/debug/README.md)

## Building the Firmware

The firmware is built using the Flipper Build Tool (fbt) with SCons as the underlying build system. To build the complete firmware package, execute `./fbt updater_package` on Linux/macOS or `./fbt.cmd updater_package` on Windows. This command compiles the firmware, packages resources, and creates a distributable archive in the `dist/` directory. For release builds, use `./fbt COMPACT=1 DEBUG=0 updater_package` to produce an optimized version without debug symbols.

The build system supports multiple targets: `fw_dist` builds the firmware binary, `fap_dist` compiles external applications (.fap files), and `flash_usb_full` builds and installs the firmware via USB. Developers can also build specific application sets by setting `FIRMWARE_APP_SET`, such as `unit_tests` for testing firmware components. The build process automatically handles dependencies, ensuring all required components are compiled and linked correctly according to the target hardware configuration.

**Section sources**
- [documentation/HowToBuild.md](file://documentation/HowToBuild.md)
- [SConstruct](file://SConstruct)
- [documentation/fbt.md](file://documentation/fbt.md)

## Configuration Options

The build system offers several configuration options to customize the firmware build process. The primary configuration file `fbt_options.py` contains default settings, which can be overridden via command-line parameters or a local `fbt_options_local.py` file. Key configuration variables include `COMPACT` (enable size optimization), `DEBUG` (include debug symbols), and `TARGET_HW` (specify hardware target, where 7 is standard Flipper Zero and 18 is the minimal development variant).

Application inclusion can be customized using `FIRMWARE_APP_SET` to select predefined application sets, or by using `--extra-int-apps` and `--extra-ext-apps` to force specific applications into internal or external builds. Additional C compiler definitions can be passed with `--extra-define`. The environment can be customized with variables like `FBT_TOOLCHAIN_PATH` to specify the toolchain location, `FBT_VERBOSE` for detailed output, and `FBT_NOENV=1` to use system-installed tools instead of the bundled toolchain.

**Section sources**
- [documentation/fbt.md](file://documentation/fbt.md#configuration)
- [SConstruct](file://SConstruct)

## Verification and Diagnostics

After building the firmware, verification can be performed by checking the output in the `dist/` directory, where the generated `flipper-z-f7-update-{version}.tgz` file contains the complete update package. The build system generates a compilation database (`compile_commands.json`) in `build/latest/` for use with IDEs and static analysis tools. For runtime diagnostics, the Flipper CLI can be accessed via `./fbt cli`, providing a serial console to the device for debugging and testing.

When using the Wi-Fi Developer Board, debugging can be initiated with `./fbt debug`, which flashes the firmware and starts a GDB session with the ELF file loaded. The Developer Board supports both Black Magic and DAP Link debugging modes, accessible via USB or Wi-Fi. Log output can be monitored through the serial interface, and the device's debug mode can be verified in the Flipper Zero settings under System → Debug.

**Section sources**
- [documentation/devboard/Get started with the Dev Board.md](file://documentation/devboard/Get started with the Dev Board.md)
- [documentation/devboard/Debugging via the Devboard.md](file://documentation/devboard/Debugging via the Devboard.md)
- [SConstruct](file://SConstruct)

## Troubleshooting

Common issues during setup include toolchain download failures, USB permission errors, and build configuration problems. If the toolchain fails to download, verify internet connectivity and check that curl or wget is installed. On Linux, ensure your user is in the `dialout` group and udev rules are properly installed. If building fails due to submodule issues, run `git submodule update --init --recursive` manually.

For debugging connection problems with the Developer Board, ensure Debug Mode is enabled on the Flipper Zero (Settings → System → Debug → ON) and that the board is properly seated. When using VS Code, run `./fbt vscode_dist` to regenerate configuration files if debugging fails. If firmware flashing fails, try power-cycling the Flipper Zero with the Developer Board attached. Persistent issues may require cleaning the build with `./fbt -c` before rebuilding.

**Section sources**
- [documentation/HowToInstall.md](file://documentation/HowToInstall.md)
- [documentation/fbt.md](file://documentation/fbt.md)
- [scripts/debug/README.md](file://scripts/debug/README.md)
- [documentation/devboard/Get started with the Dev Board.md](file://documentation/devboard/Get started with the Dev Board.md)