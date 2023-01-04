//
//  ObjectModel.cpp
//  cpprenderer
//
//  Created by john on 2022/12/30.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "ObjectModel.h"
#include "geometry.h"

// Reference: https://en.wikipedia.org/wiki/Wavefront_.obj_file

ObjectModel::ObjectModel(const char *filename) : vertex_list(), face_list() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            // vertex
            // v character
            iss >> trash;
            VectorFloat3 vertex;
            for (int i = 0; i < 3; i++) {
                iss >> vertex.raw[i];
            }
            vertex_list.push_back(vertex);
        } else if (!line.compare(0, 2, "f ")) {
            // faces f vertex/vertex texture/vertex normal(法线)
            std::vector<int> face;
            int index, vertex_texture, vertex_normal;
            // f character
            iss >> trash;
            while (iss >> index >> trash >> vertex_texture >> trash >> vertex_normal) {
                index--;
                face.push_back(index);
            }
            face_list.push_back(face);
        }
    }
    std::cout << "vertices size: " << vertex_list.size() << " faces size: " << face_list.size() << "\n";
}

unsigned long ObjectModel::vertices_count() {
    return vertex_list.size();
}

unsigned long ObjectModel::faces_count() {
    return face_list.size();
}

std::vector<int> ObjectModel::face(int index) {
    return face_list[index];
}

VectorFloat3 ObjectModel::vertex(int index) {
    return vertex_list[index];
}
