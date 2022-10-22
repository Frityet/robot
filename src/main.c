#include "pros/screen.h"
#include "../config.h"

#include <stdnoreturn.h>
#include "controller.h"

void disabled() {}
void competition_initialize(){}
void autonomous() {}
void initialize() {}

noreturn void opcontrol()
{
    adi_port_set_config(PORTS.pneumatics[0], E_ADI_ANALOG_OUT);
    adi_port_set_config(PORTS.pneumatics[1], E_ADI_ANALOG_OUT);

    collect_controller_input(&(struct ControllerConfig) {
        .port = PORTS.controller,
        .actions = {
            .analog = {
                .actions = {
                    [ControllerStick_LEFT_Y] = $(void, (int32 val), {
//                        controller_print(E_CONTROLLER_MASTER, 0, 0, "LValue: %d", val);
                        motor_move(PORTS.drive.left.front, -val);
                        motor_move(PORTS.drive.left.back, val);
                    }),

                    [ControllerStick_RIGHT_Y] = $(void, (int32 val), {
//                        controller_print(E_CONTROLLER_MASTER, 1, 0, "RValue: %d", val);
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
                                adi_digital_write(PORTS.pneumatics[0], true);
                                adi_digital_write(PORTS.pneumatics[1], true);
                                delay(1000);
                                adi_digital_write(PORTS.pneumatics[1], false);
                                adi_digital_write(PORTS.pneumatics[0], false);
                            }),
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
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at MAX                  ");
                            }),
                        },

                        [ControllerBumper_R1] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127 / 2);
                                motor_move(PORTS.flywheel[1], -127 / 2);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at half                 ");
                            }),
                        },

                        [ControllerBumper_R2] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 127);
                                motor_move(PORTS.flywheel[1], -127 / 2);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at quarter             ");
                            }),
                        },

                        [ControllerBumper_L2] = {
                            .on = $(void, (), {
                                motor_move(PORTS.flywheel[0], 0);
                                motor_move(PORTS.flywheel[1], -0);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher off                     ");
                            }),
                        },
                    }
                }
            }
        }
    });

    while (true);
}
