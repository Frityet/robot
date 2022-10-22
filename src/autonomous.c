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
    int8 rot = 127;

    Port_t  left_f  = PORTS.drive.left.front,
            left_b  = PORTS.drive.left.back,
            right_f = PORTS.drive.right.front,
            right_b = PORTS.drive.right.back;

    motor_move(left_f, -rot);
    motor_move(left_b, rot);
    motor_move(right_f, -rot);
    motor_move(right_b, rot);

    uint16 current = get_rotation();
    while (deg - current > 0 || deg - current < 1) {
        print("%d\n", current = get_rotation());
        delay(10);
    }

    set_drive(0);


}

///
/// It is 0:41 as when I start this, lets see when I end
/// Waheguruji Ka Khalsa Waheguruji Ki Fateh
///
void autonomous(void)
{
    set_drive_units(E_MOTOR_ENCODER_DEGREES);

    printf("Moving flipper");
    motor_move(PORTS.intake[0], CONFIG.flipper_strength);
    delay(1000);
    motor_move(PORTS.intake[0], 0);

    rotate(45);

    motor_move(PORTS.flywheel[0], 127 / 2);
    motor_move(PORTS.flywheel[1], -127 / 2);
    delay(1000);

    println(2, "Pneumatics - On");
    adi_digital_write(PORTS.pneumatics, true);
    delay(1000);
    adi_digital_write(PORTS.pneumatics, false);
    println(2, "Pneumatics - Off");

    println(2, "Pneumatics - On");
    adi_digital_write(PORTS.pneumatics, true);
    delay(1000);
    adi_digital_write(PORTS.pneumatics, false);
    println(2, "Pneumatics - Off");
}
