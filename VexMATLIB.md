# VEXcode Pro With MATLIB
By Shyaam Shanmugam, 50810E 

### Introduction
MATLIB is a library that makes programming the drivetrain of a v5 robot significantly easier. It uses a simple proportional-derivative loop with motion profiling for drivetrain control.

## Project setup

### Creating a MATLIB project using the template
1. Install VEXcode with the installer linked above.
2. Download and unzip the code file.
4. Open the project in VEXcode.

### Adding Greenhat to an existing project
1. Download and unzip MATLIB.
2. Copy and paste the MATLIB.h and MATLIB.cpp into your own project.
3. Add `#include "MATLIB.h"` to the vex.h file.


## Getting started

If you have never coded in vexcode before, these videos are a good introduction:
- https://www.youtube.com/watch?v=zKzSx7_sklE
- https://www.youtube.com/watch?v=AKQtMJ7rUOs

### Configuring the drive
At the top of the drive.cpp file there is a set of user editable constants. These are the settings we can use to configure how the drive motors perform. 
Start by configuring the motor ports according to what ports your drive motors are plugged into. 
Make sure each motor is set to turn in a certain way, so that if it turns clockwise, the drivetrain moves forward.
Specify what motors are on each side of the drivetrain and list them in the `whenStarted();` function as shown.
