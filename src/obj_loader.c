#include "obj_loader.h" // Đảm bảo đã include header này
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Đảm bảo tên hàm và kiểu dữ liệu khớp với obj_loader.h
bool loadOBJ(const char *path, Model *model)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        fprintf(stderr, "Fehler: Konnte Datei %s nicht öffnen\n", path);
        return false;
    }

    // Tạm cấp bộ nhớ thô (số lượng giới hạn, có thể mở rộng nâng cao)
    unsigned int maxVertices = 10000;
    unsigned int maxIndices = 30000;
    float *vertices = (float *)malloc(sizeof(float) * 3 * maxVertices);
    unsigned int *indices = (unsigned int *)malloc(sizeof(unsigned int) * maxIndices);

    if (!vertices || !indices)
    {
        fprintf(stderr, "Speicherallokation fehlgeschlagen\n");
        fclose(file);
        return false;
    }

    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;

    char lineHeader[128];

    while (fscanf(file, "%s", lineHeader) != EOF)
    {
        if (strcmp(lineHeader, "v") == 0)
        {
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            if (vertexCount < maxVertices)
            {
                vertices[3 * vertexCount] = x;
                vertices[3 * vertexCount + 1] = y;
                vertices[3 * vertexCount + 2] = z;
                vertexCount++;
            }
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            unsigned int vertexIndex[3];
            // Dòng face có thể dạng "f 1 2 3" hoặc "f 1/1/1 2/2/2 3/3/3"
            int matches = fscanf(file, "%u %u %u\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
            if (matches != 3)
            {
                // Thử dạng "f v1/... v2/... v3/..."
                char faceLine[128];
                fgets(faceLine, 128, file);
                // Bỏ qua chi tiết phức tạp, trong ví dụ đơn giản này ta không parse texture/normal
                continue;
            }
            // OBJ index bắt đầu từ 1, ta trừ 1 để thành index C
            if (indexCount + 3 <= maxIndices)
            {
                indices[indexCount++] = vertexIndex[0] - 1;
                indices[indexCount++] = vertexIndex[1] - 1;
                indices[indexCount++] = vertexIndex[2] - 1;
            }
        }
        else
        {
            // Bỏ qua dòng khác
            char buffer[1000];
            fgets(buffer, 1000, file);
        }
    }

    fclose(file);

    // Cấp phát đúng kích thước cho model
    model->vertices = (float *)malloc(sizeof(float) * 3 * vertexCount);
    model->indices = (unsigned int *)malloc(sizeof(unsigned int) * indexCount);

    // Kiểm tra cấp phát bộ nhớ
    if (!model->vertices || !model->indices)
    {
        fprintf(stderr, "Fehler: Speicherallokation für endgültiges Model fehlgeschlagen\n");
        free(vertices);
        free(indices);
        // Giải phóng model nếu đã cấp phát một phần
        if (model->vertices)
            free(model->vertices);
        if (model->indices)
            free(model->indices);
        model->vertices = NULL;
        model->indices = NULL;
        model->vertexCount = 0;
        model->indexCount = 0;
        return false;
    }

    memcpy(model->vertices, vertices, sizeof(float) * 3 * vertexCount);
    memcpy(model->indices, indices, sizeof(unsigned int) * indexCount);

    model->vertexCount = vertexCount;
    model->indexCount = indexCount;

    free(vertices);
    free(indices);

    return true;
}

// Đảm bảo tên hàm và kiểu dữ liệu khớp với obj_loader.h
void freeModel(Model *model)
{
    if (model->vertices)
        free(model->vertices);
    if (model->indices)
        free(model->indices);
    model->vertices = NULL;
    model->indices = NULL;
    model->vertexCount = 0;
    model->indexCount = 0;
}