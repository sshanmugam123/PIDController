#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor Drive1 = motor(PORT19, ratio6_1, true);
motor Drive2 = motor(PORT20, ratio6_1, true);
motor Drive3 = motor(PORT18, ratio6_1, true);
motor Drive4 = motor(PORT17, ratio6_1, false);
motor Drive5 = motor(PORT16, ratio6_1, false);
motor Drive6 = motor(PORT15, ratio6_1, false);
controller Controller1 = controller(primary);
inertial InertialSensor = inertial(PORT14);
digital_out clamp = digital_out(Brain.ThreeWirePort.A);
motor Intake = motor(PORT3, ratio6_1, false);
digital_out doinkRight = digital_out(Brain.ThreeWirePort.B);
rotation Rotation12 = rotation(PORT12, false);
optical Optical = optical(PORT4);
digital_out hang = digital_out(Brain.ThreeWirePort.G);
rotation pod = rotation(PORT21, false);
digital_out doinkLeft = digital_out(Brain.ThreeWirePort.C);
motor liftMotorA = motor(PORT2, ratio18_1, false);
motor liftMotorB = motor(PORT13, ratio18_1, true);
motor_group lift = motor_group(liftMotorA, liftMotorB);
digital_out IntakeLift = digital_out(Brain.ThreeWirePort.D);
digital_out rushClamp = digital_out(Brain.ThreeWirePort.E);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}