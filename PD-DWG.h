// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// DWG Intermediate transfer curve.
struct DWGCurve {
    float a;
    float b;
    float c;
    float m;
    float lin_cut;
    float log_cut;
};

__DEVICE__ struct DWGCurve
dwg_curve()
{
    struct DWGCurve cv;
    cv.a = 0.0075f;
    cv.b = 7.0f;
    cv.c = 0.07329248f;
    cv.m = 10.44426855f;
    cv.lin_cut = 0.00262409f;
    cv.log_cut = 0.02740668f;
    return cv;
}

__DEVICE__ float
DWGCurve_lin_dwg(struct DWGCurve cv, float lin)
{
    return lin > cv.lin_cut ? (log2_f(lin + cv.a) + cv.b) * cv.c : lin * cv.m;
}

__DEVICE__ float
DWGCurve_dwg_lin(struct DWGCurve cv, float value)
{
    return value > cv.log_cut ? pow_f(2.0f, value / cv.c - cv.b) - cv.a : value / cv.m;
}

// DWG colorspace
struct DWGColorspace {
    struct Matrix dwg_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
DWGColorspace_xyz_dwg(struct DWGColorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.dwg_matrix);
}

__DEVICE__ float3
DWGColorspace_dwg_xyz(struct DWGColorspace cs, float3 rgb)
{
    return mult_matrix(rgb, cs.xyz_matrix);
}

__DEVICE__ struct DWGColorspace
dwg_colorspace()
{
    struct DWGColorspace cs;

    // colortool --inputcolorspace DWG -v
    cs.dwg_matrix.m00 = 1.516672f;
    cs.dwg_matrix.m01 = -0.281478f;
    cs.dwg_matrix.m02 = -0.146964f;
    cs.dwg_matrix.m03 = -0.464917f;
    cs.dwg_matrix.m04 = 1.251424f;
    cs.dwg_matrix.m05 = 0.174885f;
    cs.dwg_matrix.m06 = 0.064849f;
    cs.dwg_matrix.m07 = 0.109139f;
    cs.dwg_matrix.m08 = 0.761415f;

    cs.xyz_matrix.m00 = 0.700622f;
    cs.xyz_matrix.m01 = 0.148775f;
    cs.xyz_matrix.m02 = 0.101059f;
    cs.xyz_matrix.m03 = 0.274119f;
    cs.xyz_matrix.m04 = 0.873632f;
    cs.xyz_matrix.m05 = -0.147750f;
    cs.xyz_matrix.m06 = -0.098963f;
    cs.xyz_matrix.m07 = -0.137895f;
    cs.xyz_matrix.m08 = 1.325916f;

    return cs;
}

__DEVICE__ float3
dwg_y_lum_coeff()
{
    struct DWGColorspace cs = dwg_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// linear -> DWG Intermediate
__DEVICE__ float3
lin_dwgintermediate(float3 rgb)
{
    struct DWGCurve cv = dwg_curve();
    return make_float3(DWGCurve_lin_dwg(cv, rgb.x), DWGCurve_lin_dwg(cv, rgb.y), DWGCurve_lin_dwg(cv, rgb.z));
}

// DWG Intermediate -> linear
__DEVICE__ float3
dwgintermediate_lin(float3 rgb)
{
    struct DWGCurve cv = dwg_curve();
    return make_float3(DWGCurve_dwg_lin(cv, rgb.x), DWGCurve_dwg_lin(cv, rgb.y), DWGCurve_dwg_lin(cv, rgb.z));
}

// XYZ -> DWG
__DEVICE__ float3
xyz_dwg(float3 xyz)
{
    struct DWGColorspace cs = dwg_colorspace();
    return DWGColorspace_xyz_dwg(cs, xyz);
}

// DWG -> XYZ
__DEVICE__ float3
dwg_xyz(float3 rgb)
{
    struct DWGColorspace cs = dwg_colorspace();
    return DWGColorspace_dwg_xyz(cs, rgb);
}
