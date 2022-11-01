///
///@file autonomous.c
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-22
///

#include "../config.h"

static void set_drive_units(enum motor_encoder_units_e units)
{
    motor_set_encoder_units(PORTS.drive.right.front, units);
    motor_set_encoder_units(PORTS.drive.right.back, units);
    motor_set_encoder_units(PORTS.drive.left.front, units);
    motor_set_encoder_units(PORTS.drive.left.back, units);
}

static void set_drive(int8 volts)
{
    motor_move(PORTS.drive.left.front,-volts);
    motor_move(PORTS.drive.left.back, volts);

    motor_move(PORTS.drive.right.front, volts);
    motor_move(PORTS.drive.right.back, -volts);
}

static void move_spaces(int32 spacec, int32 speed)
{
    motor_move_relative(PORTS.drive.left.front, spacec,   -speed);
    motor_move_relative(PORTS.drive.left.back,  spacec,   speed);

    motor_move_relative(PORTS.drive.right.front,spacec,   speed);
    motor_move_relative(PORTS.drive.right.back, spacec,  -speed);

    delay(2000);

    set_drive(0);
}

static uint16 clamp_angle(int64 val)
{
    return val > 360 || val < 0 ? val % 360 : val;
}

static uint16 get_rotation()
{
    struct DrivetrainSide left = PORTS.drive.left, right = PORTS.drive.right;

    return clamp_angle((((motor_get_position(left.front) + motor_get_position(left.back)) / 2 - (motor_get_position(right.front) + motor_get_position(right.back)) / 2) / ROBOT.dimensions.x) * (M_PI / 180));
}

static void rotate(uint16 deg)
{
    Port_t  left_f  = PORTS.drive.left.front,
            left_b  = PORTS.drive.left.back,
            right_f = PORTS.drive.right.front,
            right_b = PORTS.drive.right.back;

    motor_move(left_f, -127);
    motor_move(left_b, 127);
    motor_move(right_f, -127);
    motor_move(right_b, 127);

    uint16 current = get_rotation();
    while (deg - current > -1 || deg - current < 1) {
        print("%llf\n", current = get_rotation());
        delay(10);
    }

    set_drive(0);
}

void drive_for(uint64 millis, int8 voltage)
{
    set_drive(voltage);
    delay(millis);
    set_drive(0);
}

///
/// It is 0:41 as when I start this, lets see when I end
/// Waheguruji Ka Khalsa Waheguruji Ki Fateh
///
void autonomous(void)
{
    set_drive_units(E_MOTOR_ENCODER_DEGREES);

    drive_for(100, 127);

    motor_move(PORTS.intake[1], CONFIG.flipper_strength);
    delay(1000);
    motor_move(PORTS.intake[1], 0);

    drive_for(100, -127);
//    rotate(90);

    motor_move(PORTS.flywheel[0], (int8)(127.0 * 0.75));
    motor_move(PORTS.flywheel[1], (int8)(-127.0 * 0.75));

    yield;

    println(2, "Pneumatics - On");
    adi_digital_write(PORTS.pneumatics, off);
    delay(ROBOT.pneumatic_wait);
    adi_digital_write(PORTS.pneumatics, on);
    println(2, "Pneumatics - Off");
    delay(ROBOT.pneumatic_wait);
    println(2, "Pneumatics - On");
    adi_digital_write(PORTS.pneumatics, off);
    delay(ROBOT.pneumatic_wait);
    adi_digital_write(PORTS.pneumatics, on);
    println(2, "Pneumatics - Off");
}
