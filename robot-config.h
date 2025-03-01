using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor Drive1;
extern motor Drive2;
extern motor Drive3;
extern motor Drive4;
extern motor Drive5;
extern motor Drive6;
extern controller Controller1;
extern inertial InertialSensor;
extern digital_out clamp;
extern motor Intake;
extern digital_out doinkRight;
extern rotation Rotation12;
extern optical Optical;
extern digital_out hang;
extern rotation pod;
extern digital_out doinkLeft;
extern motor_group lift;
extern digital_out IntakeLift;
extern digital_out rushClamp;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );