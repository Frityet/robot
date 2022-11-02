///
///@file autonomous.c
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-22
///

#include "../config.h"

extern adi_gyro_t gyro;

static float getrotation()
{ return (float)(adi_gyro_get(gyro) / 10); }

static void rotate(int16 degr)
{
    float curr = 0;
    while ((curr = getrotation() / degr) > 5 || curr < -5) {
        motor_move(PORTS.drive.left.front, 64);
        motor_move(PORTS.drive.left.back, -64);

        motor_move(PORTS.drive.right.front, -64);
        motor_move(PORTS.drive.right.back, 64);
    }
}

void autonomous()
{
    lcd_print(1, "%f", getrotation());

    rotate(90);
}