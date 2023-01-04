//
//  ObjectModel.hpp
//  cpprenderer
//
//  Created by john on 2022/12/30.
//

#ifndef ObjectModel_hpp
#define ObjectModel_hpp

#include <stdio.h>
#include <vector>
#include "geometry.h"

class ObjectModel {
    std::vector<VectorFloat3> vertex_list;
    std::vector< std::vector<int> > face_list;

public:
    ObjectModel(const char *filename);
    unsigned long vertices_count();
    unsigned long faces_count();
    std::vector<int> face(int index);
    VectorFloat3 vertex(int index);
};

#endif /* ObjectModel_hpp */
