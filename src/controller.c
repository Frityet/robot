///
///@file controller.c
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-18
///

#include "controller.h"

#include <time.h>

//Returns the object dereferenced if not null
#define NN(x) ({ \
    typedef __typeof__(*(x)) Func_t; \
    Func_t *default_fn = (Func_t *)$(int64_t, (), { return 0; }); \
    (x) == NULL ? default_fn : (x);\
})

/// \brief Checks if each button in the group is pressed, calling the on function if true, and indicating that at least 1 button was action for the return value
/// \param group Group to get buttons for
/// \param offset Start value of the group in the \refitem controller_digital_e_t enum
/// \return
static bool get_digitals(struct Controller_DigitalActionGroup group[static 1], controller_digital_e_t offset)
{
    bool any_active = false;

    for (int32 i = 0; i < 4; i++)
        if (controller_get_digital_new_press(E_CONTROLLER_MASTER, offset + i)) {
            any_active = true;
            NN(group->actions[i].on)();
        } else NN(group->actions[i].off)();

    return any_active;
}

noreturn void collect_controller_input(struct ControllerConfig arg[static 1])
{
    //TODO: Run analog on main thread, have a unique thread for each digital group
    task_create((void *)$(void, (struct ControllerConfig controller[static 1]), {
        while (true) {
            struct Controller_DigitalActionGroup *dgroup = &controller->digital[ControllerActionGroup_BUMPERS];
            if (!get_digitals(dgroup, E_CONTROLLER_DIGITAL_L1))
                NN(dgroup->all_off)();

            dgroup = &controller->digital[ControllerActionGroup_ARROWS];
            if (!get_digitals(dgroup, E_CONTROLLER_DIGITAL_UP))
                NN(dgroup->all_off)();

            dgroup = &controller->digital[ControllerActionGroup_BUTTONS];
            if (!get_digitals(dgroup, E_CONTROLLER_DIGITAL_X))
                NN(dgroup->all_off)();
        }
    }), arg, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Controller task");

    while (true)
    for (int32 i = 0; i < 4; i++)
        NN(arg->analog.actions[i])(controller_get_analog(E_CONTROLLER_MASTER, i));
}
