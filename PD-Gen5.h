// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// Gen5 transfer curve
struct Gen5Curve {
    float a;
    float b;
    float c;
    float d;
    float e;
    float lin_cut;
    float log_cut;
};

__DEVICE__ struct Gen5Curve
gen5_curve()
{
    struct Gen5Curve cv;
    cv.a = 0.08692876065491224f;
    cv.b = 0.005494072432257808f;
    cv.c = 0.5300133392291939f;
    cv.d = 8.283605932402494f;
    cv.e = 0.09246575342465753f;
    cv.lin_cut = 0.005f;
    cv.log_cut = cv.d * cv.lin_cut + cv.e;
    return cv;
}

__DEVICE__ float
Gen5Curve_lin_gen5(struct Gen5Curve cv, float lin)
{
    return lin >= cv.lin_cut ? cv.a * log_f(lin + cv.b) + cv.c : cv.d * lin + cv.e;
}

__DEVICE__ float
Gen5Curve_gen5_lin(struct Gen5Curve cv, float value)
{
    return value >= cv.log_cut ? exp_f((value - cv.c) / cv.a) - cv.b : (value - cv.e) / cv.d;
}

// Gen5 colorspace
struct Gen5Colorspace {
    struct Matrix gen5_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
Gen5Colorspace_xyz_gen5(struct Gen5Colorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.gen5_matrix);
}

__DEVICE__ float3
Gen5Colorspace_gen5_xyz(struct Gen5Colorspace cs, float3 rgb)
{
    return mult_matrix(rgb, cs.xyz_matrix);
}

__DEVICE__ struct Gen5Colorspace
gen5_colorspace()
{
    struct Gen5Colorspace cs;

    // colortool --inputcolorspace GEN5 -v
    cs.gen5_matrix.m00 = 1.866382f;
    cs.gen5_matrix.m01 = -0.518397f;
    cs.gen5_matrix.m02 = -0.234610f;
    cs.gen5_matrix.m03 = -0.600342f;
    cs.gen5_matrix.m04 = 1.378149f;
    cs.gen5_matrix.m05 = 0.176732f;
    cs.gen5_matrix.m06 = 0.002452f;
    cs.gen5_matrix.m07 = 0.086400f;
    cs.gen5_matrix.m08 = 0.836943f;

    cs.xyz_matrix.m00 = 0.606530f;
    cs.xyz_matrix.m01 = 0.220408f;
    cs.xyz_matrix.m02 = 0.123479f;
    cs.xyz_matrix.m03 = 0.267989f;
    cs.xyz_matrix.m04 = 0.832731f;
    cs.xyz_matrix.m05 = -0.100720f;
    cs.xyz_matrix.m06 = -0.029442f;
    cs.xyz_matrix.m07 = -0.086611f;
    cs.xyz_matrix.m08 = 1.204861f;

    return cs;
}

__DEVICE__ float3
gen5_y_lum_coeff()
{
    struct Gen5Colorspace cs = gen5_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// linear -> Gen5
__DEVICE__ float3
lin_gen5(float3 rgb)
{
    struct Gen5Curve cv = gen5_curve();
    return make_float3(Gen5Curve_lin_gen5(cv, rgb.x), Gen5Curve_lin_gen5(cv, rgb.y), Gen5Curve_lin_gen5(cv, rgb.z));
}

// Gen5 -> linear
__DEVICE__ float3
gen5_lin(float3 rgb)
{
    struct Gen5Curve cv = gen5_curve();
    return make_float3(Gen5Curve_gen5_lin(cv, rgb.x), Gen5Curve_gen5_lin(cv, rgb.y), Gen5Curve_gen5_lin(cv, rgb.z));
}

// XYZ -> Gen5
__DEVICE__ float3
xyz_gen5(float3 xyz)
{
    struct Gen5Colorspace cs = gen5_colorspace();
    return Gen5Colorspace_xyz_gen5(cs, xyz);
}

// Gen5 -> XYZ
__DEVICE__ float3
gen5_xyz(float3 rgb)
{
    struct Gen5Colorspace cs = gen5_colorspace();
    return Gen5Colorspace_gen5_xyz(cs, rgb);
}
