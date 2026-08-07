// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// acesAP0 curve
struct AcesCurve {};

// acesAP0cc curve
struct AcesccCurve {
    float lin_cut;
    float log_cut;
    float min;
    float offset;
    float scale;
    float toe;
};

// acesAP0cct curve
struct AcescctCurve {
    float x_brk;
    float y_brk;
    float a;
    float b;
};

__DEVICE__ float
AcesCurve_lin_aces(struct AcesCurve cv, float lin)
{
    return lin;
}

__DEVICE__ float
AcesCurve_aces_lin(struct AcesCurve cv, float val)
{
    return val;
}

__DEVICE__ float
AcesccCurve_lin_acescc(struct AcesccCurve cv, float lin)
{
    if (lin <= 0.0f) {
        return cv.min;
    }
    else if (lin < cv.lin_cut) {
        return (log2_f(cv.toe + lin * 0.5f) + cv.offset) / cv.scale;
    }
    else {
        return (log2_f(lin) + cv.offset) / cv.scale;
    }
}

__DEVICE__ float
AcesccCurve_acescc_lin(struct AcesccCurve cv, float log)
{
    if (log < cv.log_cut) {
        return (exp2_f(log * cv.scale - cv.offset) - cv.toe) * 2.0f;
    }
    else {
        return exp2_f(log * cv.scale - cv.offset);
    }
}

__DEVICE__ float
AcescctCurve_lin_acescct(struct AcescctCurve cv, float lin)
{
    if (lin <= cv.x_brk) {
        return cv.a * lin + cv.b;
    }
    else {
        return (log2_f(lin) + 9.72f) / 17.52f;
    }
}

__DEVICE__ float
AcescctCurve_acescct_lin(struct AcescctCurve cv, float log)
{
    if (log > cv.y_brk) {
        return exp2_f(log * 17.52f - 9.72f);
    }
    else {
        return (log - cv.b) / cv.a;
    }
}

// aces colorspace
struct AcesColorspace {
    struct Matrix aces_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
AcesColorspace_xyz_aces(struct AcesColorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.aces_matrix);
}

__DEVICE__ float3
AcesColorspace_aces_xyz(struct AcesColorspace cs, float3 acesAP0)
{
    return mult_matrix(acesAP0, cs.xyz_matrix);
}   

__DEVICE__ struct AcesCurve
aces_curve()
{
    struct AcesCurve cv;
    return cv;
}

__DEVICE__ struct AcesccCurve
acescc_curve()
{
    struct AcesccCurve cv;
    cv.lin_cut = exp2_f(-15.0f);
    cv.log_cut = -0.3013698630f;
    cv.min = -0.3584474886f;
    cv.offset = 9.72f;
    cv.scale = 17.52f;
    cv.toe = exp2_f(-16.0f);
    return cv;
}

__DEVICE__ struct AcescctCurve
acescct_curve()
{
    struct AcescctCurve cv;
    cv.x_brk = 0.0078125f;
    cv.y_brk = 0.155251141552511f;
    cv.a = 10.5402377416545f;
    cv.b = 0.0729055341958355f;
    return cv;
}

__DEVICE__ struct AcesColorspace
acesAP0_colorspace()
{
    struct AcesColorspace cs;
    // colortool --inputcolorspace AP0 -v
    // convert xyz to acesAP0 matrix
    cs.aces_matrix.m00 = 1.0498110175f;
    cs.aces_matrix.m01 = 0.000000000f;
    cs.aces_matrix.m02 = -0.0000974845f;
    cs.aces_matrix.m03 = -0.4959030231f;
    cs.aces_matrix.m04 = 1.3733130458f;
    cs.aces_matrix.m05 = 0.0982400361f;
    cs.aces_matrix.m06 = 0.0000000000f;
    cs.aces_matrix.m07 = 0.0000000000f;
    cs.aces_matrix.m08 = 0.9912520182f;
    // convert acesAP0 to xyz matrix
    cs.xyz_matrix.m00 = 0.9525523959f;
    cs.xyz_matrix.m01 = 0.0000000000f;
    cs.xyz_matrix.m02 = 0.0000936786f;
    cs.xyz_matrix.m03 = 0.3439664498f;
    cs.xyz_matrix.m04 = 0.7281660966f;
    cs.xyz_matrix.m05 = -0.0721325464f;
    cs.xyz_matrix.m06 = 0.0000000000f;
    cs.xyz_matrix.m07 = 0.0000000000f;
    cs.xyz_matrix.m08 = 1.0088251844f;
    return cs;
}

