// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drive1               motor         19              
// Drive2               motor         20              
// Drive3               motor         18              
// Drive4               motor         17              
// Drive5               motor         16              
// Drive6               motor         15              
// Controller1          controller                    
// InertialSensor       inertial      14              
// clamp                digital_out   A               
// Intake               motor         3               
// doinkRight           digital_out   B               
// Rotation12           rotation      12              
// Optical              optical       4               
// hang                 digital_out   G               
// pod                  rotation      21              
// doinkLeft            digital_out   C               
// lift                 motor_group   2, 13           
// IntakeLift           digital_out   D               
// rushClamp            digital_out   E               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drive1               motor         19              
// Drive2               motor         20              
// Drive3               motor         18              
// Drive4               motor         17              
// Drive5               motor         16              
// Drive6               motor         15              
// Controller1          controller                    
// InertialSensor       inertial      14              
// clamp                digital_out   A               
// Intake               motor         3               
// doinkRight           digital_out   B               
// Rotation12           rotation      12              
// Optical              optical       4               
// hang                 digital_out   G               
// pod                  rotation      21              
// rusharm              digital_out   C               
// lift                 motor_group   2, 13           
// IntakeLift           digital_out   D               
// rushClamp            digital_out   E               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drive1               motor         19              
// Drive2               motor         20              
// Drive3               motor         18              
// Drive4               motor         17              
// Drive5               motor         16              
// Drive6               motor         15              
// Controller1          controller                    
// InertialSensor       inertial      14              
// clamp                digital_out   A               
// Intake               motor         3               
// doink                digital_out   B               
// Rotation12           rotation      12              
// Optical              optical       4               
// hang                 digital_out   G               
// pod                  rotation      21              
// rusharm              digital_out   C               
// lift                 motor_group   2, 13           
// IntakeLift           digital_out   D               
// rushClamp            digital_out   E               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drive1               motor         19              
// Drive2               motor         20              
// Drive3               motor         18              
// Drive4               motor         17              
// Drive5               motor         16              
// Drive6               motor         15              
// Controller1          controller                    
// InertialSensor       inertial      14              
// clamp                digital_out   A               
// Intake               motor         3               
// doink                digital_out   B               
// Rotation12           rotation      12              
// Optical              optical       4               
// hang                 digital_out   G               
// pod                  rotation      21              
// rusharm              digital_out   C               
// lift                 motor_group   2, 13           
// Potentiometer        potV2         F               
// IntakeLift           digital_out   D               
// rushClamp            digital_out   E               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drive1               motor         19              
// Drive2               motor         20              
// Drive3               motor         18              
// Drive4               motor         17              
// Drive5               motor         16              
// Drive6               motor         15              
// Controller1          controller                    
// InertialSensor       inertial      14              
// clamp                digital_out   A               
// Intake               motor         3               
// doink                digital_out   B               
// Rotation12           rotation      12              
// Optical              optical       4               
// hang                 digital_out   G               
// pod                  rotation      21              
// rusharm              digital_out   C               
// lift                 motor_group   2, 13           
// Potentiometer        potV2         F               
// IntakeLift           digital_out   D               
// ---- END VEXCODE CONFIGURED DEVICES ----
  
#include "vex.h"
using namespace vex;
competition Competition;
bool ClampState = false; 
bool HangState = false;
bool autonRingdetector = true;
bool activateStop = false;
bool autonLb = true;
bool Lbscore = true;
bool Lbreset = true;
bool ringStopper = false;
bool antiJam = false;
bool ringFling = false;
bool liftTaskEnabled = true;

bool teamColor;
double globalFdeg;

#define RED true
#define BLUE false

/////// Wall Stake Positions ///////

double AlliancePosition = 150;
double HighHold = 80;
double Activate = 21;
double Down = 0;
double Descore = 139;
double AllianceScore = 200;

////////////////////////////////////

void liftAllyPos(){globalFdeg = AlliancePosition; liftTaskEnabled = true;}    
void liftHighHold() {globalFdeg = HighHold; liftTaskEnabled = true;}
void liftActivate(){globalFdeg = Activate; liftTaskEnabled = true;}    
void liftDown(){globalFdeg = Down; liftTaskEnabled = true;} 
void DescoreArm(){globalFdeg = Descore; liftTaskEnabled = true;}
void liftAllianceScore(){globalFdeg = AllianceScore; liftTaskEnabled = true;}

void ToggleLift(){
  liftTaskEnabled = true;
  if (globalFdeg == Down){
    globalFdeg = Activate;
  }else{
    globalFdeg = Down;
  }
}

color detectColor = Optical.color();
double hue = Optical.hue();

void whenStarted()
{
  InertialSensor.startCalibration();
  wait(1, sec);
  waitUntil(!InertialSensor.isCalibrating());
  leftDriveMotors(Drive1, Drive2, Drive3);
  rightDriveMotors(Drive6, Drive5, Drive4);
  lift.setStopping(hold);
  rotation Rotation3 = rotation(PORT3,false);
  Rotation3.resetPosition();
  enableDrivetrain();
}

