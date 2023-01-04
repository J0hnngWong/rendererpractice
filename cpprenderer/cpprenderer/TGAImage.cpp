//
//  TGAImage.cpp
//  cpprenderer
//
//  Created by john on 2022/12/29.
//

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include "TGAImage.h"

TGAImage::TGAImage(int canvas_width, int canvas_height, int color_format) {
    this->canvas_width = canvas_width;
    this->canvas_height = canvas_height;
    this->color_format = color_format;
    unsigned long canvas_bytes_count = canvas_width * canvas_height * color_format;
    canvas_data = new unsigned char[canvas_bytes_count];
    memset(canvas_data, 0, canvas_bytes_count);
}

TGAImage::~TGAImage() {

}

bool TGAImage::set(int origin_x, int origin_y, TGAColor color) {
    if (!canvas_data || origin_x < 0 || origin_x > canvas_width || origin_y < 0 || origin_y > canvas_height) {
        return false;
    }
    memcpy(canvas_data + (origin_y * canvas_width + origin_x) * color_format, color.raw, color_format);
    return true;
}

bool TGAImage::write_tga_file(const char *file_name, bool run_length_encode) {

    std::ofstream out;

    out.open(file_name, std::ios::binary);

    TGAImageHeader header;
    memset((void *)&header, 0, sizeof(header));
    header.image_type = color_format == GRAY_SCALE ? (run_length_encode ? 11 : 3) : (run_length_encode ? 10 : 2);
    header.image_width = canvas_width;
    header.image_height = canvas_height;
    header.pixel_depth = color_format << 3;
    header.image_descriptor = 0x20;

    TGAImageFooter footer;
    char footer_char[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    memset((void *)&footer, 0, sizeof(footer));
//    strcpy((char *)&footer.developer_area_ref, {0,0,0,0});
//    strcpy((char *)&footer.extension_area_ref, "0000");
    strcpy((char *)&footer.footer, footer_char); //"TRUEVISION-XFILE.\0"

    out.write((char *)&header, sizeof(header));
    out.write((char *)canvas_data, canvas_width * canvas_height * color_format);
    out.write((char *)&footer, sizeof(footer));

    return true;
}

bool TGAImage::flip_horizontally() {
    if (!canvas_data) {
        return false;
    }
    int half_width = canvas_width / 2;
    for (int horizontal_index = 0; horizontal_index < half_width; horizontal_index++) {
        for (int vertical_index = 0; vertical_index < canvas_height; vertical_index++) {
            TGAColor pixel_color = color(horizontal_index, vertical_index);
            TGAColor opposite_pixel_color = color(canvas_width - 1 - horizontal_index, vertical_index);
            set(horizontal_index, vertical_index, opposite_pixel_color);
            set(canvas_width - 1 - horizontal_index, vertical_index, pixel_color);
        }
    }
    return true;
}

bool TGAImage::flip_vertically() {
    if (!canvas_data) {
        return false;
    }
    int bytes_per_line = canvas_width * color_format;
    unsigned char *current_line = new unsigned char[bytes_per_line];
    int half_height = canvas_height / 2;
    for (int vertical_index = 0; vertical_index < half_height; vertical_index++) {
        int line_start_index = vertical_index * bytes_per_line;
        int oppesite_line_start_index = (canvas_height - 1 - vertical_index) * bytes_per_line;
        memmove((void *)current_line, (void *)&canvas_data[line_start_index], bytes_per_line);
        memmove((void *)&canvas_data[line_start_index], (void *)&canvas_data[oppesite_line_start_index], bytes_per_line);
        memmove((void *)&canvas_data[oppesite_line_start_index], (void *)current_line, bytes_per_line);
    }
    delete [] current_line;
    return true;
}

TGAColor TGAImage::color(int x, int y) {
    if (x > canvas_width || y > canvas_height || x < 0 || y < 0 || !canvas_data) {
        return TGAColor();
    }
    auto offset = ((x + y * canvas_width) * color_format);
    auto pointer = &canvas_data[offset];
    return TGAColor(pointer, color_format);
}
