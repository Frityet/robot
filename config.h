///
///@file config.h
///@author Amrit Bhogal
///@version 1.0.0 - Amrit Bhogal on 2022-10-18
///

#pragma once

#include "src/common.h"

static const struct {
    byte controller, intake[2], flywheel[2];
    struct {
        struct {
            byte back, front;
        } right, left;
    } drive;
    byte pneumatics;
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
    .pneumatics = 'B'
};
