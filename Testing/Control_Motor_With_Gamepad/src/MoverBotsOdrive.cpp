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
    //Make odrive objects
    ODriveArduino odrive_1(ser_1);
    ODriveArduino odrive_2(ser_2);
    o1 = &odrive_1;
    o2 = &odrive_2;
    //Setup serial
    ser_1.begin(ODRIVE_BAUDRATE);
    ser_2.begin(ODRIVE_BAUDRATE);
    //Config
    ODriveConfig config1(ser_1, axis_1);
    ODriveConfig config2(ser_2, axis_2);
    config1.controller_config_enable_vel_limit(true);
    config2.controller_config_enable_vel_limit(true);
    config1.controller_config_vel_limit(10.0);
    config2.controller_config_vel_limit(10.0);
    //Calibrate
    odrive_1.run_state(axis_1, AXIS_STATE_MOTOR_CALIBRATION, true);
    odrive_2.run_state(axis_2, AXIS_STATE_MOTOR_CALIBRATION, true);
    odrive_1.run_state(axis_1, AXIS_STATE_ENCODER_OFFSET_CALIBRATION, true, 25.0f);
    odrive_2.run_state(axis_2, AXIS_STATE_ENCODER_OFFSET_CALIBRATION, true, 25.0f);
    odrive_1.run_state(axis_1, AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    odrive_2.run_state(axis_2, AXIS_STATE_CLOSED_LOOP_CONTROL, false);
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