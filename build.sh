#!/usr/bin/env bash

clang -std=c11 -lm -pthread timer.c task.c dummy.c thread_pool.c barrier.c -Wall -o antwork
