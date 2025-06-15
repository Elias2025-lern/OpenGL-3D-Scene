#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrix.h"
#ifndef M_PI
#define M_PI 3.14
#endif

#define EPSILON 0.0001f

// Vergleich von zwei float-Werten
int float_equal(float a, float b) {
    return fabsf(a - b) < EPSILON;
}

// Vergleich zweier Vektoren
int vec3_equal(vec3 a, vec3 b) {
    return float_equal(a.x, b.x) && float_equal(a.y, b.y) && float_equal(a.z, b.z);
}

int vec4_equal(vec4 a, vec4 b) {
    return float_equal(a.x, b.x) && float_equal(a.y, b.y) &&
           float_equal(a.z, b.z) && float_equal(a.w, b.w);
}

// Vergleich zweier Matrizen
int mat4_equal(mat4 a, mat4 b) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (!float_equal(a.m[i][j], b.m[i][j]))
                return 0;
    return 1;
}

void test(const char *name, int condition) {
    printf("%s: %s\n", name, condition ? "OK" : "Nicht OK");
}

int main() {
    // Test mat4_identity
    mat4 id = mat4_identity();
    mat4 expected_id = {{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}};
    test("mat4_identity", mat4_equal(id, expected_id));

    // Test mat4_multiply (identity * identity = identity)
    mat4 mult = mat4_multiply(id, id);
    test("mat4_multiply identity", mat4_equal(mult, id));

    // Test mat4_translate
    vec3 t = vec3_create(1, 2, 3);
    mat4 translated = mat4_translate(mat4_identity(), t);
    mat4 expected_trans = mat4_identity();
    expected_trans.m[3][0] = 1;
    expected_trans.m[3][1] = 2;
    expected_trans.m[3][2] = 3;
    test("mat4_translate", mat4_equal(translated, expected_trans));

    // Test mat4_scale
    vec3 scale = vec3_create(2, 3, 4);
    mat4 scaled = mat4_scale(mat4_identity(), scale);
    mat4 expected_scale = {{{2,0,0,0},{0,3,0,0},{0,0,4,0},{0,0,0,1}}};
    test("mat4_scale", mat4_equal(scaled, expected_scale));

    // Test mat4_rotate um 90 Grad bei z-Achse
    vec3 z_axis = vec3_create(0, 0, 1);
    float angle = M_PI / 2;
    mat4 rot = mat4_rotate(mat4_identity(), angle, z_axis);
    vec4 v = vec4_create(1, 0, 0, 1);
    vec4 rotated = mat4_transform_vec4(rot, v);
    vec4 expected_rotated = vec4_create(0, 1, 0, 1);
    test("mat4_rotate 90 deg z-axis", vec4_equal(rotated, expected_rotated));

    // Test mat4_lookAt
    vec3 eye = vec3_create(0, 0, 1);
    vec3 center = vec3_create(0, 0, 0);
    vec3 up = vec3_create(0, 1, 0);
    mat4 view = mat4_lookAt(eye, center, up);
    vec4 origin = vec4_create(0, 0, 0, 1);
    vec4 transformed_origin = mat4_transform_vec4(view, origin);
    test("mat4_lookAt eye=(0,0,1)", float_equal(transformed_origin.z, -1));

    // Test mat4_perspective
    mat4 proj = mat4_perspective(M_PI/2, 1.0f, 1.0f, 10.0f);
    test("mat4_perspective valid", float_equal(proj.m[2][2], -1.22222f) == 0 ? 0 : 1); // approximate test

    // Test mat4_transform_vec4 with identity
    vec4 vin = vec4_create(1, 2, 3, 1);
    vec4 vout = mat4_transform_vec4(mat4_identity(), vin);
    test("mat4_transform_vec4 identity", vec4_equal(vin, vout));

    // Sonderfall: Transformation in-place testen
    mat4 m = mat4_identity();
    vec4 inplace = vec4_create(0, 1, 0, 1);
    vec4 result = mat4_transform_vec4(m, inplace);
    test("mat4_transform_vec4 inplace", vec4_equal(result, inplace));

    return 0;
}
