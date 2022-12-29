//
//  main.cpp
//  cpprenderer
//
//  Created by john on 2022/12/29.
//

#include <stdio.h>
#include "TGAImage.h"

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::ColorFormat::RGB);
    image.write_tga_file("test.tga", false);
}
