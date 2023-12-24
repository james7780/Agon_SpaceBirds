echo off
REM Convert .hex to .bin, then run in emulator
hex2bin -s 40000 Spacebirds.hex
copy Spacebirds.bin C:\Emulators\fab_0.9.12\sdcard







