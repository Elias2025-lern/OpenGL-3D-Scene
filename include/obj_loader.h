#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdbool.h>

// Model struct
typedef struct
{
    float *vertices;
    unsigned int vertexCount;
    unsigned int *indices;
    unsigned int indexCount;
} Model;

// Hàm load obj, trả về true nếu thành công
bool loadOBJ(const char *path, Model *model);

// Giải phóng bộ nhớ model
void freeModel(Model *model);

#endif