__DEVICE__ float3
acesAP0_y_lum_coeff()
{
    struct AcesColorspace cs = acesAP0_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

__DEVICE__ struct AcesColorspace
acesAP1_colorspace()
{
    struct AcesColorspace cs;
    // colortool --inputcolorspace AP1 -v
    // convert xyz to acesAP1 matrix
    cs.aces_matrix.m00 = 1.641023f;
    cs.aces_matrix.m01 = -0.324803f;
    cs.aces_matrix.m02 = -0.236425f;
    cs.aces_matrix.m03 = -0.663663f;
    cs.aces_matrix.m04 = 1.615332f;
    cs.aces_matrix.m05 = 0.016756f;
    cs.aces_matrix.m06 = 0.011722f;
    cs.aces_matrix.m07 = -0.008284f;
    cs.aces_matrix.m08 = 0.98839f;
    // convert acesAP1 to xyz matrix
    cs.xyz_matrix.m00 = 0.662454f;
    cs.xyz_matrix.m01 = 0.134004f;
    cs.xyz_matrix.m02 = 0.156188f;
    cs.xyz_matrix.m03 = 0.272229f;
    cs.xyz_matrix.m04 = 0.674082f;
    cs.xyz_matrix.m05 = 0.053690f;
    cs.xyz_matrix.m06 = -0.005575f;
    cs.xyz_matrix.m07 = 0.004061f;
    cs.xyz_matrix.m08 = 1.010339f;
    return cs;
}

__DEVICE__ float3
acesAP1_y_lum_coeff()
{
    struct AcesColorspace cs = acesAP1_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// convert linear to aces
__DEVICE__ float3
lin_aces(float3 rgb)
{
    struct AcesCurve cv = aces_curve();
    return make_float3(AcesCurve_lin_aces(cv, rgb.x), AcesCurve_lin_aces(cv, rgb.y), AcesCurve_lin_aces(cv, rgb.z));
}

// convert aces to linear
__DEVICE__ float3
aces_lin(float3 rgb)
{
    struct AcesCurve cv = aces_curve();
    return make_float3(AcesCurve_aces_lin(cv, rgb.x), AcesCurve_aces_lin(cv, rgb.y), AcesCurve_aces_lin(cv, rgb.z));
}

// convert linear to acescc
__DEVICE__ float3
lin_acescc(float3 rgb)
{
    struct AcesccCurve cv = acescc_curve();
    return make_float3(AcesccCurve_lin_acescc(cv, rgb.x), AcesccCurve_lin_acescc(cv, rgb.y),
                       AcesccCurve_lin_acescc(cv, rgb.z));
}

// convert acescc to linear
__DEVICE__ float3
acescc_lin(float3 rgb)
{
    struct AcesccCurve cv = acescc_curve();
    return make_float3(AcesccCurve_acescc_lin(cv, rgb.x), AcesccCurve_acescc_lin(cv, rgb.y),
                       AcesccCurve_acescc_lin(cv, rgb.z));
}

// convert linear to acescct
__DEVICE__ float3
lin_acescct(float3 rgb)
{
    struct AcescctCurve cv = acescct_curve();
    return make_float3(AcescctCurve_lin_acescct(cv, rgb.x), AcescctCurve_lin_acescct(cv, rgb.y),
                       AcescctCurve_lin_acescct(cv, rgb.z));
}

// convert acescct to linear
__DEVICE__ float3
acescct_lin(float3 rgb)
{
    struct AcescctCurve cv = acescct_curve();
    return make_float3(AcescctCurve_acescct_lin(cv, rgb.x), AcescctCurve_acescct_lin(cv, rgb.y),
                       AcescctCurve_acescct_lin(cv, rgb.z));
}

// convert xyz to acesAP0
__DEVICE__ float3
xyz_acesAP0(float3 rgb)
{
    struct AcesColorspace cs = acesAP0_colorspace();
    float3 acesAP0 = AcesColorspace_xyz_aces(cs, rgb);
    return acesAP0;
}

// convert acesAP0 to xyz
__DEVICE__ float3
acesAP0_xyz(float3 rgb)
{
    struct AcesColorspace cs = acesAP0_colorspace();
    return AcesColorspace_aces_xyz(cs, rgb);
}

// convert xyz to acesAP1
__DEVICE__ float3
xyz_acesAP1(float3 rgb)
{
    struct AcesColorspace cs = acesAP1_colorspace();
    float3 acesAP1 = AcesColorspace_xyz_aces(cs, rgb);
    return acesAP1;
}

// convert acesAP1 to xyz
__DEVICE__ float3
acesAP1_xyz(float3 rgb)
{
    struct AcesColorspace cs = acesAP1_colorspace();
    return AcesColorspace_aces_xyz(cs, rgb);
}
