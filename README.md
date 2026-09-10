# MOZA ATS Blinker — ATS 1.60 / MOZA RS V2

This project is a native SCS Telemetry SDK plugin for American Truck Simulator.
It reads the truck's actual left/right blinker state and drives the MOZA steering-wheel
shift/RPM indicator through MOZA SDK 1.0.1.8.

## Intended hardware

- MOZA R5
- MOZA RS V2
- MOZA Multi-Function Stalks
- Windows x64
- American Truck Simulator 1.60.x

## Effect

Normal:
- Your existing MOZA RPM/shift indicator configuration is preserved.

Left blinker:
- LEDs 1–5 flash amber according to ATS's actual `truck.light.lblinker` state.

Right blinker:
- LEDs 6–10 flash amber according to ATS's actual `truck.light.rblinker` state.

Hazards:
- All 10 LEDs flash amber.

When the signal is switched off, the plugin restores the indicator mode, switch mode,
colors and RPM thresholds that were active when the plugin started.

## Install

1. Build the DLL with `build.bat`.
2. Close ATS.
3. Create this folder if necessary:

   <ATS>\bin\win_x64\plugins\

4. Copy these two files from `build\bin\` into that folder:

   - `moza_ats_blinker.dll`
   - `MOZA_SDK.dll`

5. Start ATS.
6. Accept the normal SCS "advanced SDK features" warning if ATS displays it.
7. Enter the truck and test the left/right indicators.

## Requirements for building

- Windows 10/11 x64
- Visual Studio 2022 with "Desktop development with C++"
- CMake 3.25+

Run:

    build.bat

The project already contains the x64 MOZA SDK 1.0.1.8 files supplied with the project.

## Important

This is a telemetry plugin, not an `.scs` mod. It does not modify game files or your profile.

The plugin uses the official SCS telemetry channels:
- truck.lblinker
- truck.rblinker
- truck.hazard.warning
- truck.light.lblinker
- truck.light.rblinker

The actual on/off phase comes from the game's light channels, so the wheel LEDs follow
the ATS blink cycle rather than an independent timer.

## If the LED direction is reversed

The RS V2 LED order can vary by firmware/API mapping. If the physical left/right
halves are reversed, swap the two loops in `set_leds()` in:

    src\moza_ats_blinker.cpp

Then rebuild.

## Safety / recovery

If you dislike the effect, simply remove `moza_ats_blinker.dll` from the ATS plugins
folder. Pit House and the steering wheel remain usable normally.

If ATS reports a plugin crash, remove the DLL and send the ATS game log plus the plugin
build output.
