///
///@file controller.c
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-18
///

#include "controller.h"

#include <time.h>

#define var __auto_type

//Returns the object dereferenced if not null
#define NN(x) ({ \
    typedef __typeof__(*(x)) Func_t; \
    Func_t *default_fn = (Func_t *)$(int64_t, (){ return 0; }); \
    (x) == NULL ? default_fn : (x);\
})

//static void get_mutex_digital(struct Controller_DigitalActionGroup group[static 1], controller_digital_e_t offset)
//{
//    bool buttonstates[4] = {0};
//    int active_index = -1;
//    for (int i = 0; i < 4; i++)
//        buttonstates[i] = controller_get_digital(E_CONTROLLER_MASTER, offset + 1);
//
//    for (int i = 0, is_active = 0; i < 4; i++, is_active = group->active[i])
//        if (is_active) {
//            active_index = i;
//            break;
//        }
//
//    for (int i = 0; i < 4; i++) {
//        if (active_index == -1 && buttonstates[i] == on) {
//            group->active[i] = on;
//            return;
//        }
//
//        if (buttonstates[active_index]) group->active[active_index] = off;
//    }
//}


static bool get_digitals(struct Controller_DigitalActionGroup group[static 1], controller_digital_e_t offset)
{
    for (int i = 0; i < 4; i++)
        if (controller_get_digital(E_CONTROLLER_MASTER, offset + i))
            group->active[i] = !group->active[i];

    if (!ANY_ACTIVE(&group->active)) return false;

    for (int i = 0; i < 4; i++)
        NN(group->active[i] ? group->actions->on : group->actions->off)(&group->active);

    return true;
}

task_t collect_controller_input(struct ControllerConfig arg[static 1])
{
    return task_create($(void, (struct ControllerConfig controller[static 1]) {
        controller_clear_line(E_CONTROLLER_MASTER, 0);
        controller_print(E_CONTROLLER_MASTER, 0, 0, "Created task");

        while (true) {
            //First one is analog (sticks)
            var analog = &controller->actions.analog;
            memset(analog->active, 0, sizeof(bool [4]));
            int vals[4] = {0};

            for (int i = 0; i < 4; i++) {
                vals[i] = controller_get_analog(E_CONTROLLER_MASTER, i);
                if (vals[i]) analog->active[i] = on;
            }

            for (int i = 0; i < 4; i++) {
                NN(analog->actions[i])(&analog->active, vals[i]);
                vals[i] = 0;
            }


            struct Controller_DigitalActionGroup *digital_group = &controller->actions.digital[0];
            if (!get_digitals(digital_group,E_CONTROLLER_DIGITAL_L1))
                digital_group->on_all_off();

            digital_group = &controller->actions.digital[1];
            if (!get_digitals(digital_group, E_CONTROLLER_DIGITAL_UP))
                digital_group->on_all_off();

            digital_group = &controller->actions.digital[2];
            if (!get_digitals(digital_group, E_CONTROLLER_DIGITAL_X))
                digital_group->on_all_off();
        }
    }), arg, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Controller task");
}
