#ifndef MOVERBOTSODRIVE_H
#define MOVERBOTSODRIVE_H

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

// Printing with stream operator helper functions
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

class ODriveConfig {
public:
    HardwareSerial* p_ser;
    int ax;
    ODriveConfig(HardwareSerial& serial, int axis);
    void controller_move_incremental(float displacement, bool from_input_pos);
    void controller_config_enable_vel_limit(bool is_enabled);
    void controller_config_vel_limit(float limit);
    void controller_config_control_mode(uint8_t mode);
    void controller_config_vel_gain(float gain);
    void controller_config_vel_integrator_gain(float gain);
    void controller_config_vel_integrator_limit(float limit);
    void motor_config_current_lim(float limit);
};

class MOdrive {
public:
    HardwareSerial& ser_1 = ODRIVE_SERIAL_1;
    HardwareSerial& ser_2 = ODRIVE_SERIAL_2;
    int axis_1;
    int axis_2;
    ODriveArduino* o1;
    ODriveArduino* o2;
    MOdrive();
    float get_angle_rad();
    float get_speed();
    float get_angular_speed();
    void set_angular_speed(float wpeed);
    void set_speed(float speed);
    void set_speed_and_angular_speed(float speed, float wpeed);
    void set_m1_speed(float speed);
    void set_m2_speed(float speed);
};

#endif // MOVERBOTSODRIVE_H