void IntakeIn(){Intake.spin(forward, 11 , voltageUnits::volt);}
void IntakeOut(){Intake.spin(forward, -11 , voltageUnits::volt);}
void IntakeInBig(){Intake.spin(forward, 12 , voltageUnits::volt);}
void IntakeOutBig(){Intake.spin(forward, -12 , voltageUnits::volt);}
void IntakeStop(){Intake.stop();}

void lifting(){lift.spin(forward, 12 , voltageUnits::volt); liftTaskEnabled = false;}
void liftdescend(){lift.spin(forward, 11 , voltageUnits::volt);}
void liftingstop(){lift.stop(brake);}

void extendDoinkRight(){doinkRight.set(true);}
void retractDoinkRight(){doinkRight.set(false);}
void extendDoinkLeft(){doinkLeft.set(true);}
void retractDoinkLeft(){doinkLeft.set(false);}
void IntakeUp(){IntakeLift.set(true);}
void IntakeDown(){IntakeLift.set(false);}
void extendGoalRush(){rushClamp.set(true);}
void retractGoalRush(){rushClamp.set(false);}



int ringController(){
  while (true){
    Optical.setLight(ledState::on);
    Optical.setLightPower(100, percent); 
    double hue = Optical.hue();
    //////////////////////////////////////// ANTI INTAKE JAM ////////////////////////////////////////////
    if(fabs(Intake.velocity(pct)) < 2 && fabs(Intake.voltage()) > 3 && antiJam == true && ringStopper == false){
      wait(0.2, sec);
      if(fabs(Intake.velocity(pct)) < 2 && fabs(Intake.voltage()) > 3 && antiJam == true){
        wait(0.3, sec);
        IntakeOutBig();
        wait(0.3,seconds);
        IntakeIn();
        Controller1.Screen.print("JAM"); // debug
      }
    }
    if (teamColor == RED){ ////////////////////////// RED TEAM RING SORT //////////////////////////////////
      if(hue >= 107 && hue<= 233){
        if(ringStopper == true&& Optical.isNearObject() && hue >= 0 && hue<= 38){
          wait(220,msec);
          IntakeStop();
          Controller1.Screen.print("R1 "); // debug
        }
        else if(hue >= 107 && hue<= 233 && ringStopper == false && ringFling == true){
          wait(131, msec); //145 on 107 //251 //231
          IntakeOut();
          wait(170, msec);
          IntakeIn();
          Controller1.Screen.print("R2 "); // debug
        }
      }else{
        if(ringStopper == true&& Optical.isNearObject() && hue >= 0 && hue<= 38){
          IntakeStop();
          Controller1.Screen.print("R3 "); // debug
        }
        else if(hue >= 107 && hue<= 233 && ringStopper == false && ringFling == true){
          wait(231, msec); //145 on 107 //251
          IntakeOut();
          wait(170, msec);
          IntakeIn();
          Controller1.Screen.print("R4 "); // debug
        }
      }
    }
    else if(teamColor == BLUE){////////////////////// BLUE TEAM RING SORT /////////////////////////////////
      if(hue >= 0 && hue<= 30){
        if(ringStopper == true && Optical.isNearObject() && hue >= 107 && hue<= 233 ){
          wait(190,msec);
          IntakeStop();
          Controller1.Screen.print("B1 "); // debug
        }
        else if(hue >= 0 && hue<= 30 && ringStopper == false && ringFling == true){
          wait(200, msec); //145 on 107 //251
          IntakeOut();
          wait(175, msec);
          IntakeIn();
          Controller1.Screen.print("B2 "); // debug
        }
      }
      else {
        if(ringStopper == true && Optical.isNearObject() && hue >= 180 && hue<= 233 ){
          IntakeStop();
          Controller1.Screen.print("B3 "); // debug
        }
        else if(hue >= 0 && hue<= 30 && ringStopper == false && ringFling == true){
          wait(240, msec); //145 on 107
          IntakeOut();
          wait(170, msec);
          IntakeIn();
          Controller1.Screen.print("B4 "); // debug
        }
      }    
    }
    wait(7, msec);
  }
  return 0;
}

task ringControl(ringController);

void Driver()
{
  startDriver();
  lift.setStopping(hold);
  IntakeLift.set(false);
  driveCoast();
  ringControl.stop();
}

int liftController(){
  double kp = 0.22;
  double kd = 0.0001;
  globalFdeg = 0;
  lift.setStopping(hold);
  double Lerror;
  while(true){
    if (liftTaskEnabled){
      Lerror = globalFdeg - Rotation12.position(deg);
      lift.spin(forward, kp*Lerror - kd*Rotation12.velocity(dps), voltageUnits::volt);
    }
    wait(10, msec);
  }
 return 0;
}

task liftControl(liftController);

void Clamping(){
  if(ClampState == false){
    clamp.set(true);
    ClampState = true;
  }
  else if(ClampState == true){
    clamp.set(false);
    ClampState = false;
  }
}

