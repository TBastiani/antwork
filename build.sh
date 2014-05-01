#!/usr/bin/env bash

clang -std=c99 -lm timer.c task.c dummy.c -Wall -o antwork
