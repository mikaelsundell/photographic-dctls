// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// acesAP0 curve
struct AcesAP0Curve
{
};

// acesAP0cc curve
struct AcesAP0ccCurve
{
    float a;
    float b;
    float t;
    float ls;
    float lo;
};

// acesAP0cct curve
struct AcesAP0cctCurve
{
    float x_brk;
    float y_brk;
    float a;
    float b;
};

__DEVICE__ float AcesAP0Curve_lin_acesAP0(struct AcesAP0Curve cv, float lin) {
    return lin;
}

__DEVICE__ float AcesAP0Curve_acesAP0_lin(struct AcesAP0Curve cv, float val) {
    return val;
}

__DEVICE__ float AcesAP0ccCurve_lin_acesAP0cc(struct AcesAP0ccCurve cv, float lin) {
    if (lin <= 0.0f) {
        return -0.3584474886f;
    } 
    else if (lin < exp2_f(-15.0f)) {
        return (log2_f(_exp2f(-16.0f) + lin * 0.5f) + 9.72f) / 17.52f;
    } 
    else {
        return (log2_f(lin) + 9.72f) / 17.52f;
    }
}

__DEVICE__ float AcesAP0ccCurve_acesAP0cc_lin(struct AcesAP0ccCurve cv, float log) {
    if (log < -0.3013698630f) {
        return (exp2_f(log * 17.52f - 9.72f) - exp2_f(-16.0f)) * 2.0f;
    }
    else {
        return exp2_f(log * 17.52f - 9.72f);
    }
}

__DEVICE__ float AcesAP0cctCurve_lin_acesAP0cct(struct AcesAP0cctCurve cv, float lin) {
    if (lin <= cv.x_brk) {
        return cv.a * lin + cv.b;
    }
    else { 
        return (log2_f(lin) + 9.72f) / 17.52f;
    }
}

__DEVICE__ float AcesAP0cctCurve_acesAP0cct_lin(struct AcesAP0cctCurve cv, float log) {
    if (log > cv.y_brk) {
        return exp2_f(log * 17.52f - 9.72f);
    }
    else {
        return (log - cv.b) / cv.a;
    }
}

