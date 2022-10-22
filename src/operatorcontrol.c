#include "pros/screen.h"
#include "../config.h"

#include <stdnoreturn.h>
#include "controller.h"

void disabled() {}
void competition_initialize(){}

void initialize()
{
    lcd_initialize();

    if (adi_port_set_config(PORTS.pneumatics, E_ADI_ANALOG_OUT) == PROS_ERR) println(0, "ERROR, %s", strerror(errno));
    if (adi_port_set_config(PORTS.launcher, E_ADI_ANALOG_OUT) == PROS_ERR) println(0, "ERROR, %s", strerror(errno));

//    adi_digital_write(PORTS.pneumatics, on);
//    adi_digital_write(PORTS.launcher, on);
}

noreturn void opcontrol()
{
//    println(0, "Launcher   - Off");
//    println(1, "Intake     - Off");
//    println(2, "Pneumatics - Off");

    collect_controller_input(&(struct ControllerConfig) {
        .port = PORTS.controller,
        .actions = {
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
                [ControllerActionGroup_BUTTONS] = {
                    .actions = {
                        [ControllerButton_X] = {
                            .on = $(void, (), {
//                                println(2, "Pneumatics - On");
//                                adi_digital_write(PORTS.pneumatics, on);
//                                delay(1000);
//                                adi_digital_write(PORTS.pneumatics, off);
//                                println(2, "Pneumatics - Off");
                            }),
                        },
                        [ControllerButton_B] = {
                            .on = $(void, (), {
                                println(1, "Intake - On");
                                motor_move(PORTS.intake[0], 127);
                                motor_move(PORTS.intake[1], 127);
                            }),
                        },

                        [ControllerButton_Y] = {
                            .on = $(void, (), {
                                println(1, "Intake - Off");
                                motor_move(PORTS.intake[0], 0);
                                motor_move(PORTS.intake[1], -0);
                            })
                        }
                    }
                },

                [ControllerActionGroup_ARROWS] = {
                    .actions = {
                        [ControllerArrow_UP] = {
                            .on = $(void, (), {
//                                println(2, "Nuts - On");
//                                adi_digital_write(PORTS.launcher, on);
//                                delay(1000);
//                                adi_digital_write(PORTS.launcher, off);
//                                println(2, "Nuts - Off");
                            })
                        }
                    }
                },

                [ControllerActionGroup_BUMPERS] = {
                    .actions = {
                        [ControllerBumper_L1] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127);
                                motor_move(PORTS.flywheel[1], -127);
                                println(0, "Launcher - Max");
                            }),
                        },

                        [ControllerBumper_R1] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127 / 2);
                                motor_move(PORTS.flywheel[1], -127 / 2);
                                println(0, "Launcher - Half");
                            }),
                        },

                        [ControllerBumper_R2] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127);
                                motor_move(PORTS.flywheel[1], -127 / 4);
                                println(0, "Launcher - Quarter");
                            }),
                        },

                        [ControllerBumper_L2] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 0);
                                motor_move(PORTS.flywheel[1], -0);
                                println(0, "Launcher - Off");
                            }),
                        },
                    }
                }
            }
        }
    });
}
