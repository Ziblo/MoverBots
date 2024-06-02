//MoverBotsOdrive
#include <HardwareSerial.h>
#include <ODriveArduino.h>

#define ODRIVE_SERIAL_1 Serial1 //pin 16 & 15
#define ODRIVE_SERIAL_2 Serial2 //pin 19 & 20
#define ODRIVE_AXIS_1 0
#define ODRIVE_AXIS_2 0
#define ODRIVE_BAUDRATE 115200
#define GEAR_RATIO 15 //I don't know that it's 15. This is a tentative value PLEASE UPDATE
#define WHEEL_RATIO 1 //wheel spins per one side full rotation
#define WHEEL_RADIUS 0.2
#define TWO_PI 6.28318530718

// Printing with stream operator helper functions
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

class ODriveConfig {
public:
    //There are a LOT of configuration options. 
    //https://docs.odriverobotics.com/v/latest/fibre_types/com_odriverobotics_ODrive.html#ODrive.Controller
    HardwareSerial* p_ser;
    int ax;
    ODriveConfig(HardwareSerial& serial, int axis){
        p_ser = &serial;
        ax = axis;
    }
    void controller_move_incremental(float displacement, bool from_input_pos){
        *p_ser << "w axis" << ax << ".controller.move_incremental " << displacement << " " << from_input_pos<< '\n';
    }
    void controller_config_enable_vel_limit(bool is_enabled){
        *p_ser << "w axis" << ax << ".controller.config.enable_vel_limit " << is_enabled << '\n';
    }
    void controller_config_vel_limit(float limit){
        *p_ser << "w axis" << ax << ".controller.config.vel_limit " << limit << '\n';
    }
    void controller_config_control_mode(uint8_t mode){
        //VOLTAGE_CONTROL= 0 (0x0)
        //TORQUE_CONTROL= 1 (0x1)
        //VELOCITY_CONTROL= 2 (0x2)
        //POSITION_CONTROL= 3 (0x3)
        *p_ser << "w axis" << ax << ".controller.config.control_mode " << mode << '\n';
    }
    void controller_config_vel_gain(float gain){
        //If in VELOCITY_CONTROL mode
        *p_ser << "w axis" << ax << ".controller.config.vel_gain " << gain << '\n';
    }
    void controller_config_vel_integrator_gain(float gain){
        //If in VELOCITY_CONTROL mode
        *p_ser << "w axis" << ax << ".controller.config.vel_integrator_gain " << gain << '\n';
    }
    void controller_config_vel_integrator_limit(float limit){
        //If in VELOCITY_CONTROL mode
        *p_ser << "w axis" << ax << ".controller.config.vel_integrator_limit " << limit << '\n';
    }
    void controller_config_vel_integrator_limit(float limit){
        //If in VELOCITY_CONTROL mode
        *p_ser << "w axis" << ax << ".controller.config.vel_integrator_limit " << limit << '\n';
    }
    void motor_config_current_lim(float limit){
        *p_ser << "w axis" << ax << ".motor.config.current_lim " << limit << '\n';
    }
};

class MOdrive {
public:
    HardwareSerial& ser_1 = ODRIVE_SERIAL_1;
    HardwareSerial& ser_2 = ODRIVE_SERIAL_2;
    int axis_1 = ODRIVE_AXIS_1;
    int axis_2 = ODRIVE_AXIS_2;
    ODriveArduino* o1;
    ODriveArduino* o2;
    MOdrive(){
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
        odrive_1.run_state(axis_1, AXIS_STATE_MOTOR_CALIBRATION, true);
        odrive_2.run_state(axis_2, AXIS_STATE_MOTOR_CALIBRATION, true);
        odrive_1.run_state(axis_1, AXIS_STATE_ENCODER_OFFSET_CALIBRATION, true, 25.0f);
        odrive_2.run_state(axis_2, AXIS_STATE_ENCODER_OFFSET_CALIBRATION, true, 25.0f);
        odrive_1.run_state(axis_1, AXIS_STATE_CLOSED_LOOP_CONTROL, false);
        odrive_2.run_state(axis_2, AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    }
    //Kinematics equations
    float get_angle_rad(){
        float diff = (o1->GetPosition(axis_1) - o2->GetPosition(axis_2))/2;
        //one turn will return a position of 1
        return TWO_PI * diff / GEAR_RATIO;
    }
    float get_speed(){
        float diff = (o1->GetVelocity(axis_1) - o2->GetVelocity(axis_2))/2;
        return TWO_PI * WHEEL_RADIUS * WHEEL_RATIO * diff / GEAR_RATIO;
    }
    float get_angular_speed(){
        float sum = (o1->GetVelocity(axis_1) + o2->GetVelocity(axis_2))/2;
        return TWO_PI * sum / GEAR_RATIO;
    }
    void set_angular_speed(float wpeed){
        set_speed_and_angular_speed(get_speed(), wpeed);
    }
    void set_speed(float speed){
        set_speed_and_angular_speed(speed, get_angular_speed());
    }
    void set_speed_and_angular_speed(float speed, float wpeed){
        o1->SetVelocity(axis_1, (wpeed + speed)/2);
        o2->SetVelocity(axis_2, (wpeed - speed)/2);
    }
};