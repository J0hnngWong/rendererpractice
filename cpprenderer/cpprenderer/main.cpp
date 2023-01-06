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
TGAColor white_color(255, 255, 255, 255);
TGAColor red_color(255, 0, 0, 255);
TGAColor green_color(0, 255, 0, 255);
TGAColor blue_color(0, 0, 255, 255);

// Lesson 1
//const int canvas_width = 800;
//const int canvas_height = 800;

// Lesson 2
const int canvas_width = 200;
const int canvas_height = 200;

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

void draw_line(VectorInt2 start_point, VectorInt2 end_point, TGAImage &image, TGAColor color) {
    draw_line(start_point.x, start_point.y, end_point.x, end_point.y, image, color);
}

bool cross_direction_same(VectorInt2 *vertices, VectorInt2 &point) {
    VectorFloat3 point_a = VectorFloat3(vertices[0].x, vertices[0].y, 0);
    VectorFloat3 point_b = VectorFloat3(vertices[1].x, vertices[1].y, 0);
    VectorFloat3 point_c = VectorFloat3(vertices[2].x, vertices[2].y, 0);
    VectorFloat3 point_p = VectorFloat3(point.x, point.y, 0);
    
    VectorFloat3 vector_ab = point_b - point_a;
    VectorFloat3 vector_ap = point_p - point_a;
    
    VectorFloat3 vector_bc = point_c - point_b;
    VectorFloat3 vector_bp = point_p - point_b;
    
    VectorFloat3 vector_ca = point_a - point_c;
    VectorFloat3 vector_cp = point_p - point_c;
    
    float ap_cross = vector_ap.cross_product(vector_ab).z;
    float bp_cross = vector_bp.cross_product(vector_bc).z;
    float cp_cross = vector_cp.cross_product(vector_ca).z;
    
    return (ap_cross > 0 && bp_cross > 0 && cp_cross > 0) || (ap_cross < 0 && bp_cross < 0 && cp_cross < 0);
}

void draw_triangle_line(VectorInt2 v0, VectorInt2 v1, VectorInt2 v2, TGAImage &image, TGAColor stroke_color) {
    draw_line(v0, v1, image, stroke_color);
    draw_line(v1, v2, image, stroke_color);
    draw_line(v2, v0, image, stroke_color);
}

void draw_triangle_fill(VectorInt2 v0, VectorInt2 v1, VectorInt2 v2, TGAImage &image, TGAColor fill_color) {
    int upper_x = std::max(std::max(v0.x, v1.x), v2.x);
    int upper_y = std::max(std::max(v0.y, v1.y), v2.y);
    int lower_x = std::min(std::min(v0.x, v1.x), v2.x);
    int lower_y = std::min(std::min(v0.y, v1.y), v2.y);
    
    VectorInt2 vector_p = VectorInt2();
    for (vector_p.x = lower_x; vector_p.x < upper_x; vector_p.x++) {
        for (vector_p.y = lower_y; vector_p.y < upper_y; vector_p.y++) {
            VectorInt2 triangle_vertices[3] = { v0, v1, v2 };
            if (cross_direction_same(triangle_vertices, vector_p)) {
                image.set(vector_p.x, vector_p.y, fill_color);
            }
        }
    }
}

void draw_vertices_of_an_object(const char *file_path) {
    model = new ObjectModel(file_path);
    TGAImage image(canvas_width, canvas_height, TGAImage::ColorFormat::RGB);
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
}

void lesson1_final(int argc, char** argv) {
    const char *file_path;
    if (argc == 2) {
        file_path = argv[1];
    } else {
        file_path = "cpprenderer/resources/african_head.obj";
    }
    draw_vertices_of_an_object(file_path);
}

/*
 Reference:
 https://en.wikipedia.org/wiki/Barycentric_coordinate_system
 https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
*/
void lesson2_final(int argc, char** argv) {
    TGAImage image(canvas_width, canvas_height, TGAImage::ColorFormat::RGB);
    
    VectorInt2 v0[3] = { VectorInt2(10, 70), VectorInt2(50, 160), VectorInt2(70, 80) };
    VectorInt2 v1[3] = { VectorInt2(180, 50), VectorInt2(150, 1), VectorInt2(70, 180) };
    VectorInt2 v2[3] = { VectorInt2(180, 150), VectorInt2(120, 160), VectorInt2(130, 180) };
    
//    draw_triangle_line(v0[0], v0[1], v0[2], image, red_color);
//    draw_triangle_line(v1[0], v1[1], v1[2], image, white_color);
//    draw_triangle_line(v2[0], v2[1], v2[2], image, green_color);
    
    draw_triangle_fill(v0[0], v0[1], v0[2], image, red_color);
    draw_triangle_fill(v1[0], v1[1], v1[2], image, white_color);
    draw_triangle_fill(v2[0], v2[1], v2[2], image, green_color);
    
    image.flip_vertically();
    image.write_tga_file("test.tga", false);
}

void cpp_feature_test() {
}

int main(int argc, char** argv) {
    cpp_feature_test();
//    lesson1_final(argc, argv);
    lesson2_final(argc, argv);
    return 0;
}
