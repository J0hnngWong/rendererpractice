//
//  ObjectModel.cpp
//  cpprenderer
//
//  Created by john on 2022/12/30.
//

#include <iostream>
#include <vector>
#include "ObjectModel.hpp"
#include "geometry.h"

// Reference: https://en.wikipedia.org/wiki/Wavefront_.obj_file

class ObjectModel {
    std::vector<VectorFloat3> vertexs;
    
public:
    ObjectModel(const char *filename);
};
