#!/bin/sh
#!/usr/bin/env bash

g++ -std=c++11 -o game snake.cpp main.cpp -lncursesw
./game
