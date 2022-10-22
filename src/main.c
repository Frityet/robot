#include "pros/screen.h"
#include "../config.h"

#include <stdnoreturn.h>
#include "controller.h"

#define DEF(n) void n(){}
DEF(disabled);
DEF(competition_initialize);
DEF(autonomous);
DEF(initialize);

static void rev_launcher(int8 force)
{
    motor_move(PORTS.flywheel[0], force);
    motor_move(PORTS.flywheel[1], -force);
}

static void stop_launcher(time_t _)
{
    rev_launcher(0);
}



noreturn void opcontrol()
{
    adi_port_set_config(PORTS.pneumatics, true);

    controller_print(E_CONTROLLER_MASTER, 0, 0, "Start this POS");

    collect_controller_input(&(struct ControllerConfig) {
        .port = PORTS.controller,
        .actions = {
            .analog = {
                .actions = {
                    [ControllerStick_LEFT_Y] = $(void, (bool (*active)[4], int32 val) {
                        motor_move(PORTS.drive.left.front, -val);
                        motor_move(PORTS.drive.left.back, val);
                    }),

                    [ControllerStick_RIGHT_Y] = $(void, (bool (*active)[4], int32 val) {
                        motor_move(PORTS.drive.right.front, val);
                        motor_move(PORTS.drive.right.back, -val);
                    })
                }
            },

            .digital = {
                [ControllerActionGroup_BUTTONS] = {
                    .actions = {
                        [ControllerButton_A] = {
                            .on = $(void, (bool active[static 4]) {
//                                adi_port_set_value(PORTS.pneumatics, 100);
//                                delay(1000);
//                                adi_port_set_value(PORTS.pneumatics, 0);
                            }),
                        }
                    }
                },

                [ControllerActionGroup_ARROWS] = {

                },

                [ControllerActionGroup_BUMPERS] = {
                    .actions = {
                        [ControllerBumper_L1] = {
                            .on = $(void, (bool active[static 4]) {
                                rev_launcher(127);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at MAX             ");
                            }),
                        },

                        [ControllerBumper_L2] = {
                            .on = $(void, (bool active[static 4]) {
                                rev_launcher(0);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher off             ");
                            }),
                        },

                        [ControllerBumper_R1] = {
                            .on = $(void, (bool active[static 4]) {
                                rev_launcher(127 / 2);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at half              ");
                            }),
                        },

                        [ControllerBumper_R2] = {
                            .on = $(void, (bool active[static 4]) {
                                rev_launcher(127 / 4);
                                controller_print(E_CONTROLLER_MASTER, 0, 0, "Launcher at quarter          ");
                            }),
                        },
                    }
                }
            }
        }
    });

    while (true);
}

/**
        .buttons = {




            [ControllerButton_A].digital = {
                .on = $(void, (time_t elapsed) {
                    set_pneumatics(true);
                }),
                .off = $(void, (time_t elapsed) {
                    set_pneumatics(false);
                })
            },

            [ControllerButton_L2].digital = {
                .on = $(void, (time_t elapsed) {
                    motor_move(PORTS.intake[0], 127);
                    motor_move(PORTS.intake[1], -127);
                }),

                .off = $(void, (time_t elapsed) {
                    motor_move(PORTS.intake[0], 0);
                    motor_move(PORTS.intake[1], 0);
                })
            }
        }
 */
