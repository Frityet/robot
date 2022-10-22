///
///@file autonomous.c
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-22
///

#include "../config.h"

typedef int32 Rotation_t;

static Rotation_t clamp_rotation(Rotation_t val)
{
    return val > 360 ? val - 360 : val < 0 ? clamp_rotation(-val) : val;
}

static Rotation_t rotate(Rotation_t val, Rotation_t amount)
{
    val = clamp_rotation(val + amount);

//    motor_move_relative(PORTS.drive.left.front, 10);
//    start_motors(PORTS.drive.left.back, -10);

    return val;
}

///
/// It is 0:41 as when I start this, lets see when I end
/// Waheguruji Ka Khalsa Waheguruji Ki Fateh
///
void autonomous(void)
{
    lcd_print(0, "Waheguru Waheguru Waheguru Ji,");
    lcd_print(1, "Satnam Satnam Satnam Ji");

    Rotation_t rotation = 0;


}
