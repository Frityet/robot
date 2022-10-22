#include "pros/screen.h"
#include "../config.h"

#include <stdnoreturn.h>
#include "controller.h"

void disabled() {}
void competition_initialize(){}
void autonomous() {}
void initialize() {}

static void rev_launcher(int8 force)
{
    motor_move(PORTS.flywheel[0], force);
    motor_move(PORTS.flywheel[1], -force);
}

noreturn void opcontrol()
{
    adi_port_set_config(PORTS.pneumatics, E_ADI_ANALOG_OUT);

//    controller_print(E_CONTROLLER_MASTER, 0, 0, "Start this POS");

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
                                adi_port_set_value(PORTS.pneumatics, 100);
                                delay(1000);
                                adi_port_set_value(PORTS.pneumatics, 0);
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
                                rev_launcher(127);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at MAX                  ");
                            }),
                        },

                        [ControllerBumper_L2] = {
                            .on = $(void, (), {
                                rev_launcher(0);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher off                     ");
                            }),
                        },

                        [ControllerBumper_R1] = {
                            .on = $(void, (), {
                                rev_launcher(127 / 2);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at half                 ");
                            }),
                        },

                        [ControllerBumper_R2] = {
                            .on = $(void, (), {
                                rev_launcher(127 / 4);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at quarter             ");
                            }),
                        },
                    }
                }
            }
        }
    });

    while (true);
}
