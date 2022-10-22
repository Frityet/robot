///
///@file config.h
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-18
///

#pragma once

#include "src/common.h"

static const struct {
    Port_t controller, intake[2], flywheel[2];
    union {
        struct {
            struct {
                Port_t back, front;
            } right, left;
        };
        struct {
            Port_t right[2], left[2];
        } as_array;
    } drive;
    Port_t pneumatics[2];
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
    .pneumatics = { 'A', 'B' }
};

static const struct {
    Point_t size, tile_size;
    int8    flipper_strength;
} AUTONOMOUS = {
    .size = { (uint32)365.76, (uint32)365.76 },
    .tile_size = { (uint32)60.96, (uint32)60.96 },
    .flipper_strength = 32
};
