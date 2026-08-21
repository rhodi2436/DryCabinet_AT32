# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Firmware for a **dehumidifying cabinet (防潮柜)** electronic label controller. MCU: **Artery AT32F403AVCT7** (ARM Cortex-M4, 240MHz). The device controls a TFT LCD display, reads an SHT30 temperature/humidity sensor, drives a dehumidification system (fan + PTC heater + mica heater), and communicates over RS485 with a Modbus-like protocol to receive electronic label picking tasks.

## Build System

- **IDE**: Keil MDK (uVision 5). Project file: `Project/T-S1_V30.uvprojx`
- **Compiler**: ARMCC (Keil's ARM Compiler). No GCC/Makefile build exists.
- The startup file is `Project/startup_at32f403avct7.s`
- Linker scripts are under `Libraries/CMSIS/CM4/DeviceSupport/startup/gcc/linker/` (GCC only; Keil uses scatter file `Project/Objects/T-ES1.sct`)

## Code Architecture

### Directory Layout

| Path | Purpose |
|------|---------|
| `Project/USER_APP/` | Main application logic (main, interrupt handlers, protocol, sensor, display, dehumidification) |
| `Project/USER_DRV/` | Low-level peripheral drivers (UART, I2C, SPI NOR Flash, LEDs, keys, EEPROM, buzzer, timers) |
| `Project/BSP/` | Board support package — GPIO pin definitions, board-level init |
| `Project/Head File/` | All headers. `includes.h` is the master include; `Glob.h` defines all global variables |
| `Project/QRcode/` | QR code generation library |
| `Libraries/AT32F4xx_StdPeriph_Driver/` | Artery standard peripheral library (GPIO, USART, TIM, I2C, XMC, etc.) |
| `Libraries/CMSIS/` | ARM CMSIS Core + Artery device support (`at32f4xx.h`, `system_at32f4xx.c`) |
| `Middlewares/` | FreeRTOS and USB stacks (not used by this project) |

### Global Variable Pattern

`Glob.h` uses a `GLOB_DEF` macro trick: `main.c` defines `#define GLOB_DEF` before including `includes.h`, which makes `CPU_EXT` expand to nothing (definition). All other `.c` files see `CPU_EXT` as `extern` (declaration). This centralizes all global state in one header.

### Core Application Files

- **`main.c`**: `Board_Init()` → `System_Init()` → `SHT30_sys_Init()` → main loop calling `LCD_Scan()` + `check_state()`
- **`Interrupt.c`**: Contains the 1ms TIM3 ISR (`TMR3_GLOBAL_IRQHandler`) which is the heartbeat of the system — samples SHT30 every 1s, runs `ChkHumiditySts()` state machine, scans keys, decrements timers, and triggers `Cmd_Process()` when UART RX completes. Also contains `USART1_IRQHandler` for RS485 RX.
- **`process.c`**: RS485 protocol handling. Parses broadcast/unicast Modbus-like commands for electronic label task management (display SKU codes, counts, directions, units). Functions: `Cmd_Process()`, `DiscodeProtocalV1()`, `Commu_Task_Process()`, `Commu_Info600U_Process()`.
- **`Movement.c`**: Hardware control for dehumidification — `fan_switch()`, `ptc_switch()`, `mica_switch()`, plus `check_state()` (called from main loop, updates display state icon) and `alarm_scan()`.
- **`SHT3x.c`**: SHT30 sensor driver via bit-banged I2C. Provides `SHT3X_ReadMeasurementBuffer()`, called from TIM3 ISR once per second. Applies calibration offsets from EEPROM config.
- **`TFT3.55.c`**: TFT LCD driver via XMC (FSMC) parallel interface. 240×320 resolution. Renders text, images, QR codes from NOR Flash font/image data.
- **`Init_sys.c`**: Older/alternate system init (appears unused; `main.c` has its own `System_Init()`).

### Dehumidification Control Flow

The active humidity control logic lives in `ChkHumiditySts()` inside `Interrupt.c` (the older `Dry_Module.c` version is largely commented out). It implements a state machine defined in `Movement.h`:

```
STATE_INIT → STATE_IDLE → STATE_FAST → STATE_DRY1/DRY2 → back to STATE_IDLE
```

- **STATE_INIT**: Runs at power-up (40 min heat → 20 min cooldown → 60 min fan only → IDLE)
- **STATE_IDLE**: All off, monitors humidity. Every 30s check if above setpoint+1 → FAST
- **STATE_FAST**: Fan only. Monitors for humidity rise over 5 minutes; if rising, transitions to DRY1 (>8% RH) or DRY2 (≤8% RH)
- **STATE_DRY1/DRY2**: Multi-phase drying cycle (mica preheat → PTC+mica heat → mica off/PTC on → all off → back to IDLE)

Key global: `float Cur_SHT30_Humidity` — the current displayed humidity value (may differ from raw sensor reading).

### RS485 Protocol

- Physical: UART1, 38400 baud, 9-bit word, 1 stop, no parity
- Protocol: Modbus-inspired with address byte + function codes (0x03 read, 0x06 write, 0x10 multi-write)
- Device address stored in EEPROM (configurable, range 1-247)
- CRC16 for packet integrity
- Supports broadcast (address 0), task dispatch (single/dual label picking), info display (600U mode)

### Configuration Storage

Parameters stored in AT24C02 EEPROM via bit-banged I2C:
- Device address, target humidity setpoint, humidity/temperature calibration offsets, language toggle, LED behavior, wait time, function key mode, display mode

## Pin/Peripheral Assignments

- **TFT LCD**: XMC Bank1 NE1 (0x60000000), 16-bit parallel, RS on XMC_A16
- **SPI NOR Flash**: SPIM interface (PA8=NSS, PA11=IO0, PA12=IO1, PB1=SCK, PB6=IO2, PB7=IO3) — stores font bitmaps and images
- **SHT30**: Bit-banged I2C on GPIO (defined in BSP header)
- **RS485**: USART1 (PA9=TX, PA10=RX), with GPIO direction control
- **LEDs**: 5 GPIOs (Red, Green, Blue, Warning/Orange, Door)
- **Dehumidification actuators**: FAN, PTC heater (DOOR), Mica heater (HOT) — each on dedicated GPIO

## Key Coding Conventions

- Chinese comments throughout (GB2312 encoding in source files)
- Custom integer typedefs in `Typedef.h`: `INT8U`, `INT16U`, `INT32U`, `INT8VU` (volatile variants), etc.
- `CPU_EXT` macro for global variable definition/declaration duality
- `ON`/`OFF` = 1/0, `TRUE`/`FALSE` = 1/0
- Timer values in milliseconds; the TIM3 ISR provides a 1ms tick
- Display pages defined as `PAGE_*` constants in `TFT3.55.h` and selected via `SetLcdLay()`
