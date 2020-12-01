#!/bin/bash

gcc -Wall -pedantic -Werror -o main ./src/*.c -Iinclude -pthread -DDEBUG