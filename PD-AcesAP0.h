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
struct AcesAP0Colorspace
{
    struct Matrix acesAP0_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3 AcesAP0Colorspace_xyz_acesAP0(struct AcesAP0Colorspace cs, float3 xyz) {
    return mult_matrix(xyz, cs.acesAP0_matrix);
}

__DEVICE__ float3 AcesAP0Colorspace_acesAP0_xyz(struct AcesAP0Colorspace cs, float3 acesAP0) {
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

__DEVICE__ struct AcesAP0Colorspace acesAP0_colorspace() {
    struct AcesAP0Colorspace cs;
     // acesAP0 matrix
    cs.acesAP0_matrix.m00 = 1.0498110175; cs.acesAP0_matrix.m01 = 0.000000000; cs.acesAP0_matrix.m02 = -0.0000974845;
    cs.acesAP0_matrix.m03 = -0.4959030231; cs.acesAP0_matrix.m04 = 1.3733130458; cs.acesAP0_matrix.m05 = 0.0982400361;
    cs.acesAP0_matrix.m06 = 0.0000000000; cs.acesAP0_matrix.m07 = 0.0000000000; cs.acesAP0_matrix.m08 = 0.9912520182;
    // xyz matrix
    cs.xyz_matrix.m00 = 0.9525523959; cs.xyz_matrix.m01 = 0.0000000000; cs.xyz_matrix.m02 = 0.0000936786;
    cs.xyz_matrix.m03 = 0.3439664498; cs.xyz_matrix.m04 = 0.7281660966; cs.xyz_matrix.m05 = -0.0721325464;
    cs.xyz_matrix.m06 = 0.0000000000; cs.xyz_matrix.m07 = 0.0000000000; cs.xyz_matrix.m08 = 1.0088251844;
    return cs;
}

// convert linear to acesAP0
__DEVICE__ float3 lin_acesAP0(float3 rgb) {
    struct AcesAP0Curve cv = acesAP0_curve();
    return make_float3(AcesAP0Curve_lin_acesAP0(cv, rgb.x), AcesAP0Curve_lin_acesAP0(cv, rgb.y), AcesAP0Curve_lin_acesAP0(cv, rgb.z));
}

// convert acesAP0 to linear
__DEVICE__ float3 acesAP0_lin(float3 rgb) {
    struct AcesAP0Curve cv = acesAP0_curve();
    return make_float3(AcesAP0Curve_acesAP0_lin(cv, rgb.x), AcesAP0Curve_acesAP0_lin(cv, rgb.y), AcesAP0Curve_acesAP0_lin(cv, rgb.z));
}

// convert linear to acesAP0cc
__DEVICE__ float3 lin_acesAP0cc(float3 rgb) {
    struct AcesAP0ccCurve cv = acesAP0cc_curve();
    return make_float3(AcesAP0ccCurve_lin_acesAP0cc(cv, rgb.x), AcesAP0ccCurve_lin_acesAP0cc(cv, rgb.y), AcesAP0ccCurve_lin_acesAP0cc(cv, rgb.z));
}

// convert acesAP0cc to linear
__DEVICE__ float3 acesAP0cc_lin(float3 rgb) {
    struct AcesAP0ccCurve cv = acesAP0cc_curve();
    return make_float3(AcesAP0ccCurve_acesAP0cc_lin(cv, rgb.x), AcesAP0ccCurve_acesAP0cc_lin(cv, rgb.y), AcesAP0ccCurve_acesAP0cc_lin(cv, rgb.z));
}

// convert linear to acesAP0cct
__DEVICE__ float3 lin_acesAP0cct(float3 rgb) {
    struct AcesAP0cctCurve cv = acesAP0cct_curve();
    return make_float3(AcesAP0cctCurve_lin_acesAP0cct(cv, rgb.x), AcesAP0cctCurve_lin_acesAP0cct(cv, rgb.y), AcesAP0cctCurve_lin_acesAP0cct(cv, rgb.z));
}

// convert acesAP0cct to linear
__DEVICE__ float3 acesAP0cct_lin(float3 rgb) {
    struct AcesAP0cctCurve cv = acesAP0cct_curve();
    return make_float3(AcesAP0cctCurve_acesAP0cct_lin(cv, rgb.x), AcesAP0cctCurve_acesAP0cct_lin(cv, rgb.y), AcesAP0cctCurve_acesAP0cct_lin(cv, rgb.z));
}   

// convert acesAP0 to xyz
__DEVICE__ float3 xyz_acesAP0(float3 rgb) {
    struct AcesAP0Colorspace cs = acesAP0_colorspace();
    float3 acesAP0 = AcesAP0Colorspace_xyz_acesAP0(cs, rgb);
    return acesAP0;
}

// convert xyz to acesAP0
__DEVICE__ float3 acesAP0_xyz(float3 rgb) {
    struct AcesAP0Colorspace cs = acesAP0_colorspace();
    return AcesAP0Colorspace_acesAP0_xyz(cs, rgb);
}    
