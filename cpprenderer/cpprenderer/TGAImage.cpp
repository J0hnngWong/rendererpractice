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

void TGAImage::set(int origin_x, int origin_y, TGAColor color) {

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
