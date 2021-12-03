/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * AS IS BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * Copyright (c) 2020, OPEN AI LAB
 * Author: qtang@openailab.com
 */

#pragma once

#include "types.hpp"

#include "tengine/c_api.h"
#include "yolo_layer.hpp"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

static const char *class_names[] = {
    "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
    "hair drier", "toothbrush"};

static const unsigned char colors[81][3] = {
    {226, 255, 0},
    {56, 0, 255},
    {0, 94, 255},
    {0, 37, 255},
    {0, 255, 94},
    {255, 226, 0},
    {0, 18, 255},
    {255, 151, 0},
    {170, 0, 255},
    {0, 255, 56},
    {255, 0, 75},
    {0, 75, 255},
    {0, 255, 169},
    {255, 0, 207},
    {75, 255, 0},
    {207, 0, 255},
    {37, 0, 255},
    {0, 207, 255},
    {94, 0, 255},
    {0, 255, 113},
    {255, 18, 0},
    {255, 0, 56},
    {18, 0, 255},
    {0, 255, 226},
    {170, 255, 0},
    {255, 0, 245},
    {151, 255, 0},
    {132, 255, 0},
    {75, 0, 255},
    {151, 0, 255},
    {0, 151, 255},
    {132, 0, 255},
    {0, 255, 245},
    {255, 132, 0},
    {226, 0, 255},
    {255, 37, 0},
    {207, 255, 0},
    {0, 255, 207},
    {94, 255, 0},
    {0, 226, 255},
    {56, 255, 0},
    {255, 94, 0},
    {255, 113, 0},
    {0, 132, 255},
    {255, 0, 132},
    {255, 170, 0},
    {255, 0, 188},
    {113, 255, 0},
    {245, 0, 255},
    {113, 0, 255},
    {255, 188, 0},
    {0, 113, 255},
    {255, 0, 0},
    {0, 56, 255},
    {255, 0, 113},
    {0, 255, 188},
    {255, 0, 94},
    {255, 0, 18},
    {18, 255, 0},
    {0, 255, 132},
    {0, 188, 255},
    {0, 245, 255},
    {0, 169, 255},
    {37, 255, 0},
    {255, 0, 151},
    {188, 0, 255},
    {0, 255, 37},
    {0, 255, 0},
    {255, 0, 170},
    {255, 0, 37},
    {255, 75, 0},
    {0, 0, 255},
    {255, 207, 0},
    {255, 0, 226},
    {255, 245, 0},
    {188, 255, 0},
    {0, 255, 18},
    {0, 255, 75},
    {0, 255, 151},
    {255, 56, 0},
    {245, 255, 0}};

class YOLO
{
public:
    YOLO(const std::string &model, const int &w, const int &h, const std::array<float, 3> &scale, const std::array<float, 3> &bias);
    ~YOLO();
    int detect(const cv::Mat &image, std::vector<Object> &objects);

private:
    int init();
    void run_post(int image_width, int image_height, std::vector<Object> &boxes);

private:
    context_t context;
    graph_t graph;

    int width;
    int height;

    std::vector<uint8_t> input_uint8;
    std::vector<float> input_float;

    std::vector<std::vector<float>> output_float;

    std::vector<uint8_t> canvas;
    std::vector<uint8_t> canvas_permute;

    float in_scale;
    int in_zp;

    bool init_done;

    std::vector<float> out_scale;
    std::vector<int> out_zp;

private:
    std::array<float, 3> scale;
    std::array<float, 3> bias;
};