void Hanging(){
  if(HangState == false){
    hang.set(true);
    HangState = true;
  }
  else if(HangState == true){
    hang.set(false);
    HangState = false;
  }
}

void turnGoal(double deg){
  setTurnProfile(25, 0.14);
  setTurnPowerLimit(65);
  setTurnConstants(1.8, 1.55);
  turn(deg);
  setTurnProfile(15, 0.5);
  setTurnPowerLimit(80);
  setTurnConstants(1.8, 1.5);
}

void Rush(double Distance){
  setMoveProfile(100, 100, 100, 100);
  setMovePowerLimit(100);
  moveAsync(Distance + 0.5);
  waitUntilDistance(Distance);
  driveStop();
  rushClamp.set(true);
  move(-Distance);
  setMoveProfile(100, 100, 100, 100); //
  setMovePowerLimit(100);             //
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// BEGINNING OF AUTONOMOUS ROUTES /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Auton(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(0);
  
  // Insert path here
  IntakeIn();
  move(39);
  wait(200, msec);
  moveArc(-19, -90);
  move(-10);
  clamp.set(true);




  Controller1.Screen.print(autonTime.value());
}

void AutonTuning(){
  moveTuning(24);
  turnTuning(90);
  swingLeftTuning(-90);
  swingRightTuning(90);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// ELIMINATION AUTOS /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void blueGoalRushElims(){
  timer autonTime;
  teamColor = BLUE;
  setStartingAngle(18.22);
  task ringControlOn(ringController);
  ringFling = true;
  


  setMoveProfile(100, 100, 100, 100);
  liftActivate();
  liftHighHold();
  moveAsync(36);
  IntakeIn();
  ringStopper = true;
  extendDoinkLeft();
  waitUntilDistance(35.5);
  waitUntil(isMoving() == false);
  extendGoalRush();
  driveStop();
  move(-26);
  setMoveProfile(40, 15, 40, 2.1);
  
  
  
  
  ringStopper = false;
  IntakeStop();
  antiJam = true;
  retractGoalRush();
  wait(500, msec);
  turn(185);
  retractDoinkLeft();
  wait(100,msec);
  move(-20);
  clamp.set(true);
  wait(100,msec);
  IntakeIn();
  moveArc(34,90);
  clamp.set(false);
  ringFling=false;
  ringStopper = true;
  
  setMovePowerLimit(100);
  move(30);

  setMovePowerLimit(50);
  move(20);
  wait(300, msec);
  setMovePowerLimit(100);
  move(-9.5);
  extendDoinkRight();
  turn(coterm(190));
  retractDoinkRight();
  turn(coterm(180));
  setMovePowerLimit(70);
  move(20.5);
  setMoveProfile(100, 100, 100, 100);
  setMovePowerLimit(100);
  move(-9);
  liftAllianceScore();
  wait(200, msec);
  move(-15);
  turn(130);
  move(-30);
  clamp.set(true);
  IntakeIn();

  
  


  
  


  


  

  

  Controller1.Screen.print(autonTime.value());
}

void redGoalRushElims(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(18.47);
  task ringControlOn(ringController);
  ringFling = true;
  


  setMoveProfile(100, 100, 100, 100);
  liftActivate();
  liftHighHold();
  moveAsync(36);
  IntakeIn();
  ringStopper = true;
  extendDoinkLeft();
  waitUntilDistance(35.5);
  waitUntil(isMoving() == false);
  extendGoalRush();
  driveStop();
  move(-26);
  setMoveProfile(40, 15, 40, 2.1);
  
  
  ringStopper = false;
  IntakeStop();
  antiJam = true;
  retractGoalRush();
  wait(500, msec);
  turn(185);
  retractDoinkLeft();
  wait(100,msec);
  move(-22);
  clamp.set(true);
  wait(300,msec);
  IntakeIn();
  
  moveArc(38,(coterm(270)));
  clamp.set(false);
  ringFling=false;
  ringStopper = true;
  
  setMovePowerLimit(70);
  move(30);

  wait(300, msec);
  setMovePowerLimit(100);
  move(-11);
  extendDoinkLeft();
  turn(coterm(170));
  retractDoinkLeft();
  turn(coterm(180));
  setMovePowerLimit(70);
  move(21);
  setMoveProfile(100, 100, 100, 100);
  setMovePowerLimit(100);
  move(-9);
  
  liftAllianceScore();
  wait(300, msec);
  
  move(-15);
  turn(230);
  
  move(-30);
  clamp.set(true);
  IntakeIn();




  Controller1.Screen.print(autonTime.value());
}

