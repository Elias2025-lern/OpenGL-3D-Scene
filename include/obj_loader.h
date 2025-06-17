
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdbool.h>

bool load_obj(const char* filename, float** out_vertices, int* out_num_floats);

#endif
