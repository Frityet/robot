///
///@file controller.h
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-18
///

#pragma once

#include "common.h"

#define ANY_ACTIVE(x) ((*(x))[0] || (*(x))[1] || (*(x))[2] || (*(x))[3])
#define OTHER_ACTIVE(x, i1, i2, i3) ((*(x))[i1] || (*(x))[i2] || (*(x))[i3])

enum ControllerStick {
    ControllerStick_LEFT_X ,
    ControllerStick_LEFT_Y ,
    ControllerStick_RIGHT_X,
    ControllerStick_RIGHT_Y
};

enum ControllerBumper {
    ControllerBumper_L1,
    ControllerBumper_L2,
    ControllerBumper_R1,
    ControllerBumper_R2
};

enum ControllerArrow {
    ControllerArrow_UP,
    ControllerArrow_DOWN,
    ControllerArrow_LEFT,
    ControllerArrow_RIGHT
};

enum ControllerButton {
    ControllerButton_X,
    ControllerButton_B,
    ControllerButton_Y,
    ControllerButton_A
};

enum ControllerActionGroupType {
    ControllerActionGroup_STICKS = -1,
    ControllerActionGroup_BUTTONS,
    ControllerActionGroup_ARROWS,
    ControllerActionGroup_BUMPERS,

    ControllerActionGroup_COUNT = 4,
};

typedef void AnalogControllerAction_f(int32 value);
typedef void DigitalControllerAction_f(void);

struct ControllerConfig {
    Port_t port;

    struct {
        //one analog (stick) action group
        struct {
            AnalogControllerAction_f *actions[ControllerActionGroup_COUNT];
        } analog;

        //three digital (button) action groups
        struct Controller_DigitalActionGroup {
            DigitalControllerAction_f *all_off;
            struct {
                DigitalControllerAction_f *on, *off;
            } actions[ControllerActionGroup_COUNT];
        } digital[3];
    } actions;
};

task_t collect_controller_input(struct ControllerConfig controller[static 1]);
