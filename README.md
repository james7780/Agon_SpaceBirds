# Space Birds

A simple game written in C for the Agon Light retro computer.

This game is inspired by classic games such as Demon Attack and Phoenix.

Level 1: 
    - Eggs hatch into small space birds, which attack you

Level 2:
    - Small space birds grow into big space birds, which shoot faster

Level 3:
    - Eggs hatch into big space birds, which divide into 2 small space birds after a while

Level 4:
    - FUTURE : Will be a boss level
    - NOW : Big space birds

The levels repeat after level 4, but getting harder: more birds will appear and will shoot faster

Right now there is an issue with reading multiple simultaneous keypresses on the Agon Light, hopefully this will be fixed in a future release of the VDP firmware.


NB:
- Compiled with Zilog ZDS2 IDE 
- The paths in the link files (Debug.linkcmd and Release.linkcmd) need to be modified to reflect where the tools are located on your hard drive before this will compile.