void blueRingRushElimsGoal(){
timer autonTime;
  teamColor = BLUE;
  setStartingAngle(19.46);
  task ringControlOn(ringController);
  ringFling = true;
  antiJam =false;
    
  setMoveProfile(100, 100, 100, 100);
  ringStopper = true;
  moveAsync(54);
  IntakeIn();
  extendDoinkRight();
  waitUntilDistance(54);
  move(-6);
  turn(55);
  move(-28);
  waitUntilDistance(12);
  setMovePowerLimit(70);
  clamp.set(true);
  retractDoinkRight();
  ringStopper = false;
  antiJam =true;
  IntakeIn();
  wait(300, msec);
  setMovePowerLimit(50);
  move(5);
  turn(80);
  move(35);
  wait(500, msec);
  setMovePowerLimit(100);
  move(-43);
  turn(-33);
  extendDoinkRight();
  IntakeStop();
  move(19.5);
  wait(200, msec);
  setMovePowerLimit(70);
  move(-55);
  wait(200, msec);
  retractDoinkRight();
  wait(200,msec);
  move(-5);
  swingRight(-17);
  IntakeIn();
  move(15);
  turn(-130);
  wait(200, msec);
  move(13);
  wait(500,msec);
  turn(-90);
  wait(200,msec);
  IntakeUp();
  move(16);
  IntakeDown();
  move(-5);
  
  

  Controller1.Screen.print(autonTime.value());
}

void redRingRushElimsGoal(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(242.28);
  task ringControlOn(ringController);
  ringFling = true;

  setMoveProfile(100, 100, 100, 100);
  ringStopper = true;
  moveAsync(54);
  IntakeIn();
  extendDoinkLeft();
  waitUntilDistance(54);
  move(-6);
  turn(-55);
  move(-28);
  waitUntilDistance(12);
  setMovePowerLimit(70);
  clamp.set(true);
  retractDoinkLeft();
  ringStopper = false;
  antiJam =true;
  IntakeIn();
  wait(300, msec);
  setMovePowerLimit(50);
  move(5);
  turn(-80);
  move(35);
  wait(500, msec);
  setMovePowerLimit(100);
  move(-43);
  turn(33);
  extendDoinkLeft();
  IntakeStop();
  move(19.5);
  wait(200, msec);
  setMovePowerLimit(70);
  move(-55);
  wait(200, msec);
  retractDoinkLeft();
  wait(200,msec);
  move(-5);
  swingRight(17);
  IntakeIn();
  move(15);
  turn(130);
  wait(200, msec);
  move(13);
  wait(500,msec);
  turn(90);
  wait(200,msec);
  IntakeUp();
  move(16);
  IntakeDown();
  move(-5);



  Controller1.Screen.print(autonTime.value());
}

void blueRingRushElimsAlliance(){
  timer autonTime;
  teamColor = BLUE;
  setStartingAngle(0);
  task ringControlOn(ringController);
  ringFling = true;

  //Add Path



  Controller1.Screen.print(autonTime.value());
}

void redRingRushElimsAlliance(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(-18.15);
  task ringControlOn(ringController);
  ringFling = true;
  antiJam =true;

  setMoveProfile(100, 100, 100, 100);
  ringStopper = true;
  moveAsync(47);
  printf("%f \n" ,autonTime.value() );
  IntakeIn();
  extendDoinkLeft();
  waitUntilDistance(47);
  move(-6);
  turn(-63);
  move(-24);
  clamp.set(true);
  ringStopper= false;
  retractDoinkLeft();
  wait(500, msec);
  move(5);
  IntakeIn();
  turn(-88);
  move(27);
  wait(200, msec);
  move(-10);
  turn(coterm(142));
  move(29);
  wait(900, msec); //wait to score preload
  clamp.set(false);
  turn(coterm(90));
  ringFling =false;
  ringStopper =true;
  setMovePowerLimit(40);
  move(25);
  wait(600, msec);
  ringStopper =false;
  liftActivate();
  antiJam=false;
  IntakeIn();
  wait(0.7 ,sec); //wait to load lb
  IntakeStop();
  move(-9.5);
  extendDoinkRight();
  turn(coterm(190));
  retractDoinkRight();
  turn(coterm(180));
  setMovePowerLimit(70);
  move(20);
  setMovePowerLimit(40);
  moveAsync(-8);
  liftAllianceScore();
  wait(400, msec);
  move(-5);

  



  Controller1.Screen.print(autonTime.value());
  printf("%f \n" ,autonTime.value() );
}

void  bluePosElimsFillGoal(){
  timer autonTime;
  teamColor = BLUE;
  setStartingAngle(0);
  task ringControlOn(ringController);
  ringFling = true;

  //Add Path



  Controller1.Screen.print(autonTime.value());
}

