echo off
cls
echo %1
mode COM3 BAUD=230400 DATA=8 PARITY=n DTR=OFF RTS=OFF
copy %1 /B \\.\COM3 /B
echo Press enter to exit
pause > nul




