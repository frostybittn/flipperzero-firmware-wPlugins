# Hardware Abstraction Layer

<cite>
**Referenced Files in This Document**   
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c)
- [furi_hal_serial.h](file://targets/f7/furi_hal/furi_hal_serial.h)
- [furi_hal_pwm.h](file://targets/f7/furi_hal/furi_hal_pwm.h)
- [furi_hal_infrared.c](file://targets/f7/furi_hal/furi_hal_infrared.c)
- [furi_hal_nfc.c](file://targets/f7/furi_hal/furi_hal_nfc.c)
- [furi_hal_power.c](file://targets/f7/furi_hal/furi_hal_power.c)
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
The Hardware Abstraction Layer (HAL) in the Flipper Zero firmware provides a unified interface between the application layer and the underlying hardware peripherals. This documentation details the design principles, component interactions, and interface contracts that enable consistent hardware access across different hardware revisions. The HAL abstracts low-level register manipulation, providing developers with a simplified API for interacting with GPIO, I2C, SPI, UART, and other peripherals. The layer ensures thread safety, manages resource contention, and integrates with the system's power management framework.

## Project Structure
The HAL implementation is organized within the `targets/f7/furi_hal` directory, with interface headers located in `targets/furi_hal_include`. This separation allows for target-specific implementations while maintaining a consistent API surface. The structure follows a modular pattern where each peripheral has dedicated source files for implementation and headers for public APIs.

```mermaid
graph TD
subgraph "HAL Interface Headers"
A[furi_hal.h]
B[furi_hal_gpio.h]
C[furi_hal_i2c.h]
D[furi_hal_spi.h]
E[furi_hal_serial.h]
F[furi_hal_pwm.h]
G[furi_hal_infrared.h]
H[furi_hal_nfc.h]
I[furi_hal_power.h]
end
subgraph "HAL Implementation"
J[furi_hal_gpio.c]
K[furi_hal_i2c.c]
L[furi_hal_spi.c]
M[furi_hal_serial.c]
N[furi_hal_pwm.c]
O[furi_hal_infrared.c]
P[furi_hal_nfc.c]
Q[furi_hal_power.c]
end
A --> J
B --> J
C --> K
D --> L
E --> M
F --> N
G --> O
H --> P
I --> Q
```

**Diagram sources**
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c)
- [furi_hal_serial.h](file://targets/f7/furi_hal/furi_hal_serial.h)
- [furi_hal_pwm.h](file://targets/f7/furi_hal/furi_hal_pwm.h)
- [furi_hal_infrared.c](file://targets/f7/furi_hal/furi_hal_infrared.c)
- [furi_hal_nfc.c](file://targets/f7/furi_hal/furi_hal_nfc.c)
- [furi_hal_power.c](file://targets/f7/furi_hal/furi_hal_power.c)

**Section sources**
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)

## Core Components
The HAL consists of several core components that manage different hardware peripherals. Each component follows a consistent design pattern with initialization, configuration, data transfer, and deinitialization functions. The components are designed to be thread-safe and handle resource contention through mutexes and semaphores. The HAL also integrates with the system's power management to ensure optimal power consumption during peripheral operation.

**Section sources**
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)

## Architecture Overview
The HAL architecture is built around a bus-based model where each peripheral bus (I2C, SPI) has a dedicated handle system for resource management. The architecture ensures that only one application can access a bus at a time, preventing conflicts. The HAL uses callback mechanisms for asynchronous operations and integrates with the FreeRTOS kernel for task synchronization.

```mermaid
graph TB
subgraph "Application Layer"
App1[Application 1]
App2[Application 2]
App3[Application 3]
end
subgraph "HAL Layer"
GPIO[GPIO HAL]
I2C[I2C HAL]
SPI[SPI HAL]
UART[UART HAL]
PWM[PWM HAL]
IR[Infrared HAL]
NFC[NFC HAL]
Power[Power Management]
end
subgraph "Hardware Layer"
HW1[GPIO Pins]
HW2[I2C Bus]
HW3[SPI Bus]
HW4[UART Interface]
HW5[PWM Outputs]
HW6[IR Transceiver]
HW7[NFC Controller]
HW8[Power System]
end
App1 --> |furi_hal_gpio_init| GPIO
App2 --> |furi_hal_i2c_acquire| I2C
App3 --> |furi_hal_spi_bus_init| SPI
App1 --> |furi_hal_pwm_start| PWM
App2 --> |furi_hal_infrared_async_rx_start| IR
App3 --> |furi_hal_nfc_init| NFC
App1 --> |furi_hal_power_insomnia_enter| Power
GPIO --> HW1
I2C --> HW2
SPI --> HW3
UART --> HW4
PWM --> HW5
IR --> HW6
NFC --> HW7
Power --> HW8
```

**Diagram sources**
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c)
- [furi_hal_serial.h](file://targets/f7/furi_hal/furi_hal_serial.h)
- [furi_hal_pwm.h](file://targets/f7/furi_hal/furi_hal_pwm.h)
- [furi_hal_infrared.c](file://targets/f7/furi_hal/furi_hal_infrared.c)
- [furi_hal_nfc.c](file://targets/f7/furi_hal/furi_hal_nfc.c)
- [furi_hal_power.c](file://targets/f7/furi_hal/furi_hal_power.c)

## Detailed Component Analysis

### GPIO HAL Analysis
The GPIO HAL provides functions for configuring and controlling general-purpose input/output pins. It supports various modes including input, output, alternate functions, and interrupt handling. The implementation uses STM32's LL (Low Layer) drivers for direct register access while providing a simplified API.

```mermaid
classDiagram
class GpioMode {
+GpioModeInput
+GpioModeOutputPushPull
+GpioModeOutputOpenDrain
+GpioModeAltFunctionPushPull
+GpioModeAltFunctionOpenDrain
+GpioModeAnalog
+GpioModeInterruptRise
+GpioModeInterruptFall
+GpioModeInterruptRiseFall
}
class GpioPull {
+GpioPullNo
+GpioPullUp
+GpioPullDown
}
class GpioSpeed {
+GpioSpeedLow
+GpioSpeedMedium
+GpioSpeedHigh
+GpioSpeedVeryHigh
}
class GpioPin {
+port GPIO_TypeDef*
+pin uint16_t
}
class GpioInterrupt {
+callback GpioExtiCallback
+context void*
}
class furi_hal_gpio {
+furi_hal_gpio_init_simple()
+furi_hal_gpio_init()
+furi_hal_gpio_init_ex()
}
GpioPin --> furi_hal_gpio : "used by"
GpioMode --> furi_hal_gpio : "parameter"
GpioPull --> furi_hal_gpio : "parameter"
GpioSpeed --> furi_hal_gpio : "parameter"
GpioInterrupt --> furi_hal_gpio : "used for interrupts"
```

**Diagram sources**
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h#L50-L150)

**Section sources**
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)

### I2C HAL Analysis
The I2C HAL implements a robust interface for I2C communication with proper bus arbitration and error handling. It uses a handle-based system to manage bus access, ensuring that only one application can use the bus at a time. The implementation includes timeout mechanisms and supports both 7-bit and 10-bit addressing.

```mermaid
sequenceDiagram
participant Application
participant I2CHAL
participant I2CBus
participant PowerManager
Application->>I2CHAL : furi_hal_i2c_acquire(handle)
I2CHAL->>PowerManager : furi_hal_power_insomnia_enter()
I2CHAL->>I2CBus : Lock bus access
I2CHAL->>I2CBus : Activate bus
I2CHAL-->>Application : Bus acquired
Application->>I2CHAL : furi_hal_i2c_tx_rx()
I2CHAL->>I2CBus : Perform I2C transaction
I2CBus-->>I2CHAL : Transaction complete
I2CHAL-->>Application : Operation result
Application->>I2CHAL : furi_hal_i2c_release(handle)
I2CHAL->>I2CBus : Deactivate bus
I2CHAL->>I2CBus : Unlock bus
I2CHAL->>PowerManager : furi_hal_power_insomnia_exit()
I2CHAL-->>Application : Bus released
```

**Diagram sources**
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c#L50-L200)

**Section sources**
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)

### SPI HAL Analysis
The SPI HAL provides both blocking and DMA-based data transfer methods. It implements a handle system similar to I2C for bus arbitration and includes functions for full-duplex and half-duplex communication. The DMA implementation uses semaphores for synchronization between the application and interrupt service routines.

```mermaid
flowchart TD
Start([SPI Operation]) --> Acquire["furi_hal_spi_acquire(handle)"]
Acquire --> Lock["Lock SPI bus"]
Lock --> Power["furi_hal_power_insomnia_enter()"]
Power --> Configure["Configure SPI peripheral"]
Configure --> Transfer["Data Transfer Loop"]
Transfer --> CheckTX["LL_SPI_IsActiveFlag_TXE()"]
CheckTX --> |Ready| Transmit["LL_SPI_TransmitData8()"]
Transmit --> CheckRX["LL_SPI_IsActiveFlag_RXNE()"]
CheckRX --> |Data Ready| Receive["LL_SPI_ReceiveData8()"]
Receive --> Update["Update buffer pointers"]
Update --> MoreData{"More data?"}
MoreData --> |Yes| Transfer
MoreData --> |No| Complete["Transfer Complete"]
Complete --> Cleanup["furi_hal_spi_bus_end_txrx()"]
Cleanup --> Release["furi_hal_spi_release(handle)"]
Release --> Unlock["Unlock SPI bus"]
Unlock --> PowerExit["furi_hal_power_insomnia_exit()"]
PowerExit --> End([Operation Complete])
```

**Diagram sources**
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c#L50-L200)

**Section sources**
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c)

### UART/Serial HAL Analysis
The serial HAL implements UART functionality with both synchronous and asynchronous operation modes. It supports baud rate configuration, data transmission, and interrupt-driven reception. The asynchronous API uses callback functions that execute in interrupt context, requiring careful thread safety considerations.

```mermaid
classDiagram
class FuriHalSerialHandle {
+uart LPUART1_TypeDef*
+tx_gpio GpioPin*
+rx_gpio GpioPin*
+tx_dma_channel uint32_t
+rx_dma_channel uint32_t
+callback_context void*
+rx_callback FuriHalSerialAsyncRxCallback
+report_errors bool
+state FuriHalSerialState
}
class FuriHalSerialRxEvent {
+FuriHalSerialRxEventData
+FuriHalSerialRxEventIdle
+FuriHalSerialRxEventFrameError
+FuriHalSerialRxEventNoiseError
+FuriHalSerialRxEventOverrunError
}
class FuriHalSerialAsyncRxCallback {
<<function>>
}
class furi_hal_serial {
+furi_hal_serial_init()
+furi_hal_serial_deinit()
+furi_hal_serial_tx()
+furi_hal_serial_async_rx_start()
+furi_hal_serial_async_rx_stop()
}
FuriHalSerialHandle --> furi_hal_serial : "parameter"
FuriHalSerialRxEvent --> FuriHalSerialAsyncRxCallback : "parameter"
FuriHalSerialAsyncRxCallback --> furi_hal_serial : "callback"
```

**Diagram sources**
- [furi_hal_serial.h](file://targets/f7/furi_hal/furi_hal_serial.h#L50-L150)

**Section sources**
- [furi_hal_serial.h](file://targets/f7/furi_hal/furi_hal_serial.h)

### PWM HAL Analysis
The PWM HAL provides pulse width modulation functionality for generating signals with specific frequency and duty cycle. It supports multiple output channels and allows dynamic parameter adjustment during operation. The implementation uses STM32's timer peripherals with complementary output configurations.

```mermaid
classDiagram
class FuriHalPwmOutputId {
+FuriHalPwmOutputIdTim1PA7
+FuriHalPwmOutputIdLptim2PA4
}
class furi_hal_pwm {
+furi_hal_pwm_start()
+furi_hal_pwm_stop()
+furi_hal_pwm_set_params()
+furi_hal_pwm_is_running()
}
FuriHalPwmOutputId --> furi_hal_pwm : "parameter"
```

**Diagram sources**
- [furi_hal_pwm.h](file://targets/f7/furi_hal/furi_hal_pwm.h#L10-L50)

**Section sources**
- [furi_hal_pwm.h](file://targets/f7/furi_hal/furi_hal_pwm.h)

### Infrared HAL Analysis
The infrared HAL implements both transmission and reception of infrared signals using timer-based PWM generation and input capture. The transmission system uses DMA for efficient waveform generation, while reception uses timer input capture with interrupt handling for precise timing measurement.

```mermaid
sequenceDiagram
participant Application
participant IRHAL
participant Timer
participant DMA
participant GPIO
Application->>IRHAL : furi_hal_infrared_async_rx_start()
IRHAL->>GPIO : Configure as alternate function
IRHAL->>Timer : Initialize input capture
IRHAL->>Timer : Configure rising/falling edge detection
IRHAL->>IRHAL : Set interrupt handler
IRHAL->>Timer : Enable counter
IRHAL-->>Application : RX started
loop Signal Reception
Timer->>IRHAL : Interrupt on edge detection
IRHAL->>Timer : Read capture value
IRHAL->>Application : Callback with duration
end
Application->>IRHAL : furi_hal_infrared_async_rx_stop()
IRHAL->>Timer : Disable interrupts
IRHAL->>Timer : Stop counter
IRHAL->>GPIO : Reconfigure pin
IRHAL-->>Application : RX stopped
```

**Diagram sources**
- [furi_hal_infrared.c](file://targets/f7/furi_hal/furi_hal_infrared.c#L200-L400)

**Section sources**
- [furi_hal_infrared.c](file://targets/f7/furi_hal/furi_hal_infrared.c)

### NFC HAL Analysis
The NFC HAL provides an interface to the ST25R3916 NFC controller via SPI. It implements initialization, error checking, and technology-specific protocols for ISO14443A/B, ISO15693, and Felica. The implementation includes oscillator startup verification and voltage measurement for reliable operation.

```mermaid
flowchart TD
Start([NFC Init]) --> Acquire["furi_hal_nfc_acquire()"]
Acquire --> CheckChip["Read chip ID"]
CheckChip --> Valid{"Chip ID valid?"}
Valid --> |No| Error["Return error"]
Valid --> |Yes| Oscillator["Start oscillator"]
Oscillator --> OscReady{"Oscillator ready?"}
OscReady --> |No| Error
OscReady --> |Yes| Configure["Configure NFC controller"]
Configure --> Voltage["Measure VDD"]
Voltage --> SetIO["Set IO voltage level"]
SetIO --> FinalConfig["Final register configuration"]
FinalConfig --> IRQ["Enable interrupts"]
IRQ --> Release["furi_hal_nfc_release()"]
Release --> End([Init complete])
Error --> ReleaseError["furi_hal_nfc_release()"]
ReleaseError --> EndError([Init failed])
```

**Diagram sources**
- [furi_hal_nfc.c](file://targets/f7/furi_hal/furi_hal_nfc.c#L50-L200)

**Section sources**
- [furi_hal_nfc.c](file://targets/f7/furi_hal/furi_hal_nfc.c)

### Power Management Integration
The HAL integrates with the system's power management to control power states during peripheral operation. Functions like `furi_hal_power_insomnia_enter()` and `furi_hal_power_insomnia_exit()` prevent the system from entering low-power modes during active communication.

```mermaid
sequenceDiagram
participant Application
participant HAL
participant PowerManager
Application->>HAL : Acquire peripheral
HAL->>PowerManager : furi_hal_power_insomnia_enter()
PowerManager-->>HAL : Power lock acquired
HAL-->>Application : Peripheral acquired
Application->>HAL : Perform operations
HAL->>HAL : Maintain power lock
Application->>HAL : Release peripheral
HAL->>PowerManager : furi_hal_power_insomnia_exit()
PowerManager-->>HAL : Power lock released
HAL-->>Application : Peripheral released
```

**Diagram sources**
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c#L80-L100)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c#L120-L140)
- [furi_hal_power.c](file://targets/f7/furi_hal/furi_hal_power.c)

**Section sources**
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c)
- [furi_hal_power.c](file://targets/f7/furi_hal/furi_hal_power.c)

## Dependency Analysis
The HAL components have a hierarchical dependency structure where lower-level components depend on basic system services. The dependency graph shows how peripherals build upon core functionality while maintaining loose coupling.

```mermaid
graph TD
furi_hal_init --> furi_hal_clock
furi_hal_init --> furi_hal_os
furi_hal_init --> furi_hal_interrupt
furi_hal_init --> furi_hal_dma
furi_hal_gpio --> furi_hal_cortex
furi_hal_gpio --> stm32wbxx_ll_gpio
furi_hal_i2c --> furi_hal_power
furi_hal_i2c --> furi_hal_cortex
furi_hal_i2c --> stm32wbxx_ll_i2c
furi_hal_i2c --> furi_hal_gpio
furi_hal_spi --> furi_hal_power
furi_hal_spi --> furi_hal_interrupt
furi_hal_spi --> stm32wbxx_ll_spi
furi_hal_spi --> stm32wbxx_ll_dma
furi_hal_spi --> furi_hal_gpio
furi_hal_serial --> furi_hal_power
furi_hal_serial --> furi_hal_interrupt
furi_hal_serial --> stm32wbxx_ll_usart
furi_hal_serial --> furi_hal_gpio
furi_hal_pwm --> stm32wbxx_ll_tim
furi_hal_pwm --> furi_hal_gpio
furi_hal_infrared --> furi_hal_interrupt
furi_hal_infrared --> furi_hal_dma
furi_hal_infrared --> furi_hal_bus
furi_hal_infrared --> stm32wbxx_ll_tim
furi_hal_infrared --> stm32wbxx_ll_dma
furi_hal_nfc --> furi_hal_spi
furi_hal_nfc --> furi_hal_gpio
furi_hal_nfc --> st25r3916
```

**Diagram sources**
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c)
- [furi_hal_serial.h](file://targets/f7/furi_hal/furi_hal_serial.h)
- [furi_hal_pwm.h](file://targets/f7/furi_hal/furi_hal_pwm.h)
- [furi_hal_infrared.c](file://targets/f7/furi_hal/furi_hal_infrared.c)
- [furi_hal_nfc.c](file://targets/f7/furi_hal/furi_hal_nfc.c)
- [furi_hal_power.c](file://targets/f7/furi_hal/furi_hal_power.c)

**Section sources**
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h)
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c)
- [furi_hal_serial.h](file://targets/f7/furi_hal/furi_hal_serial.h)
- [furi_hal_pwm.h](file://targets/f7/furi_hal/furi_hal_pwm.h)
- [furi_hal_infrared.c](file://targets/f7/furi_hal/furi_hal_infrared.c)
- [furi_hal_nfc.c](file://targets/f7/furi_hal/furi_hal_nfc.c)
- [furi_hal_power.c](file://targets/f7/furi_hal/furi_hal_power.c)

## Performance Considerations
The HAL is designed with performance in mind, using direct register access through STM32's LL drivers to minimize overhead. DMA is utilized for high-throughput peripherals like SPI and infrared to reduce CPU load. The handle-based bus arbitration system ensures efficient resource sharing between applications. Power management integration prevents unnecessary wakeups while maintaining reliable communication. The use of semaphores and mutexes for synchronization adds minimal overhead due to the underlying FreeRTOS implementation.

## Troubleshooting Guide
Common issues with the HAL typically involve resource contention, improper initialization, or timing problems. When debugging, verify that peripherals are properly acquired and released, check for correct pin configuration, and ensure that power management functions are called appropriately. For I2C and SPI issues, verify bus speeds and signal integrity. For timer-based peripherals like PWM and infrared, ensure that the correct timer channels are configured. The HAL includes assertion checks using `furi_check()` that will halt execution if invalid parameters are detected, aiding in early error detection.

**Section sources**
- [furi_hal.h](file://targets/furi_hal_include/furi_hal.h#L50-L70)
- [furi_hal_gpio.h](file://targets/f7/furi_hal/furi_hal_gpio.h#L100-L120)
- [furi_hal_i2c.c](file://targets/f7/furi_hal/furi_hal_i2c.c#L150-L180)
- [furi_hal_spi.c](file://targets/f7/furi_hal/furi_hal_spi.c#L100-L130)

## Conclusion
The Hardware Abstraction Layer in the Flipper Zero firmware provides a robust, efficient, and easy-to-use interface for hardware peripherals. By abstracting low-level register manipulation and providing consistent APIs across different hardware revisions, the HAL enables application developers to focus on functionality rather than hardware specifics. The layer's design emphasizes thread safety, resource management, and power efficiency, making it suitable for embedded applications with real-time requirements. The modular structure allows for easy extension and maintenance, ensuring the firmware can adapt to future hardware changes.