void redPosElimsFillGoal(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(0);
  task ringControlOn(ringController);
  ringFling = true;

  //Add Path



  Controller1.Screen.print(autonTime.value());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// QUALIFICATION AUTOS /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void blueSoloAWP(){
  timer autonTime;
  teamColor = BLUE;
  setStartingAngle(118);
  task ringControlOn(ringController);
  ringFling = true;

  setMovePowerLimit(100);
driveCoast();
globalFdeg = 195;
wait(.5,sec);
move(-14);
turn(180);
globalFdeg = 0;
move(-37);
clamp.set(true);
ringStopper = true;
wait(.2,sec);
turn(270);
IntakeIn();
move(34);
wait(.3,sec);
ringStopper = false;
turn(135);
IntakeIn();
move(40);//31
turn(90); //91
clamp.set(false);
setMovePowerLimit(80);
ringStopper = true;
move(59);
setMovePowerLimit(100);
turn(180); //turn to grab 2nd goal
move(-31);
clamp.set(true);
ringStopper = false;
turn(91);
IntakeIn();
move(21);
wait(.3,sec);
move(-8);
turn(-36);
globalFdeg= 160;
setMovePowerLimit(32);
move(18);

  Controller1.Screen.print(autonTime.value());
}

void redSoloAWP(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(118);
  task ringControlOn(ringController);
  ringFling = true;

  setMovePowerLimit(100);
driveCoast();
globalFdeg = 195;
wait(.5,sec);
move(-14);
turn(180);
globalFdeg = 0;
move(-37);
clamp.set(true);
ringStopper = true;
wait(.2,sec);
turn(270);
IntakeIn();
move(34);
wait(.3,sec);
ringStopper = false;
turn(135);
IntakeIn();
move(40);//31
turn(90); //91
clamp.set(false);
setMovePowerLimit(80);
ringStopper = true;
move(59);
setMovePowerLimit(100);
turn(180); //turn to grab 2nd goal
move(-31);
clamp.set(true);
ringStopper = false;
turn(91);
IntakeIn();
move(21);
wait(.3,sec);
move(-8);
turn(-36);
globalFdeg= 160;
setMovePowerLimit(32);
move(18);

  Controller1.Screen.print(autonTime.value());

}

void bluePosHalfAWP(){
  timer autonTime;
  teamColor = BLUE;
  setStartingAngle(118.35);
  task ringControlOn(ringController);
  ringFling = true;
  

  setMovePowerLimit(100);
  antiJam=true;
  globalFdeg = 195;
  wait(400,msec);
  move(-8);
  globalFdeg = 0;
  ringStopper=true;
  turn(66);
  wait(100,msec);
  IntakeIn();
  setMovePowerLimit(100);
  IntakeUp();
  move(17);
  wait(.1,sec);
  IntakeDown();
  wait(.1,sec);
  move(-6);
  turn(158);
  setMovePowerLimit(100);
  move(-32);
  wait(.2,sec);
  clamp.set(true);
  setMovePowerLimit(100);
  wait(.1,sec);
  ringStopper = false;
  turn(270);
  IntakeIn();
  move(24);
  wait(.9,sec);
  turn(87);
  globalFdeg = 195;
  IntakeStop();
  move(22);
  turn(45);
  move(15);



  Controller1.Screen.print(autonTime.value());
}

void redPosHalfAWP(){
    timer autonTime;
  teamColor = RED;
  setStartingAngle(-117);
  task ringControlOn(ringController);
  ringFling = true;


 setMovePowerLimit(100);
  antiJam=true;
  globalFdeg = 195;
  wait(400,msec);
  move(-8);
  globalFdeg = 0;
  ringStopper=true;
  turn(-66);
  wait(100,msec);
  IntakeIn();
  setMovePowerLimit(100);
  IntakeUp();
  move(17);
  wait(.1,sec);
  IntakeDown();
  wait(.1,sec);
  move(-6);
  turn(-158);
  setMovePowerLimit(100);
  move(-32);
  wait(.2,sec);
  clamp.set(true);
  setMovePowerLimit(100);
  wait(.1,sec);
  ringStopper = false;
  turn(-270);
  IntakeIn();
  move(26);
  wait(.9,sec);
  turn(-87);
  globalFdeg = 195;
  IntakeStop();
  move(24);
  turn(-45);
  move(16);




  Controller1.Screen.print(autonTime.value());
}

void blueNegHalfAWP(){
  timer autonTime;
  teamColor = BLUE;
  setStartingAngle(-117);
  task ringControlOn(ringController);
  ringFling = true;

 setMovePowerLimit(100);
  antiJam=true;
  globalFdeg = 195;
  wait(400,msec);
  move(-8);
  globalFdeg = 0;
  ringStopper=true;
  turn(-66);
  wait(100,msec);
  IntakeIn();
  setMovePowerLimit(100);
  IntakeUp();
  move(17);
  wait(.1,sec);
  IntakeDown();
  wait(.1,sec);
  move(-6);
  turn(-158);
  setMovePowerLimit(100);
  move(-32);
  wait(.2,sec);
  clamp.set(true);
  setMovePowerLimit(100);
  wait(.1,sec);
  ringStopper = false;
  turn(-270);
  IntakeIn();
  move(24);
  wait(.9,sec);
  turn(-87);
  globalFdeg = 195;
  IntakeStop();
  move(22);
  turn(-45);
  move(15);



  Controller1.Screen.print(autonTime.value());

}

