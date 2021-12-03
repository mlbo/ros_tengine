#!/bin/bash

# catkin_make

# 方便在VSCode中进行ROS函数跳转
catkin_make -DCMAKE_EXPORT_COMPILE_COMMANDS=Yes

source devel/setup.bash