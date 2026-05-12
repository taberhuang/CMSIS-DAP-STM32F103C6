# CMSIS-DAP Firmware for STM32F103C6 (BluePill)

[![Firmware CI](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

CMSIS-DAP (HID) + optional USB-CDC virtual COM port firmware for **STM32F103Cx**
based debug probes. The default target is **STM32F103C6** (low-density,
32 KB Flash / 10 KB RAM, ~71% Flash / ~62% RAM used).

This repository is a **second-generation fork** of
[RagingWaves/CMSIS-DAP_C6T6](https://github.com/RagingWaves/CMSIS-DAP_C6T6),
which targets a **BluePill STM32F103C6T6** CMSIS-DAP as an on-board debugger.

[RagingWaves/CMSIS-DAP_C6T6](https://github.com/RagingWaves/CMSIS-DAP_C6T6) is
based on [RadioOperator/STM32F103C8T6_CMSIS-DAP_SWO](https://github.com/RadioOperator/STM32F103C8T6_CMSIS-DAP_SWO)
and the [x893/CMSIS-DAP](https://github.com/x893/CMSIS-DAP) codebase. This
fork adds GCC + CMake + Ninja, GitHub Actions CI, and further pin / feature
changes while keeping the same overall architecture (CMSIS-DAP HID + optional
USB-CDC). On the host the device appears as a standard CMSIS-DAP debugger; when
CDC is enabled an additional virtual COM port is exposed.

---

## Repository layout

| Path | Purpose |
|------|---------|
| `Firmware/User/` | `main.c`, `system_stm32f10x.c`, device header `stm32f10x.h`, SPL config |
| `Firmware/CMSIS-DAP/` | DAP / SWD / JTAG / SWO core, plus `DAP_config.h` (board-level pins, LEDs, USB pull-up) |
| `Firmware/USB/` | Keil RL-USB stack (CDC + HID) and STM32F103 USB glue |
| `Firmware/Driver/` | STM32F10x Standard Peripheral Library V3.5.0 and ARM driver wrappers |
| `Firmware/Core/` | CMSIS Cortex-M3 core headers |
| `gcc/` | GCC build assets: CMake toolchain, GAS startup, linker script, newlib stubs, ARMCC compatibility shim |
| `.github/workflows/` | GitHub Actions CI |

---

## Build with GCC + CMake + Ninja (recommended)

Dependencies: **arm-none-eabi-gcc** (tested with 13.2.Rel1 and 14.3.1),
**CMake >= 3.20**, **Ninja**. No Keil MDK installation is required; the
necessary CMSIS / SPL headers are in-tree.

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
ninja -C build
```

Outputs under `build/`:

- `CMSIS-DAP-STM32F103C6.elf`
- `CMSIS-DAP-STM32F103C6.hex`
- `CMSIS-DAP-STM32F103C6.bin`
- `CMSIS-DAP-STM32F103C6.map`

### Board / feature defines

Edit `STM32_DEFINES` in `CMakeLists.txt`, or override it on the command line:

```bash
cmake -S . -B build -G Ninja \
  -DSTM32_DEFINES="USE_STDPERIPH_DRIVER;STM32F10X_LD;BLUEPILL;CDC_UART_USE_USART1_PA9_PA10"
```

| Define | Meaning |
|--------|---------|
| `USE_STDPERIPH_DRIVER`, `STM32F10X_LD` | Standard Peripheral Library, low-density device |
| `BLUEPILL` | BluePill pin scheme (default) |
| `CDC_UART_USE_USART1_PA9_PA10` | CDC virtual COM on USART1 (PA9 / PA10) |
| `SWD_REMAP` (optional) | Remap SWDIO / SWCLK to PA13 / PA14 |
| `NRESET_1MS_TOGGLE_TEST` (optional) | Toggle nRESET ~1 ms in the main loop, oscilloscope debug only |

---

## Continuous Integration

`.github/workflows/build.yml` runs on `ubuntu-latest` and:

1. Downloads and extracts the official **GNU Arm Embedded Toolchain** tarball
   (cached via `actions/cache@v4`; cache hits skip the download).
2. Configures and builds with the same `cmake` / `ninja` commands as above.
3. Uploads `.elf` / `.hex` / `.bin` / `.map` as a workflow artifact.

GitHub Actions usage is free for public repositories.

---

## Default pin mapping (`BLUEPILL`, no `SWD_REMAP`)

Source of truth: `Firmware/CMSIS-DAP/DAP_config.h`.

| Function | Pin | Notes |
|----------|-----|-------|
| SWDIO (TMS) | PA2 | |
| SWCLK (TCK) | PA4 | |
| TDO / SWO sample | PB7 | |
| nRESET | PA7 | Toggled when `NRESET_1MS_TOGGLE_TEST` is on |
| USB soft pull-up | PA8 | `PIN_USB_CONNECT_*` |
| CDC UART TX / RX | PA9 / PA10 | Requires `CDC_UART_USE_USART1_PA9_PA10` |
| LED_CONNECTED | PB0 | Active-low |
| LED_RUNNING | PB8 | Active-low |

With `SWD_REMAP` defined: SWDIO / SWCLK move to **PA13 / PA14**; the rest
follow the corresponding `#if` branches in `DAP_config.h`.

---

## Building with Keil MDK (legacy path)

The original µVision project at `Firmware/User/CMSIS-DAP.uvprojx` still builds
under ARMCC v5 with the same set of preprocessor defines listed above. It is
kept for reference; the GCC + CMake path above is the supported workflow.

---

## License / upstream

### Lineage (oldest → newest)

1. **[x893/CMSIS-DAP](https://github.com/x893/CMSIS-DAP)** — original CMSIS-DAP reference implementation.
2. **[RadioOperator/STM32F103C8T6_CMSIS-DAP_SWO](https://github.com/RadioOperator/STM32F103C8T6_CMSIS-DAP_SWO)** — RadioOperator’s STM32F103 port with SWO and related changes (builds on x893’s code).
3. **[RagingWaves/CMSIS-DAP_C6T6](https://github.com/RagingWaves/CMSIS-DAP_C6T6)** — BluePill **STM32F103C6T6** CMSIS-DAP for an on-board debugger; documents upstream as RadioOperator + x893.
4. **This repository** — further development (arm-none-eabi-gcc, CMake, Ninja, linker/startup, CI, documentation, and firmware tweaks).

Firmware files inherit whatever license terms their respective upstreams used
(see the headers in each tree). When in doubt, treat CMSIS-DAP–related sources
as following the same spirit as [x893/CMSIS-DAP](https://github.com/x893/CMSIS-DAP)
and [RadioOperator/STM32F103C8T6_CMSIS-DAP_SWO](https://github.com/RadioOperator/STM32F103C8T6_CMSIS-DAP_SWO).

`Firmware/User/stm32f10x.h` and `Firmware/Driver/` come from STMicroelectronics’
STM32F10x Standard Peripheral Library V3.5.0 (Copyright (c) 2011
STMicroelectronics).