void redNegHalfAWP(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(120.4);
  task ringControlOn(ringController);
  ringFling = true;

  
   setMovePowerLimit(100);
  antiJam=true;
  globalFdeg = 195;
  wait(400,msec);
  move(-8);
  globalFdeg = 0;
  ringStopper=true;
  turn(66);
  wait(100,msec);
  IntakeIn();
  setMovePowerLimit(100);
  IntakeUp();
  move(17);
  wait(.1,sec);
  IntakeDown();
  wait(.1,sec);
  move(-6);
  turn(158);
  setMovePowerLimit(100);
  move(-32);
  wait(.2,sec);
  clamp.set(true);
  setMovePowerLimit(100);
  wait(.1,sec);
  ringStopper = false;
  turn(270);
  IntakeIn();
  move(24);
  wait(.9,sec);
  turn(87);
  globalFdeg = 195;
  IntakeStop();
  move(22);
  turn(45);
  move(15);

  



  Controller1.Screen.print(autonTime.value());

}

void blueRingRushQuals(){
  timer autonTime;
  teamColor = BLUE;
  setStartingAngle(19.46);
  task ringControlOn(ringController);
  ringFling = true;
  antiJam =true;
    
  setMoveProfile(100, 100, 100, 100);
  ringStopper = true;
  moveAsync(54);
  IntakeIn();
  extendDoinkRight();
  waitUntilDistance(54);
  move(-35);
  setMoveProfile(40, 15, 40, 2.1);
  setMovePowerLimit(100);
  retractDoinkRight();
  wait(200, msec);
  turn(155);
  
  move(-25);
  clamp.set(true);
  ringStopper = false;
  IntakeIn();
  move(5);
  turn(105);
  move(40);
  //add in middle ring grab here for elims ring rush autos
  wait( 500, msec);
  move(-15);
  turn(228);
  move(20);
  

  


 Controller1.Screen.print(autonTime.value());
}

void redRingRushQuals(){
  timer autonTime;
  teamColor = RED;
  setStartingAngle(17.47);
  task ringControlOn(ringController);
  ringFling = true;

  
  

  


 Controller1.Screen.print(autonTime.value());
}

void blueGoalRushQuals(){
   timer autonTime;
  teamColor = BLUE;
  setStartingAngle(0);
  task ringControlOn(ringController);
  ringFling = true;

  //Add Path



  Controller1.Screen.print(autonTime.value());
}

void redGoalRushQuals(){
   timer autonTime;
  teamColor = RED;
  setStartingAngle(0);
  task ringControlOn(ringController);
  ringFling = true;

  //Add Path



  Controller1.Screen.print(autonTime.value());
}

