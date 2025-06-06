// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// dwg curve
struct DWGCurve
{
    float a;
    float b;
    float c;
    float m;
    float lin_cut;
    float log_cut;
};

__DEVICE__ struct DWGCurve dwg_curve() {
    struct DWGCurve cv;
    cv.a = 0.0075;
    cv.b = 7.0;
    cv.c = 0.07329248;
    cv.m = 10.44426855;
    cv.lin_cut = 0.00262409;
    cv.log_cut = 0.02740668;
    return cv;
}

__DEVICE__ float DWGCurve_lin_dwg(struct DWGCurve cv, float lin) {
    return (lin > cv.lin_cut) ? (log2_f(lin + cv.a) + cv.b) * cv.c : lin * cv.m;
}

__DEVICE__ float DWGCurve_dwg_lin(struct DWGCurve cv, float val) {
    return (val > cv.log_cut) ? pow_f(2.0f, (val / cv.c) - cv.b) - cv.a : (val / cv.m);
}

// dwg colorspace
struct DWGColorspace
{
    struct Matrix dwg_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ struct DWGColorspace dwg_colorspace() {
    struct DWGColorspace cs;
    // colortool --inputcolorspace DWG -v
    // dwg matrix
    cs.dwg_matrix.m00 = 1.516672; cs.dwg_matrix.m01 = -0.281478; cs.dwg_matrix.m02 = -0.146964;
    cs.dwg_matrix.m03 = -0.464917; cs.dwg_matrix.m04 = 1.251424; cs.dwg_matrix.m05 = 0.174885;
    cs.dwg_matrix.m06 = 0.064849; cs.dwg_matrix.m07 = 0.109139; cs.dwg_matrix.m08 = 0.761415;
    // xyz matrix
    cs.xyz_matrix.m00 =  0.700622; cs.xyz_matrix.m01 = 0.148775; cs.xyz_matrix.m02 = 0.101059;
    cs.xyz_matrix.m03 = 0.274119; cs.xyz_matrix.m04 = 0.873632; cs.xyz_matrix.m05 = -0.147750;
    cs.xyz_matrix.m06 = -0.098963; cs.xyz_matrix.m07 = -0.137895; cs.xyz_matrix.m08 = 1.325916;
    return cs;
}

__DEVICE__ float3 DWGColorspace_xyz_dwg(struct DWGColorspace cs, float3 xyz) {
    return mult_matrix(xyz, cs.dwg_matrix);
}
__DEVICE__ float3 DWGColorspace_dwg_xyz(struct DWGColorspace cs, float3 dwg) {
    return mult_matrix(dwg, cs.xyz_matrix);
}

// convert linear to dwg intermediate
__DEVICE__ float3 lin_dwgintermediate(float3 rgb) {
    struct DWGCurve cv = dwg_curve();
    return make_float3(DWGCurve_lin_dwg(cv, rgb.x), DWGCurve_lin_dwg(cv, rgb.y), DWGCurve_lin_dwg(cv, rgb.z));
}

// convert dwg intermediate to linear
__DEVICE__ float3 dwgintermediate_lin(float3 rgb) {
    struct DWGCurve cv = dwg_curve();
    return make_float3(DWGCurve_dwg_lin(cv, rgb.x), DWGCurve_dwg_lin(cv, rgb.y), DWGCurve_dwg_lin(cv, rgb.z));
}    

// convert dwg to xyz
__DEVICE__ float3 xyz_dwg(float3 rgb) {
    struct DWGColorspace cs = dwg_colorspace();
    float3 dwg = DWGColorspace_xyz_dwg(cs, rgb);
    return dwg;
}

// convert xyz to dwg
__DEVICE__ float3 dwg_xyz(float3 rgb) {
    struct DWGColorspace cs = dwg_colorspace();
    return DWGColorspace_dwg_xyz(cs, rgb);
}    