// acesAP0 colorspace
struct AcesColorspace
{
    struct Matrix aces_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3 AcesColorspace_xyz_acesAP0(struct AcesColorspace cs, float3 xyz) {
    return mult_matrix(xyz, cs.aces_matrix);
}

__DEVICE__ float3 AcesColorspace_acesAP0_xyz(struct AcesColorspace cs, float3 acesAP0) {
    return mult_matrix(acesAP0, cs.xyz_matrix);
}

__DEVICE__ struct AcesAP0Curve acesAP0_curve() {
    struct AcesAP0Curve cv;
    return cv;
}

__DEVICE__ struct AcesAP0ccCurve acesAP0cc_curve() {
    struct AcesAP0ccCurve cv;
    return cv;
}

__DEVICE__ struct AcesAP0cctCurve acesAP0cct_curve() {
    struct AcesAP0cctCurve cv;
    cv.x_brk = 0.0078125f;
    cv.y_brk = 0.155251141552511f;
    cv.a = 10.5402377416545f;
    cv.b = 0.0729055341958355f;
    return cv;
}

__DEVICE__ struct AcesAP1Colorspace acesAP0_colorspace() {
    struct AcesAP0Colorspace cs;
    // colortool --inputcolorspace AP0 -v
    // aces matrix
    cs.aces_matrix.m00 = 1.0498110175; cs.aces_matrix.m01 = 0.000000000; cs.aces_matrix.m02 = -0.0000974845;
    cs.aces_matrix.m03 = -0.4959030231; cs.aces_matrix.m04 = 1.3733130458; cs.aces_matrix.m05 = 0.0982400361;
    cs.aces_matrix.m06 = 0.0000000000; cs.aces_matrix.m07 = 0.0000000000; cs.aces_matrix.m08 = 0.9912520182;
    // xyz matrix
    cs.xyz_matrix.m00 = 0.9525523959; cs.xyz_matrix.m01 = 0.0000000000; cs.xyz_matrix.m02 = 0.0000936786;
    cs.xyz_matrix.m03 = 0.3439664498; cs.xyz_matrix.m04 = 0.7281660966; cs.xyz_matrix.m05 = -0.0721325464;
    cs.xyz_matrix.m06 = 0.0000000000; cs.xyz_matrix.m07 = 0.0000000000; cs.xyz_matrix.m08 = 1.0088251844;
    return cs;
}

__DEVICE__ struct AcesAP1Colorspace acesAP1_colorspace() {
    struct AcesAP0Colorspace cs;
    // colortool --inputcolorspace AP1 -v
    // acesAP0 matrix
    cs.aces_matrix.m00 = 1.641023; cs.aces_matrix.m01 = -0.324803; cs.aces_matrix.m02 = -0.236425;
    cs.aces_matrix.m03 = -0.663663; cs.aces_matrix.m04 = 1.615332; cs.aces_matrix.m05 = 0.016756;
    cs.aces_matrix.m06 = 0.011722; cs.aces_matrix.m07 = -0.008284; cs.aces_matrix.m08 = 0.98839;
    // xyz matrix
    cs.xyz_matrix.m00 = 0.662454; cs.xyz_matrix.m01 = 0.134004; cs.xyz_matrix.m02 = 0.156188;
    cs.xyz_matrix.m03 = 0.272229; cs.xyz_matrix.m04 = 0.674082; cs.xyz_matrix.m05 = 0.053690;
    cs.xyz_matrix.m06 = -0.005575; cs.xyz_matrix.m07 = 0.004061; cs.xyz_matrix.m08 = 1.010339;
    return cs;
}

// convert linear to aces
__DEVICE__ float3 lin_aces(float3 rgb) {
    struct AcesAP0Curve cv = acesAP0_curve();
    return make_float3(AcesAP0Curve_lin_acesAP0(cv, rgb.x), AcesAP0Curve_lin_acesAP0(cv, rgb.y), AcesAP0Curve_lin_acesAP0(cv, rgb.z));
}

// convert acesAP0 to linear
__DEVICE__ float3 aces_lin(float3 rgb) {
    struct AcesAP0Curve cv = acesAP0_curve();
    return make_float3(AcesAP0Curve_acesAP0_lin(cv, rgb.x), AcesAP0Curve_acesAP0_lin(cv, rgb.y), AcesAP0Curve_acesAP0_lin(cv, rgb.z));
}

// convert linear to acesAP0cc
__DEVICE__ float3 lin_acescc(float3 rgb) {
    struct AcesAP0ccCurve cv = acesAP0cc_curve();
    return make_float3(AcesAP0ccCurve_lin_acesAP0cc(cv, rgb.x), AcesAP0ccCurve_lin_acesAP0cc(cv, rgb.y), AcesAP0ccCurve_lin_acesAP0cc(cv, rgb.z));
}

// convert acesAP0cc to linear
__DEVICE__ float3 acescc_lin(float3 rgb) {
    struct AcesAP0ccCurve cv = acesAP0cc_curve();
    return make_float3(AcesAP0ccCurve_acesAP0cc_lin(cv, rgb.x), AcesAP0ccCurve_acesAP0cc_lin(cv, rgb.y), AcesAP0ccCurve_acesAP0cc_lin(cv, rgb.z));
}

// convert linear to acesAP0cct
__DEVICE__ float3 lin_acescct(float3 rgb) {
    struct AcesAP0cctCurve cv = acesAP0cct_curve();
    return make_float3(AcesAP0cctCurve_lin_acesAP0cct(cv, rgb.x), AcesAP0cctCurve_lin_acesAP0cct(cv, rgb.y), AcesAP0cctCurve_lin_acesAP0cct(cv, rgb.z));
}

// convert acesAP0cct to linear
__DEVICE__ float3 acescct_lin(float3 rgb) {
    struct AcesAP0cctCurve cv = acesAP0cct_curve();
    return make_float3(AcesAP0cctCurve_acesAP0cct_lin(cv, rgb.x), AcesAP0cctCurve_acesAP0cct_lin(cv, rgb.y), AcesAP0cctCurve_acesAP0cct_lin(cv, rgb.z));
}   

// convert acesAP0 to xyz
__DEVICE__ float3 xyz_acesAP0(float3 rgb) {
    struct AcesColorspace cs = acesAP0_colorspace();
    float3 acesAP0 = AcesColorspace_xyz_acesAP0(cs, rgb);
    return acesAP0;
}

// convert xyz to acesAP0
__DEVICE__ float3 acesAP0_xyz(float3 rgb) {
    struct AcesColorspace cs = acesAP0_colorspace();
    return AcesColorspace_acesAP0_xyz(cs, rgb);
}

// convert acesAP1 to xyz
__DEVICE__ float3 xyz_acesAP1(float3 rgb) {
    struct AcesColorspace cs = acesAP0_colorspace();
    float3 acesAP1 = AcesColorspace_xyz_acesAP0(cs, rgb);
    return acesAP1;
}

// convert xyz to acesAP1
__DEVICE__ float3 acesAP1_xyz(float3 rgb) {
    struct AcesColorspace cs = acesAP1_colorspace();
    return AcesColorspace_acesAP1_xyz(cs, rgb);
}    