//////////
//SKILLS//
//////////
void Skills(){
  setStartingAngle(135.6);
  //setStartingAngle(220); //test
  //setStartingAngle(46.71); //test
  setMovePowerLimit(100);
  //setMoveThreshold(.3);
  setTurnThreshold(1.5);
  setSwingThreshold(1);
  //setMoveConstants(2.4, 0.4);
  //rings.setPriority(1);
  timer AutoTime;
  //task LB(lifta);
  teamColor = RED;
  antiJam = true;
  //first stuff
  
  globalFdeg = 195; // scores alliance
  wait(.4,sec);
  move(-15);
  //wait(.2,sec);
  clamp.set(true);
  globalFdeg = 0;
  //wait(.3,sec);
  turn(-5);
  IntakeIn();
  move(17); //first ring pick up
  turn(-24);
  antiJam = false;
  moveAsync(53);//move to second ring
  waitUntilDistance(30);
  //IntakeIn();
  globalFdeg = 17;
  waitUntil(isMoving() == false);
  //IntakeStop();
  turn(-195);
  wait(.3,sec);
  IntakeStop();
  move(20);
  globalFdeg = 80;
  turn(-90);
  //antiJam = true;
  IntakeIn();
  setMovePowerLimit(80);
  moveAsync(19);
  wait(1.2,sec);
  globalFdeg = 170;
  wait(.2,sec);
  //
  move(-13);
  //IntakeStop();
  turn(-179);
  globalFdeg = 0;
  //IntakeInBig();
  antiJam = true;
  move(30);
  setMovePowerLimit(27);
  move(26);
  setMovePowerLimit(60);
  turn(-52);
  move(12);
  turn(16);
  wait(.3,sec);
  moveAsync(-11);
  wait(.8,sec);
  driveStop();
  clamp.set(false);
  wait(.2,sec);
  setMovePowerLimit(100);
  IntakeStop();
  move(7);
  turn(5);
  //ringStopper = true;
  
  
  move(74); //91 alliance take
  //wait(.3,sec);
  turn(-139); //-125
  move(-38);
  //wait(.2,sec);
  clamp.set(true);
  //antiJam = true;
  turn(100);
  //move(5);
  clamp.set(false);
  //globalFdeg = 17;
  //wait(.2,sec);
  move(-27.5);
  //wait(1.5,sec);
  //driveStop();
  wait(.2,sec);
  ringStopper = false;
  move(20);
  turn(290);
  move(-23);
  wait(.2,sec);
  clamp.set(true);
  IntakeIn();
  turn(205);
  move(29);
  turn(132);
  wait(.5,sec);
  //IntakeStop();
  move(20);
  IntakeIn();
  move(17);
  wait(.4,sec);
  IntakeStop();
  setMovePowerLimit(70);
  moveAsync(62);
  waitUntil(18);
  IntakeIn();
  waitUntil(isMoving() == false);
  turn(180);
  move(10);
  wait(.4,sec); 
  turn(59);
  move(11.5);
  turn(-30);
  wait(.3,sec);
  IntakeStop();
  clamp.set(false);
  move(-11);
  move(6);
  turn(-19);
  ringStopper = true;
  IntakeIn();
  move(22);
  turn(45);
  move(-22);
  clamp.set(true);
  ringStopper = false;
  wait(.1,sec);
  turn(20);
  IntakeInBig();
  move(46);
  turn(90);
  globalFdeg = 17;
  setMovePowerLimit(80);
  //wait(.2,sec);
  antiJam = false;
  moveAsync(19);
  wait(1.9,sec);
  IntakeStop();
  globalFdeg = 170;
  wait(.2,sec);
  printf("%lf \n", AutoTime.time(sec)); 
  setMovePowerLimit(100);
  move(-10);
  globalFdeg = 0;
  turn(2);
  IntakeIn();
  antiJam = true;
  move(22);
  turn(-90);
  move(20);
  turn(44);
  move(29);
  turn(225);
  clamp.set(false);
  moveAsync(-12);
  wait(.3,sec);
  move(15);
  turn(50);
  globalFdeg = (195);
  move(-40);

  // turn(0);
  // move(10);
  // swingLeft(93);
  // move(12);
  // turn(190);
  // moveAsync(-10);
  //clamp.set(false);
 printf("%lf \n", AutoTime.time(sec)); 
Controller1.Screen.print(AutoTime.time(sec));

  






  
   /*
  
  //IntakeInBig();
  turn(-73);
  move(-26);
  IntakeStop();
  clamp.set(true);
  turn(-150); */
  /*
  setMovePowerLimit(40);
  moveAsync(15);
  wait(.6,sec);
  driveStop();
  wait(.2,sec);
  move(-8);
  globalFdeg = 195; */
  /*turn(-84);
  extendDoink();
  wait(.2,sec);
  moveAsync(27);
  wait(1.2,sec);
  driveStop();
  turn(-260); */
  //moveArc(6,-270); */
  printf("%lf \n", AutoTime.time(sec)); 
  
  
}

