#include "pros/screen.h"
#include "../config.h"

#include <stdnoreturn.h>
#include "controller.h"

void disabled() {}
void competition_initialize(){}

void initialize()
{
    lcd_initialize();

    adi_port_set_config(PORTS.pneumatics[0], E_ADI_ANALOG_OUT);
    adi_port_set_config(PORTS.pneumatics[1], E_ADI_ANALOG_OUT);
}

noreturn void opcontrol()
{
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
                        [ControllerButton_A] = {
                            .on = $(void, (), {
                                print("Activating pneumatics");
                                adi_digital_write(PORTS.pneumatics[0], true);
                                adi_digital_write(PORTS.pneumatics[1], true);
                                print("Pneumatics - Activated");
                                delay(1000);
                                print("Deactivating pneumatics");
                                adi_digital_write(PORTS.pneumatics[1], false);
                                adi_digital_write(PORTS.pneumatics[0], false);
                                print("Pneumatics complete");
                            }),
                        },
                        [ControllerButton_B] = {
                            .on = $(void, (), {
                                motor_move(PORTS.intake[0], 127);
                                motor_move(PORTS.intake[1], -127);
                            }),
                        },

                        [ControllerButton_Y] = {
                            .on = $(void, (), {
                                motor_move(PORTS.intake[0], 0);
                                motor_move(PORTS.intake[1], -0);
                            })
                        }
                    }
                },

                [ControllerActionGroup_ARROWS] = {

                },

                [ControllerActionGroup_BUMPERS] = {
                    .actions = {
                        [ControllerBumper_L1] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127);
                                motor_move(PORTS.flywheel[1], -127);
                                print("Launcher at MAX");
                            }),
                        },

                        [ControllerBumper_R1] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127 / 2);
                                motor_move(PORTS.flywheel[1], -127 / 2);
                                print("Launcher at half!");
                            }),
                        },

                        [ControllerBumper_R2] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127);
                                motor_move(PORTS.flywheel[1], -127 / 4);
                                print("Launcher at quarter!");
                            }),
                        },

                        [ControllerBumper_L2] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 0);
                                motor_move(PORTS.flywheel[1], -0);
                                print("Launcher off!");
                            }),
                        },
                    }
                }
            }
        }
    });

    while (true);
}
