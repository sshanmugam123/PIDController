//Drivetrain control with motion profiles and PD loops, made by Shyaam Shanmugam.

double gearRatio = 0.75; // (Driven Gear Teeth) / (Driving Gear Teeth), values less than 1 are torque ratios, greater than 1 are speed ratios.
double wheelDiameter = 2.75; // Wheel diameter in inches. Old vex 4" wheels are actually 4.125".
double startingAngle = 0; // Angle which the robot starts at during autonomous. Typically, you want the direction your driver is facing to be 0 degrees.

//// Move PD

double moveKp = 3.5; // Proportional constant value, should be around 1.8. Higher values make the movement more accurate, but cause oscillations.
double moveKd = 1; // Derivative constant value, should be around half of moveKp. Higher values dampen oscillations, but might cause the robot to react to strongly (Robot will vibrate).
double moveThreshold = 0.8; // The + or - amount of inches the move can be off by to exit the loop. should be around 0.5.
double movePowerLimit = 80; // The maximum % voltage the drivetrain motors can be applied. Should be around 50 - 80.
//Move PD Timing
double moveThresholdTime = 30; // The amount of time in miliseconds it takes to exit the PD loop when it is within the threshold. Never less than 100.
//Move Motion Profile // If you do not want to do motion profiled movements, set all these values to 100. I recommend not changing anything.
double movePowerStart = 40; // The % voltage the drivetrain starts off at. Should be around 30 - 60. 
double movePowerEnd = 15; // The maximum % voltage the drivetrain ends the movement with. Should be around 15 - 30.
double moveAcceleration = 40; // The change in % voltage per inch when the robot is first accelerating. Should be around 10.
double moveDeceleration = 2.1; // The change in % voltage per inch when the robot is decelerating. Should be around 10. Higher values will make stops faster.
//Move Angle Adjustment
double angleAdjustment = 1; // Scales the constants for straight driving. When it is equal to 1 and the drivetrain is at full speed, the constants are identical to turnKp/Kd. Should be around 1

//// Turn PD

double turnKp = 2.6; // Proportional value, should be around 1.4. Higher values make the movement more accurate, but cause oscillations.
double turnKd = 1.3; // Derivative value, should be around half of turnKp. Higher values dampen oscillations, but might cause the robot to react too strongly.
double turnThreshold = 1.7; // The + or - amount of degrees the turn can be off by to exit the loop. Should be around 1.
double turnPowerLimit = 60; // The maximum % voltage the drivetrain motors can be applied. Should be around 50 - 80.
//Turn PD Timing
double turnThresholdTime = 30; // The amount of time in miliseconds it takes to exit the PD loop when it is within the threshold. Never less than 100.
//Turn Motion Profile // If you do not want to do motion profiled turns, set all these values to 100. I recommend not changing anything.
double turnPowerEnd = 15; // The maximum % voltage the drivetrain ends the turn with. Should be around 15.
double turnDeceleration = 0.6; // The change in % voltage per degree of rotation when the robot is decelerating. Should be around 0.5. Higher values will make stops faster.

//// Swing PD

double swingKp = 2; // Proportional value, should be around 1.8. Higher values make the movement more accurate, but cause oscillations.
double swingKd = 0.6; // Derivative value, should be around a fourth of swingKp. Higher values dampen oscillations, but might cause the robot to react too strongly, "death rattle".
double swingThreshold = 1.5; // The + or - amount of degrees the turn can be off by to exit the loop. Should be around 1.
double swingPowerLimit = 75; // The maximum % voltage the drivetrain motors can be applied. Should be around 50 - 80.
//Swing PD Timing
double swingThresholdTime = 50; // The amount of time in miliseconds it takes to exit the PD loop when it is within the threshold. Never less than 100.
//Swing Motion Profile // If you do not want to have motion profiled swings, set all these values to 100. I recommend not changing anything.
double swingPowerEnd = 15; // The maximum % voltage the drivetrain ends the swing with. Should be around 15.
double swingDeceleration = 0.8; // The change in % voltage per degree of rotation when the robot is decelerating. Should be around 0.6. Higher values will make stops faster.

/* ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 Below are the things you don't change unless you know what you are doing.
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "vex.h"

const double inchConversion = (360 / (wheelDiameter * 3.14159265358979)) / gearRatio;
double currentAngle = startingAngle, Inertial = startingAngle, leftAutonPower = 0, rightAutonPower = 0, leftDriverPower = 0, rightDriverPower = 0, target;
int motorCountLeft, motorCountRight, driveMode = 0;
bool timerActive = false, withinThreshold = false, driverControl, cubicArcadeControl = true, cubicTankControl = false, arcadeControl = false, tankControl = false;
motor* leftDrive[4], * rightDrive[4];
double errorValues[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

enum DriveState { STOP = 0, MOVE = 1, TURN = 2, SWINGLEFT = 3, SWINGRIGHT = 4 };

void leftDriveMotors(motor&drive1){leftDrive[0]=&drive1; motorCountLeft = 1;} // Set up for all configurations of driver control, condensed in an ugly way.
void leftDriveMotors(motor&drive1,motor&drive2){leftDrive[0]=&drive1;leftDrive[1]=&drive2; motorCountLeft = 2;}
void leftDriveMotors(motor&drive1,motor&drive2,motor&drive3){leftDrive[0]=&drive1;leftDrive[1]=&drive2;leftDrive[2]=&drive3; motorCountLeft = 3;}
void leftDriveMotors(motor&drive1,motor&drive2,motor&drive3,motor&drive4){leftDrive[0]=&drive1;leftDrive[1]=&drive2;leftDrive[2]=&drive3;leftDrive[3]=&drive4; motorCountLeft = 4;}
void rightDriveMotors(motor&drive1){rightDrive[0]=&drive1; motorCountRight = 1;}
void rightDriveMotors(motor&drive1,motor&drive2){rightDrive[0]=&drive1;rightDrive[1]=&drive2; motorCountRight = 2;}
void rightDriveMotors(motor&drive1,motor&drive2,motor&drive3){rightDrive[0]=&drive1;rightDrive[1]=&drive2;rightDrive[2]=&drive3; motorCountRight = 3;}
void rightDriveMotors(motor&drive1,motor&drive2,motor&drive3,motor&drive4){rightDrive[0]=&drive1;rightDrive[1]=&drive2;rightDrive[2]=&drive3;rightDrive[3]=&drive4; motorCountRight = 4;}

void driveLeft(double volts){ // Sets left drive motors to a voltage.
  for (int i = 0; i < (motorCountLeft); i++){ leftDrive[i]->spin(forward, volts, voltageUnits::volt); }
}

void driveRight(double volts){ // Sets right drive motors to a voltage.
  for (int i = 0; i < (motorCountRight); i++){ rightDrive[i]->spin(forward, volts, voltageUnits::volt); }
}

void driveCoast(){ // Sets stopping of all drive motors to "coast".
  for (int i = 0; i < (motorCountLeft); i++){ leftDrive[i]->setStopping(coast); }
  for (int i = 0; i < (motorCountRight); i++){ rightDrive[i]->setStopping(coast); }
}

void driveBrake(){ // Sets stopping of all drive motors to "brake".
  for (int i = 0; i < (motorCountLeft); i++){ leftDrive[i]->setStopping(brake); }
  for (int i = 0; i < (motorCountRight); i++){ rightDrive[i]->setStopping(brake); }
}

void driveHold(){ // Sets stopping of all drive motors to "hold".
  for (int i = 0; i < (motorCountLeft); i++){ leftDrive[i]->setStopping(hold); }
  for (int i = 0; i < (motorCountRight); i++){ rightDrive[i]->setStopping(hold); }
}

void driveStop(){ // Stops all drive motors and interupts any PD loop that is running.
  for (int i = 0; i < (motorCountLeft); i++){ leftDrive[i]->stop(); }
  for (int i = 0; i < (motorCountRight); i++){ rightDrive[i]->stop(); }
  driveMode = STOP;
}

void driveLeftStop(){for (int i = 0; i < (motorCountLeft); i++){ leftDrive[i]->stop(); }}
void driveRightStop(){for (int i = 0; i < (motorCountRight); i++){ rightDrive[i]->stop(); }}
void drivePositionReset(){leftDrive[0]->resetPosition(); rightDrive[0]->resetPosition();}

double motionProfile(double x, double endPoint, double slope1, double slope2, double yMin1, double yMin2, double yMax){ // Creates a trapezoidal function for motion profiling.
  double y = 0;
  double y1 = (slope1 * x) + yMin1; 
  double y2 = (slope2 * (x - endPoint)) + yMin2; 
  if (y1 <= y2){y = y1; if (y < yMin1){y = yMin1;}}
  else if(y1 > y2){y = y2; if (y < yMin2){y = yMin2;}}
  if (y > yMax){y = yMax;}
  return y;
}

void updateHeading(){ // Called everytime inertial sensor updates. Adds starting angle. Updates the variable inertial.
  Inertial = InertialSensor.rotation(degrees) + startingAngle;
}

void exitThresholdPD() { 
  if (withinThreshold) {
    driveCoast();
    withinThreshold = false;
    driveMode = STOP;
    for (int i = 0; i < 20; i++) {errorValues[i] = 0;}
  }
}

void checkExitConditions(double error, double threshold, double thresholdTime) { // Exits code if within threshold or errors are unchanging
  for (int i = 0; i < 20; i++) {printf("\n %lf", errorValues[i]);}
  for (int i = 19; i > 0; i--) {errorValues[i] = errorValues[i - 1];}
  errorValues[0] = error; bool allSame = true;
  for (int i = 0; i < 20; i++) {
      if (errorValues[i] != errorValues[0]) {allSame = false; break;}
      if (errorValues[i] == 0) {allSame = false; break;}}
  if (allSame) {
      printf("\n ##### Error: Settled out of threshold ##### "); printf("\n ##### Value of error when terminated:%lf ##### \n", error);
      withinThreshold = true;
      exitThresholdPD();
      return;}
  if (fabs(error) < threshold) {withinThreshold = true;
    if (!timerActive) {timer().event(exitThresholdPD, thresholdTime);}
  } else {withinThreshold = false;}
}

int drivetrainControl(){ // Drivetrain handler, runs PD loops and driver control.
  InertialSensor.changed(updateHeading);
  while (true){
    if (driverControl){
      if (cubicArcadeControl){ // arcade joystick style, with cubic output curves.
        leftDriverPower = pow(Controller1.Axis3.position(percent), 3) / 10000 + pow(Controller1.Axis1.position(percent), 3) / 10000;
        rightDriverPower = pow(Controller1.Axis3.position(percent), 3) / 10000 - pow(Controller1.Axis1.position(percent), 3) / 10000;
      }
      if (arcadeControl){ // arcade joystick style.
        leftDriverPower = Controller1.Axis3.position(percent) + Controller1.Axis1.position(percent);
        rightDriverPower = Controller1.Axis3.position(percent) - Controller1.Axis1.position(percent);
      }
      if (cubicTankControl){ // tank joystick style, with cubic output curves.
        leftDriverPower = pow(Controller1.Axis3.position(percent), 3) / 10000;
        rightDriverPower = pow(Controller1.Axis2.position(percent), 3) / 10000;
      }
      if (tankControl){ // tank joystick style.
        leftDriverPower = Controller1.Axis3.position(percent);
        rightDriverPower = Controller1.Axis2.position(percent);
      }
    }
    else{leftDriverPower = 0; rightDriverPower = 0;}

    if (driveMode == MOVE){ // PD loop and motion profile for moving.
      double error = (target - ((rightDrive[0]->position(degrees) + leftDrive[0]->position(degrees))/2))/inchConversion;
      double power = ((error * (moveKp / 0.12)) + (-(rightDrive[0]->velocity(percent) + leftDrive[0]->velocity(percent))/2) * moveKd); //Add P and D term
      double powerLimit = motionProfile(fabs(((rightDrive[0]->position(degrees) + leftDrive[0]->position(degrees))/2) / inchConversion), fabs(target/inchConversion), moveAcceleration, -moveDeceleration, movePowerStart, movePowerEnd, movePowerLimit);
      if (fabs(power) > powerLimit){if (power > 0){power = powerLimit;}else{power = -powerLimit;}}
      double anglePower = (((currentAngle - Inertial) * (turnKp / 0.12) + (InertialSensor.gyroRate(zaxis, dps)) * turnKd)) * fabs(power*(0.01 * angleAdjustment));
      leftAutonPower = power + anglePower;
      rightAutonPower = power - anglePower;
      checkExitConditions(error, moveThreshold, moveThresholdTime);
    }
    
    if (driveMode == TURN){ // PD loop and motion profile for turning.
      double error = target - Inertial;
      double power = ((error * (turnKp / 0.12)) + (InertialSensor.gyroRate(zaxis, dps)) * (turnKd)); //Add P and D term
      double powerLimit = motionProfile(0, fabs(error), 100, -turnDeceleration, 100, turnPowerEnd, turnPowerLimit);
      if (fabs(power) > powerLimit){if (power > 0){power = powerLimit;}else{power = -powerLimit;}}
      leftAutonPower = power;
      rightAutonPower = -power;
      checkExitConditions(error, turnThreshold, turnThresholdTime);
    }

    if (driveMode == SWINGLEFT || driveMode == SWINGRIGHT){ // PD loop and motion profile for swinging.
      double error = target - Inertial;
      double power = ((error * (turnKp / 0.12)) + (InertialSensor.gyroRate(zaxis, dps)) * (turnKd)); //Add P and D term
      double powerLimit = motionProfile(0, fabs(error), 100, -swingDeceleration, 100, swingPowerEnd, swingPowerLimit);
      if (fabs(power) > powerLimit){if (power > 0){power = powerLimit;}else{power = -powerLimit;}}
      if (driveMode == SWINGLEFT){rightAutonPower = -power; leftAutonPower = 0;}else{leftAutonPower = power; rightAutonPower = 0;}
      checkExitConditions(error, swingThreshold, swingThresholdTime);
    }

    if (driveMode == STOP){leftAutonPower = 0;rightAutonPower = 0;}
    if (leftAutonPower != 0 || leftDriverPower != 0){driveLeft((leftAutonPower + leftDriverPower) * 0.12);}else{driveLeftStop();}
    if (rightAutonPower != 0 || rightDriverPower != 0){driveRight((rightAutonPower + rightDriverPower) * 0.12);}else{driveRightStop();}
    wait(10, msec);
  }
  return 0;
}

void moveAsync(double Distance){ //Starts move PD loop in drivecontrol task and sets a target in inches, resets measuring motors to position 0.
  driveMode = STOP;
  drivePositionReset();
  target = Distance * inchConversion;
  driveMode = MOVE;
}

void turnAsync(double Angle){ //Starts turn PD loop in drivecontrol task and sets a target in degrees, sets angle correction to the target angle.
  driveMode = STOP;
  currentAngle = Angle; 
  target = Angle;
  driveMode = TURN;
}

void swingLeftAsync(double Angle){
  driveHold(); driveMode = STOP;
  currentAngle = Angle; target = Angle;
  driveMode = SWINGLEFT;
}

void swingRightAsync(double Angle){ 
  driveHold(); driveMode = STOP;
  currentAngle = Angle; target = Angle;
  driveMode = SWINGRIGHT;
}

double coterm(double angle){ //Calculates the closest coterminal angle to the parameter in the range -180 to 180.
  while(true){
    if(angle - Inertial >= 180){angle -= 360;}
    else if(angle - Inertial <= -180){angle += 360;} else{break;}
  }return angle;
}

void setStartingAngle(double angle){startingAngle = angle; currentAngle = startingAngle; Inertial = startingAngle;}//Sets the starting angle, should only be called before drivetrain control task is running.

double heading(){ return Inertial;} //Returns inertial reading.

void waitUntilAngle(double Angle){if(Inertial < Angle){while(Inertial < Angle){wait(10, msec);}}else{while(Inertial > Angle){wait(10, msec);}}}

void waitUntilDistance(double Distance){while(fabs((rightDrive[0]->position(deg) + leftDrive[0]->position(deg))/2) / inchConversion < fabs(Distance)){wait(10, msec);}}

bool isMoving(){if (driveMode == MOVE){return true;}else{return false;}}// Returns true when a movement PD loop is running.

bool isTurning(){ if (driveMode == TURN){return true;}else{return false;}}// Returns true when a turn PD loop is running.

bool isSwinging(){if (driveMode == SWINGLEFT || driveMode == SWINGRIGHT){return true;}else{return false;}}// Returns true when a swing PD loop is running.

void move(double Distance){moveAsync(Distance); while(driveMode == MOVE){wait(10, msec);}}//Calls and waits on asynchronous PD loop.

void turn(double Angle){turnAsync(Angle); while(driveMode == TURN){wait(10, msec);}}// Calls and waits on asynchronous PD loop.

void swingLeft(double Angle){swingLeftAsync(Angle); while(driveMode == SWINGLEFT){wait(10, msec);}}// Waits on asynchronous PD loop.

void swingRight(double Angle){swingRightAsync(Angle); while(driveMode == SWINGRIGHT){wait(10, msec);}}// Waits on asynchronous PD loop.

void setCubicArcadeControl(){cubicArcadeControl = true;} // Activates arcade joystick style, with exponential input curve for turning.

void setCubicTankControl(){cubicTankControl = true;}// Activates tank joystick style, with exponential input curves.

void setTankControl(){tankControl = true;}// Activates tank joystick style.

void setArcadeControl(){arcadeControl = true;}// Activates arcade joystick style.

void setMoveConstants(double P, double D){moveKp = P; moveKd = D;}// Sets move values.

void setMovePowerLimit(double limit){movePowerLimit = limit;}// Sets move values.

void setMoveThreshold(double thres){moveThreshold = thres;} // Sets move values.

void setTurnConstants(double P, double D){turnKp = P; turnKd = D;}// Sets turn values.

void setTurnPowerLimit(double limit){turnPowerLimit = limit;}// Sets turn values.

void setTurnThreshold(double thres){turnThreshold = thres;}// Sets turn values.

void setTurnProfile(double end, double decel){turnPowerEnd = end; turnDeceleration = decel;}// Sets turn values.

void setSwingConstants(double P, double D){swingKp = P; swingKd = D;}// Sets swing values.

void setSwingPowerLimit(double limit){swingPowerLimit = limit;}// Sets swing values.

void setSwingThreshold(double thres){ swingThreshold = thres;}// Sets swing values.

void setSwingProfile(double end, double decel){swingPowerEnd = end; swingDeceleration = decel;}// Sets swing values.

void setCurrentAngle(double deg){currentAngle = deg;}

void setMoveProfile(double start, double end, double accel, double decel){ // Sets move values.
  movePowerStart = start; movePowerEnd = end; moveAcceleration = accel; moveDeceleration = decel;
}

void moveArc(double Distance, double Deg){
  moveAsync(Distance); 
  double Accel = moveAcceleration; double Decel = moveDeceleration; double End = movePowerEnd; double Start = movePowerStart;
  setMoveProfile(Start, End, Accel, Decel ); double initialAngle = currentAngle;
  while(driveMode == MOVE || 1 > fabs((((rightDrive[0]->position(deg) + leftDrive[0]->position(deg))/2)/inchConversion))){
    wait(5, msec);
    currentAngle = initialAngle + (fabs((((rightDrive[0]->position(deg) + leftDrive[0]->position(deg))/2)/inchConversion)/Distance) * (Deg - initialAngle));
  }
  currentAngle = Deg; driveMode = STOP;
  setMoveProfile(Start, End, Accel, Decel);
}

void turnTuning(double Deg){
  printf("\n \n \n Start of Turn Test \n Values: \n");
  timer Time;
  Time.systemHighResolution();
  turnAsync(Deg);
  while(isTurning())
  {
    printf("\n Time: %lf \n Angle: %lf \n", Time.value(), InertialSensor.rotation());
    wait(10, msec);
  }
  printf("\n Settled in %lf seconds", Time.value());
}

void moveTuning(double Distance){
  printf("\n \n \n Start of Move Test \n Values: \n");
  timer Time;
  Time.systemHighResolution();
  moveAsync(Distance);
  while(isMoving())
  {
    printf("\n Time: %lf \n Distance: %lf Angle: %lf \n", Time.value(), (((rightDrive[0]->position(deg) + leftDrive[0]->position(deg))/2)/inchConversion)/inchConversion, InertialSensor.rotation());
    wait(10, msec);
  }
  printf("\n Settled in %lf seconds", Time.value());
}

void swingRightTuning(double Deg){
  printf("\n \n \n Start of Swing Right Test \n Values: \n");
  timer Time;
  Time.systemHighResolution();
  swingRightAsync(Deg);
  while(isSwinging())
   {
     printf("\n Time: %lf \n Angle: %lf \n", Time.value(), InertialSensor.rotation());
     wait(10, msec);
   }
  printf("\n Settled in %lf seconds", Time.value());
}

void swingLeftTuning(double Deg){
  printf("\n \n \n Start of Swing Left Test \n Values: \n");
  timer Time;
  Time.systemHighResolution();
  swingLeftAsync(Deg);
  while(isSwinging())
   {
     printf("\n Time: %lf \n Angle: %lf \n", Time.value(), InertialSensor.rotation());
     wait(10, msec);
   }
  printf("\n Settled in %lf seconds", Time.value());
}

void enableDrivetrain(){task chassis(drivetrainControl);}// Starts drivetrain control task.

void startDriver(){driverControl = true;}// Starts driver control