// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// gen5 curve
struct Gen5Curve
{
    float a;
    float b;
    float c;
    float d;
    float e;
    float lin_cut;
    float log_cut;
};

__DEVICE__ struct Gen5Curve gen5_curve() {
    struct Gen5Curve cv;
    cv.a = 0.08692876065491224; cv.b = 0.005494072432257808; cv.c = 0.5300133392291939; cv.d = 8.283605932402494; cv.e = 0.09246575342465753; cv.lin_cut = 0.005; cv.log_cut = cv.d * cv.lin_cut + cv.e;
    return cv;
}

__DEVICE__ float Gen5Curve_lin_gen5(struct Gen5Curve cv, float lin) {
    return ((lin >= cv.lin_cut) ? cv.a * log_f(lin + cv.b) + cv.c : cv.d * lin + cv.e);
}

__DEVICE__ float Gen5Curve_gen5_lin(struct Gen5Curve cv, float log) {
    return ((log >= cv.log_cut) ? exp_f((log - cv.c) / cv.a) - cv.b : (log - cv.e) / cv.d);
}

// gen5 colorspace
struct Gen5Colorspace
{
    struct Matrix gen5_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3 Gen5Colorspace_xyz_gen5(struct Gen5Colorspace cs, float3 xyz) {
    return mult_matrix(xyz, cs.gen5_matrix);
}
__DEVICE__ float3 Gen5Colorspace_gen5_xyz(struct Gen5Colorspace cs, float3 gen5) {
    return mult_matrix(gen5, cs.xyz_matrix);
}

__DEVICE__ struct Gen5Colorspace gen5_colorspace() {
    struct Gen5Colorspace cs;
    // colortool --inputcolorspace GEN5 -v
    // convert xyz to gen5 matrix
    cs.gen5_matrix.m00 = 1.866382; cs.gen5_matrix.m01 = -0.518397; cs.gen5_matrix.m02 = -0.234610;
    cs.gen5_matrix.m03 = -0.600342; cs.gen5_matrix.m04 = 1.378149; cs.gen5_matrix.m05 = 0.176732;
    cs.gen5_matrix.m06 = 0.002452; cs.gen5_matrix.m07 = 0.086400; cs.gen5_matrix.m08 = 0.836943;
    // convert gen5 to xyz matrix
    cs.xyz_matrix.m00 = 0.606530; cs.xyz_matrix.m01 = 0.220408; cs.xyz_matrix.m02 = 0.123479;
    cs.xyz_matrix.m03 = 0.267989; cs.xyz_matrix.m04 = 0.832731; cs.xyz_matrix.m05 = -0.100720;
    cs.xyz_matrix.m06 = -0.029442; cs.xyz_matrix.m07 = -0.086611; cs.xyz_matrix.m08 = 1.204861;
    return cs;
}

__DEVICE__ float3 gen5_y_lum_coeff() {
    struct Gen5Colorspace cs = gen5_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// convert linear to gen5
__DEVICE__ float3 lin_gen5(float3 rgb) {
    struct Gen5Curve cv = gen5_curve();
    return make_float3(Gen5Curve_lin_gen5(cv, rgb.x), Gen5Curve_lin_gen5(cv, rgb.y), Gen5Curve_lin_gen5(cv, rgb.z));
}

// convert gen5 to linear
__DEVICE__ float3 gen5_lin(float3 rgb) {
    struct Gen5Curve cv = gen5_curve();
    return make_float3(Gen5Curve_gen5_lin(cv, rgb.x), Gen5Curve_gen5_lin(cv, rgb.y), Gen5Curve_gen5_lin(cv, rgb.z));
}    

// convert xyz to gen5
__DEVICE__ float3 xyz_gen5(float3 rgb) {
    struct Gen5Colorspace cs = gen5_colorspace();
    return Gen5Colorspace_xyz_gen5(cs, rgb);
}

// convert gen5 to xyz
__DEVICE__ float3 gen5_xyz(float3 rgb) {
    struct Gen5Colorspace cs = gen5_colorspace();
    return Gen5Colorspace_gen5_xyz(cs, rgb);
}    
