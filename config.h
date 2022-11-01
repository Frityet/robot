///
///@file config.h
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-18
///

#pragma once

///All measurements will be in CENTIMETRES because the METRIC SYSTEM is what GOD USES

#include "src/common.h"

#include "math.h"

static const struct {
    Port_t controller, intake[2], flywheel[2];
    union {
        struct {
            struct DrivetrainSide {
                Port_t back, front;
            } right, left;
        };
        struct {
            Port_t right[2], left[2];
        } as_array;
    } drive;
    Port_t pneumatics, launcher;
} PORTS = {
    .controller = 10,
    .intake = { 7, 8 },
    .flywheel = { 9, 5 },
    .drive = {
        .right = {
            .back = 1,
            .front = 2
        },
        .left = {
            .back = 3,
            .front = 4
        }
    },
    .pneumatics = 'B',
    .launcher = 'A'
};

struct RotationPoint {
    uint32 time;
    uint16 rotation;
};

static uint32 get_delay_woodenfloor(uint16 x)
{
    return (0.000108 * pow(x, 2)) + (0.102 * x) - 3.64;
}

static const struct {
    struct {
        uint32 tile_size, tile_count;
    } field;
    int8    flipper_strength;
    uint32  (*get_delay)(uint16 degrees);
    struct RotationPoint time_to_rotate[16];
} CONFIG = {
    .field = {
.96,
        .tile_count = 6,
    },

    .get_delay = get_delay_woodenfloor,

    .flipper_strength = (int8)(127.0 / 2.5),
    .time_to_rotate = {
        {
            .time = 225,
            .rotation = 23
        },
        {
            .time = 240,
            .rotation = 28,
        },
        {
            .time = 245,
            .rotation = 30
        },
        {
            .time = 490,
            .rotation = 90 //67 before
        },
        {
            .time = 980,
            .rotation = 200
        },
        {
            .time = 1050,
            .rotation = 147,
        },
    }
};

static const struct {
    uint32 wheel_radius;
    Point_t dimensions;
    uint64 pneumatic_wait;
    struct {
        uint8 high, low;
    } launcher_speeds;
    uint8 intake_speed;
} ROBOT = {
   .wheel_radius = 4,
   .dimensions = { 18, 25.5 },
   .pneumatic_wait = 128,
   .launcher_speeds = {
       .high = 80,
       .low =  60
   },
   .intake_speed = (int8)(127.0 / 2.0)
};