void SafeSkills(){
    setStartingAngle(137.3);
  //setStartingAngle(220); //test
  //setStartingAngle(46.71); //test
  setMovePowerLimit(100);
  //setMoveThreshold(.3);
  setTurnThreshold(1.5);
  setSwingThreshold(1);
  //setMoveConstants(2.4, 0.4);
  //rings.setPriority(1);
  timer AutoTime;
  //task atuonLbtask(AutonLift);
  teamColor = RED;
  antiJam = true;
  //first stuff
  
  globalFdeg = 195; // scores alliance
  wait(.4,sec);
  move(-15.5);
  //wait(.2,sec);
  clamp.set(true);
  globalFdeg = 0;
  //wait(.3,sec);
  turn(0);
  IntakeInBig();
  move(16.1); //first ring pick up
  turn(-25);
  //antiJam = false;
  move(56.5);//move to second ring
  /*
  waitUntilDistance(30);
  //IntakeIn();
  globalFdeg = 14.1;
  waitUntil(isMoving() == false);
  */
  //IntakeStop();
  turn(-178);
  wait(.3,sec);
  //IntakeStop();
  move(52);
  //wait(.2,sec);
  /*
  globalFdeg = 80;
  turn(-90);
  //antiJam = true;
  IntakeIn();
  setMovePowerLimit(80);
  moveAsync(19);
  wait(1.2,sec);
  globalFdeg = 170;
  wait(.2,sec);
  //
  move(-12);
  //IntakeStop();
  turn(-179);
  globalFdeg = 0;
  //IntakeInBig();
  antiJam = true;
  move(30); */
  setMovePowerLimit(27);
  move(30);
  setMovePowerLimit(60);
  turn(-52);
  move(15);
  turn(16);
  wait(.3,sec);
  moveAsync(-12);
  wait(.7,sec);
  driveStop();
  clamp.set(false);
  wait(.2,sec);
  setMovePowerLimit(100);
  IntakeStop();
  move(7);
  turn(5);
  //ringStopper = true;
  
  
  move(77); //91 alliance take
  //wait(.3,sec);
  turn(-139); //-125
  
  move(-39);
  //wait(.2,sec);
  clamp.set(true);
  //antiJam = true;
  turn(105);
  //move(5);
  clamp.set(false);
  //globalFdeg = 17;
  //wait(.2,sec);
  move(-27);
  //wait(1.5,sec);
  //driveStop();
  //wait(.2,sec);
  ringStopper = false;
  move(23.5);
  turn(283);
  move(-27);
  wait(.2,sec);
  clamp.set(true);
  IntakeIn();
  turn(220);
  
  move(34);
  turn(135);
  wait(.5,sec);
  IntakeStop();
  move(20);
  IntakeIn();
  move(17);
  wait(.4,sec);
  IntakeStop();
  setMovePowerLimit(70);
  moveAsync(62);
  waitUntil(24);
  IntakeIn();
  waitUntil(isMoving() == false);
  turn(180);
  move(10);
  wait(.4,sec); 
  turn(59);
  move(11.5);
  turn(-30);
  wait(.3,sec);
  IntakeStop();
  clamp.set(false);
  move(-11);
  move(6);
  turn(-19);
  ringStopper = true;
  IntakeIn();
  move(26.9);
  turn(45);
  move(-28);
  wait(.1,sec);
  clamp.set(true);
  ringStopper = false;
  wait(.1,sec);
  turn(23);
  IntakeInBig();
  move(49.5);
  turn(90);
  //globalFdeg = 17;
  setMovePowerLimit(80);
  //wait(.2,sec);
  antiJam = false;
  moveAsync(19);
  wait(1.3,sec);
  IntakeStop();
  //globalFdeg = 170;
  //wait(.2,sec);
  printf("%lf \n", AutoTime.time(sec)); 
  setMovePowerLimit(100);
  move(-13);
  //globalFdeg = 0;
  turn(2);
  IntakeIn();
  antiJam = true;
  move(22);
  turn(-90);
  move(23.5);
  turn(46);
  extendDoinkRight();
  move(34);
  wait(.5,sec);
  IntakeStop();
  moveAsync(17); //steve & umer said to make this a move async
  turn(-165); // last corner placement turn previously 230
  clamp.set(false);
  moveAsync(-8);
  wait(.8,sec);
  driveStop();
  // turn(225);
  //clamp.set(false);
  // moveAsync(-30);
  // wait(.3,sec);
  // move(15);
  // turn(50);
  //globalFdeg = (195);
  //moveAsync(-40);
  waitUntilDistance(30);
  setMovePowerLimit(20);
  waitUntil(isMoving() == false);
  // turn(0);
  // move(10);
  // swingLeft(93);
  // move(12);
  // turn(190);
  // moveAsync(-10);
  //clamp.set(false); */
 printf("%lf \n", AutoTime.time(sec)); 
Controller1.Screen.print(AutoTime.time(sec));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// END OF AUTONOMOUS ROUTES ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() 
{
  vexcodeInit();
  whenStarted();
  
  Controller1.ButtonR1.pressed(IntakeIn);
  Controller1.ButtonL1.pressed(IntakeOut);
  Controller1.ButtonL1.released(IntakeStop);
  Controller1.ButtonR1.released(IntakeStop);
  
  Controller1.ButtonRight.pressed(extendDoinkRight);
  Controller1.ButtonRight.released(retractDoinkRight);
  Controller1.ButtonY.pressed(Clamping);
  Controller1.ButtonX.pressed(DescoreArm);
  Controller1.ButtonA.pressed(extendDoinkLeft);
  Controller1.ButtonA.released(retractDoinkLeft);
  Controller1.ButtonUp.pressed(extendGoalRush);
  Controller1.ButtonUp.released(retractGoalRush);

  Controller1.ButtonR2.pressed(lifting);
  Controller1.ButtonL2.pressed(ToggleLift);
  Controller1.ButtonR2.released(liftingstop);
  
  Controller1.ButtonLeft.pressed(liftActivate);
  Controller1.ButtonB.pressed(liftAllyPos);
  Controller1.ButtonDown.pressed(liftHighHold);

  Controller1.ButtonB.pressed(Hanging);
  Competition.drivercontrol(Driver);
  Competition.autonomous(redGoalRushElims);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// AUTO SLOTS AND NAMES ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*

QUALIFICATION MATCHES
Slot One blueSoloAWP            DSAHS 
Slot Two redSoloAWP
Slot Three bluePosHalfAWP       DSAHS
Slot Four redPosHalfAWP         DSAHS
Slot Five blueNegHalfAWP        DSAHS
Slot Six redNegHalfAWP          DSAHS
Slot Seven blueGoalRushQuals
Slot Eight redGoalRushQuals

/////////////////////////////////////////// ELIMINATION MATCHES ///////////////////////////////////////////////////////////////////////////

Slot One blueGoalRushElims
Slot Two redGoalRushElims
Slot Three blueRingRushElimsGoal                  fills goal
Slot Four redRingRushElimsGoal                    fills goal
Slot Five blueRingRushElimsAlliance               gets alliance stake       
Slot Six redRingRushElimsAlliance                 gets alliance stake       DSAHS
Slot Seven bluePosElimsFillGoal
Slot Eight redPosElimsFillGoal

*/