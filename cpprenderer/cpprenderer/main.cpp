//
//  main.cpp
//  cpprenderer
//
//  Created by john on 2022/12/29.
//

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "TGAImage.h"
#include "ObjectModel.h"

ObjectModel *model = NULL;
const int canvas_width = 800;
const int canvas_height = 800;

// Reference: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
// points: (x - x0)/(y - y0) = (x1 - x0)/(y1 - y0)
void draw_line(int start_x, int start_y, int end_x, int end_y, TGAImage &image, TGAColor color) {
    float x0 = start_x, x1 = end_x, y0 = start_y, y1 = end_y;
    bool steep = abs(end_y - start_y) > abs(end_x - start_x);
    if (steep) {
        // 交换x与y轴
        x0 = start_y;
        x1 = end_y;
        y0 = start_x;
        y1 = end_x;
    }
    if (x0 > x1) {
        int tempx = x0;
        x0 = x1;
        x1 = tempx;

        int tempy = y0;
        y0 = y1;
        y1 = tempy;
    }
    float tangent = (abs(y1 - y0)/(x1 - x0));
    // 已变换为x1一定大于x0并且x长度一定大于y的线段
    // y = tanø * x
    // y = tanø * (x + 1)
    // y = tanø * x + tanø
    // 所以x增加1，y增加tanø
    float current_tangent = 0;
    int y = y0;
    int y_step = y1 > y0 ? 1 : -1;
    for (int x = x0; x < x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        current_tangent += tangent;
        if (current_tangent >= 0.5) {
            y += y_step;
            current_tangent -= abs(y_step);
        }
    }
}

void cpp_feature_test() {
}

int main(int argc, char** argv) {
    cpp_feature_test();

    if (argc == 2) {
        model = new ObjectModel(argv[1]);
    } else {
        model = new ObjectModel("cpprenderer/resources/african_head.obj");
    }
    TGAImage image(canvas_width, canvas_height, TGAImage::ColorFormat::RGB);
    TGAColor white_color(255, 255, 255, 255);
//    draw_line(10, 10, 90, 20, image, white_color);
//    draw_line(10, 10, 20, 90, image, white_color);
    for (int face_index = 0; face_index < model->faces_count(); face_index++) {
        std::vector<int> face_list = model->face(face_index);
        for (int line_index = 0; line_index < 3; line_index++) {
            int vertex_index0 = face_list[line_index];
            int vertex_index1 = face_list[(line_index + 1)%3];
            VectorFloat3 vertex0 = model->vertex(vertex_index0);
            VectorFloat3 vertex1 = model->vertex(vertex_index1);
            int x0 = (vertex0.x + 1) * canvas_width / 2.;
            int y0 = (vertex0.y + 1) * canvas_height / 2.;
            int x1 = (vertex1.x + 1) * canvas_width / 2.;
            int y1 = (vertex1.y + 1) * canvas_height / 2.;
            draw_line(x0, y0, x1, y1, image, white_color);
        }
    }
    image.flip_vertically();
    image.write_tga_file("test.tga", false);
    return 0;
}
