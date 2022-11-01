#include "pros/screen.h"
#include "../config.h"

#include <stdnoreturn.h>
#include "controller.h"

void disabled() {}
void competition_initialize(){}

void initialize()
{
    lcd_initialize();

    //Initalise pneumatics, these must be digital out, and the value for "back" is `on` (1)
    if (adi_port_set_config(PORTS.pneumatics, E_ADI_DIGITAL_OUT) == PROS_ERR) println(0, "ERROR, %s", strerror(errno));
    if (adi_port_set_config(PORTS.launcher, E_ADI_DIGITAL_OUT) == PROS_ERR) println(0, "ERROR, %s", strerror(errno));

    adi_digital_write(PORTS.pneumatics, on);
    adi_digital_write(PORTS.launcher, on);

    lcd_print(0, "Waheguru Waheguru Waheguru Ji,");
    lcd_print(1, "Satnam Satnam Satnam Ji");

    yield;
}

noreturn void opcontrol()
{
    println(0, "Launcher - OFF");
    println(1, "Intake - OFF");
    println(2, "Pneumatics - Off");

    yield;

    //This function will not return!
    collect_controller_input(&(struct ControllerConfig) {

        .analog = {
            .actions = {
                [ControllerStick_LEFT_Y] = $(void, (int32 val), {
                    motor_move(PORTS.drive.left.front, -val);
                    motor_move(PORTS.drive.left.back, val);
                }),

                [ControllerStick_RIGHT_Y] = $(void, (int32 val), {
                    motor_move(PORTS.drive.right.front, val);
                    motor_move(PORTS.drive.right.back, -val);
                })
            }
        },

        .digital = {
            [ControllerActionGroup_BUMPERS] = {
                .actions = {
                    [ControllerBumper_L1] = {
                        .on = $(void, (), {
                            motor_move(PORTS.flywheel[0], (int8)(127.0 * (ROBOT.launcher_speeds.high / 100.0)));
                            motor_move(PORTS.flywheel[1], (int8)(-127.0 * (ROBOT.launcher_speeds.high / 100.0)));
                            println(0, "Launcher - HIGH");
                        }),
                    },

                    [ControllerBumper_R1] = {
                        .on = $(void, (), {
                            motor_move(PORTS.flywheel[0], (int8)(127.0 * (ROBOT.launcher_speeds.low / 100.0)));
                            motor_move(PORTS.flywheel[1], (int8)(-127.0 * (ROBOT.launcher_speeds.low / 100.0)));
                            println(0, "Launcher - LOW");
                        }),
                    },

                    [ControllerBumper_L2] = {
                        .on = $(void, (), {
                            motor_move(PORTS.flywheel[0], 0);
                            motor_move(PORTS.flywheel[1], -0);
                            println(0, "Launcher - OFF");
                        }),
                    },

                    [ControllerBumper_R2] = {
                        .hold = true,
                        .on = $(void, (), {
                            println(1, "Intake - ON");
                            motor_move(PORTS.intake[0], 127);
                            motor_move(PORTS.intake[1], 127);
                        }),
                        .off = $(void, (), {
                            println(1, "Intake - OFF");
                            motor_move(PORTS.intake[0], 0);
                            motor_move(PORTS.intake[1], 0);
                        })
                    },
                }
            },
            [ControllerActionGroup_BUTTONS] = {
                .actions = {
                    [ControllerButton_X] = {
                        .on = $(void, (), {
                            static bool running = false;
                            if (running) return;
                            running = true;

                            println(2, "Pneumatics - On");
                            adi_digital_write(PORTS.pneumatics, off);
                            delay(ROBOT.pneumatic_wait);
                            adi_digital_write(PORTS.pneumatics, on);
                            running = false;
                            println(2, "Pneumatics - Off");
                        }),
                    },
                }
            },
        }
    });
}
