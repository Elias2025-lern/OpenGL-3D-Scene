#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// --- Định nghĩa Struct cho Vector 3D (vec3) ---
typedef struct
{
    float x, y, z;
} vec3;

// --- Định nghĩa Struct cho Vector 4D (vec4) ---
typedef struct
{
    float x, y, z, w;
} vec4;

// --- Định nghĩa Struct cho Ma trận 4x4 (mat4) ---
// Lưu trữ theo cột chính (column-major) - chuẩn của OpenGL
// m[col][row]
typedef struct
{
    float m[4][4];
} mat4;

// --- Khai báo các Hàm Toán học cho Vector ---

// Tạo một vec3 mới
vec3 vec3_create(float x, float y, float z);
// Tạo một vec4 mới
vec4 vec4_create(float x, float y, float z, float w);

// Cộng hai vec3
vec3 vec3_add(vec3 a, vec3 b);
// Trừ hai vec3
vec3 vec3_sub(vec3 a, vec3 b);
// Nhân vec3 với một vô hướng
vec3 vec3_scale(vec3 v, float s);
// Nhân vô hướng (Dot Product) của hai vec3
float vec3_dot(vec3 a, vec3 b);
// Nhân vector (Cross Product) của hai vec3
vec3 vec3_cross(vec3 a, vec3 b);
// Chuẩn hóa vec3 (đưa về độ dài bằng 1)
vec3 vec3_normalize(vec3 v);
// Tính độ dài của vec3
float vec3_length(vec3 v);

// Chuyển đổi vec3 sang vec4 (với w = 1.0f cho điểm, 0.0f cho vector hướng)
vec4 vec3_to_vec4(vec3 v, float w);
// Chuyển đổi vec4 sang vec3
vec3 vec4_to_vec3(vec4 v);

// --- Khai báo các Hàm Toán học cho Ma trận ---

// Tạo một ma trận đơn vị (identity matrix)
mat4 mat4_identity();
// Nhân hai ma trận
mat4 mat4_multiply(mat4 a, mat4 b);
// Tạo ma trận tịnh tiến (Translation Matrix)
mat4 mat4_translate(mat4 m, vec3 v);
// Tạo ma trận xoay (Rotation Matrix) quanh một trục (x, y, hoặc z)
mat4 mat4_rotate(mat4 m, float angle_rad, vec3 axis);
// Tạo ma trận tỉ lệ (Scaling Matrix)
mat4 mat4_scale(mat4 m, vec3 v);

// Tạo ma trận nhìn (View Matrix) từ vị trí camera, điểm nhìn, và vector "up"
mat4 mat4_lookAt(vec3 eye, vec3 center, vec3 up);
// Tạo ma trận chiếu phối cảnh (Perspective Projection Matrix)
mat4 mat4_perspective(float fov_rad, float aspect, float near_plane, float far_plane);

// Áp dụng ma trận lên một vec4
vec4 mat4_transform_vec4(mat4 m, vec4 v);

// Hàm debug: In ma trận ra console
void mat4_print(mat4 m);

#endif // MATH_UTILS_H