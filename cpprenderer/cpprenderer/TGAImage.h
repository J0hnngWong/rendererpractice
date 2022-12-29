//
//  TGAImage.h
//  cpprenderer
//
//  Created by john on 2022/12/28.
//

#ifndef TGAImage_h
#define TGAImage_h

// Reference: https://en.wikipedia.org/wiki/Truevision_TGA

// 可以规划各个属性的储存位置来保证对齐

#pragma pack(push, 1)
struct TGAImageHeader {
    char id_length;
    char color_map_type;
    /*
     0 no image data is present
     1 uncompressed color-mapped image
     2 uncompressed true-color image
     3 uncompressed black-and-white (grayscale) image
     9 run-length encoded color-mapped image
     10 run-length encoded true-color image
     11 run-length encoded black-and-white (grayscale) image
     */
    char image_type;
    // color map specification
    /*
     First entry index (2 bytes): index of first color map entry that is included in the file
     Color map length (2 bytes): number of entries of the color map that are included in the file
     Color map entry size (1 byte): number of bits per color map entry
     */
    short first_entry_index;
    short color_map_length;
    char color_map_entry_size;
    // image specification
    short origin_x;
    short origin_y;
    short image_width;
    short image_height;
    char pixel_depth;
    char image_descriptor;
};
#pragma pack(pop)

// difference between those two structs
/* struct SizeTest
{
    char idarea1;
    short width1;
    char idarea2;
};
struct SizeTest
{
    char idarea1;
    char idarea2;
    short width1;
};
*/

struct TGAImageFooter {
    unsigned char developer_area_ref[4];
    unsigned char extension_area_ref[4];
    unsigned char footer[18];
};

struct TGAColor {

};

class TGAImage {
private:
    unsigned char* canvas_data;
    int canvas_width;
    int canvas_height;
    int color_format;

public:
    enum ColorFormat {
        GRAY_SCALE = 1,
        RGB = 3,
        RGBA = 4
    };

    TGAImage(int canvas_width, int canvas_height, int color_format);
    ~TGAImage();
    void set(int origin_x, int origin_y, TGAColor color);
    bool write_tga_file(const char *file_name, bool run_length_encode);
};

#endif /* TGAImage_h */
