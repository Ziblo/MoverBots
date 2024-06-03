#include "MoverBotsOdrive.h"

ODriveConfig::ODriveConfig(HardwareSerial& serial, int axis) {
    p_ser = &serial;
    ax = axis;
}

void ODriveConfig::controller_move_incremental(float displacement, bool from_input_pos) {
    *p_ser << "w axis" << ax << ".controller.move_incremental " << displacement << " " << from_input_pos << '\n';
}
void ODriveConfig::controller_config_enable_vel_limit(bool is_enabled) {
    *p_ser << "w axis" << ax << ".controller.config.enable_vel_limit " << is_enabled << '\n';
}
void ODriveConfig::controller_config_vel_limit(float limit) {
    *p_ser << "w axis" << ax << ".controller.config.vel_limit " << limit << '\n';
}
void ODriveConfig::controller_config_control_mode(uint8_t mode) {
    *p_ser << "w axis" << ax << ".controller.config.control_mode " << mode << '\n';
}
void ODriveConfig::controller_config_vel_gain(float gain) {
    *p_ser << "w axis" << ax << ".controller.config.vel_gain " << gain << '\n';
}
void ODriveConfig::controller_config_vel_integrator_gain(float gain) {
    *p_ser << "w axis" << ax << ".controller.config.vel_integrator_gain " << gain << '\n';
}
void ODriveConfig::controller_config_vel_integrator_limit(float limit) {
    *p_ser << "w axis" << ax << ".controller.config.vel_integrator_limit " << limit << '\n';
}
void ODriveConfig::motor_config_current_lim(float limit) {
    *p_ser << "w axis" << ax << ".motor.config.current_lim " << limit << '\n';
}

MOdrive::MOdrive(){
    axis_1 = ODRIVE_AXIS_1;
    axis_2 = ODRIVE_AXIS_2;
    //Make odrive objects
    o1 = new ODriveArduino(ODRIVE_SERIAL_1);
    o2 = new ODriveArduino(ODRIVE_SERIAL_2);
    // //Setup serial
    ODRIVE_SERIAL_1.begin(ODRIVE_BAUDRATE);
    ODRIVE_SERIAL_2.begin(ODRIVE_BAUDRATE);
}

MOdrive::~MOdrive() {
    // Clean up dynamically allocated memory to avoid memory leaks
    delete o1;
    delete o2;
}

void MOdrive::Init(){
    //Config
    ODriveConfig config1(ODRIVE_SERIAL_1, axis_1);
    ODriveConfig config2(ODRIVE_SERIAL_2, axis_2);
    config1.controller_config_enable_vel_limit(true);
    config2.controller_config_enable_vel_limit(true);
    config1.controller_config_vel_limit(10.0);
    config2.controller_config_vel_limit(10.0);
    //Calibrate
    if (!calibrate(o1, axis_1)) Serial << "Failed to calibrate ODrive 1.\n";
    if (!calibrate(o2, axis_2)) Serial << "Failed to calibrate ODrive 2.\n";
}

bool MOdrive::calibrate(ODriveArduino* o, int axis){
    Serial << "Axis " << axis << ": Requesting state " << AXIS_STATE_MOTOR_CALIBRATION << '\n';
    if(!o->run_state(axis, AXIS_STATE_MOTOR_CALIBRATION, true, 1.0f)) return false;
    Serial << "Axis " << axis << ": Requesting state " << AXIS_STATE_ENCODER_OFFSET_CALIBRATION << '\n';
    if(!o->run_state(axis, AXIS_STATE_ENCODER_OFFSET_CALIBRATION, true, 25.0f)) return false;
    Serial << "Axis " << axis << ": Requesting state " << AXIS_STATE_CLOSED_LOOP_CONTROL << '\n';
    if(!o->run_state(axis, AXIS_STATE_CLOSED_LOOP_CONTROL, false /*don't wait*/)) return false;
    return true;
}

float MOdrive::get_angle_rad() {
    float diff = (o1->GetPosition(axis_1) - o2->GetPosition(axis_2)) / 2;
    return TWO_PI * diff / GEAR_RATIO;
}

float MOdrive::get_speed() {
    float diff = (o1->GetVelocity(axis_1) - o2->GetVelocity(axis_2)) / 2;
    return TWO_PI * WHEEL_RADIUS * WHEEL_RATIO * diff / GEAR_RATIO;
}

float MOdrive::get_angular_speed() {
    float sum = (o1->GetVelocity(axis_1) + o2->GetVelocity(axis_2)) / 2;
    return TWO_PI * sum / GEAR_RATIO;
}

void MOdrive::set_angular_speed(float wpeed) {
    set_speed_and_angular_speed(get_speed(), wpeed);
}

void MOdrive::set_speed(float speed) {
    set_speed_and_angular_speed(speed, get_angular_speed());
}

void MOdrive::set_speed_and_angular_speed(float speed, float wpeed) {
    o1->SetVelocity(axis_1, (wpeed + speed) / 2);
    o2->SetVelocity(axis_2, (wpeed - speed) / 2);
}

void MOdrive::set_m1_speed(float speed){
    o1->SetVelocity(axis_1, speed);
}

void MOdrive::set_m2_speed(float speed){
    o2->SetVelocity(axis_2, speed);
}