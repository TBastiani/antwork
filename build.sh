#!/usr/bin/env bash

g++ -std=c++11 -lm -pthread timer.c task.cpp dummy.cpp thread_pool.cpp barrier.cpp -Wall -o antwork
