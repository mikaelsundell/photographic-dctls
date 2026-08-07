// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// logCColor
struct LogCColor {
    float stop;
    float r;
    float g;
    float b;
};

__CONSTANT__ struct LogCColor logC_colors[] = {
    { -8, 0.01f, 0.01f, 0.01f }, { -7, 0.05f, 0.05f, 0.05f }, { -6, 0.10f, 0.10f, 0.10f }, { -5, 0.40f, 0.25f, 0.60f },
    { -4, 0.20f, 0.45f, 0.70f }, { -3, 0.40f, 0.60f, 0.95f }, { -2, 0.40f, 0.60f, 0.25f }, { -1, 0.60f, 0.90f, 0.55f },
    { 0, 0.50f, 0.50f, 0.50f },  { 1, 1.00f, 0.95f, 0.25f },  { 2, 0.90f, 0.60f, 0.25f },  { 3, 0.90f, 0.50f, 0.25f },
    { 4, 0.90f, 0.35f, 0.30f },  { 5, 0.90f, 0.30f, 0.20f },  { 6, 0.90f, 0.90f, 0.90f },  { 7, 0.95f, 0.95f, 0.95f },
    { 8, 0.99f, 0.99f, 0.99f }
};

#define logC_stops 17

// logC3 curve
struct LogC3Curve {
    int ei;
    float cut;
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
};

__DEVICE__ struct LogC3Curve
logC3_curve(int ei)
{
    struct LogC3Curve cv;
    if (ei == EI160) {
        cv.ei = 160;
        cv.cut = 0.005561f;
        cv.a = 5.555556f;
        cv.b = 0.080216f;
        cv.c = 0.269036f;
        cv.d = 0.381991f;
        cv.e = 5.842037f;
        cv.f = 0.092778f;
    }
    else if (ei == EI200) {
        cv.ei = 200;
        cv.cut = 0.006208f;
        cv.a = 5.555556f;
        cv.b = 0.076621f;
        cv.c = 0.266007f;
        cv.d = 0.382478f;
        cv.e = 5.776265f;
        cv.f = 0.092782f;
    }
    else if (ei == EI250) {
        cv.ei = 250;
        cv.cut = 0.006871f;
        cv.a = 5.555556f;
        cv.b = 0.072941f;
        cv.c = 0.262978f;
        cv.d = 0.382966f;
        cv.e = 5.710494f;
        cv.f = 0.092786f;
    }
    else if (ei == EI320) {
        cv.ei = 320;
        cv.cut = 0.007622f;
        cv.a = 5.555556f;
        cv.b = 0.068768f;
        cv.c = 0.259627f;
        cv.d = 0.383508f;
        cv.e = 5.637732f;
        cv.f = 0.092791f;
    }
    else if (ei == EI400) {
        cv.ei = 400;
        cv.cut = 0.008318f;
        cv.a = 5.555556f;
        cv.b = 0.064901f;
        cv.c = 0.256598f;
        cv.d = 0.383999f;
        cv.e = 5.571960f;
        cv.f = 0.092795f;
    }
    else if (ei == EI500) {
        cv.ei = 500;
        cv.cut = 0.009031f;
        cv.a = 5.555556f;
        cv.b = 0.060939f;
        cv.c = 0.253569f;
        cv.d = 0.384493f;
        cv.e = 5.506188f;
        cv.f = 0.092800f;
    }
    else if (ei == EI640) {
        cv.ei = 640;
        cv.cut = 0.009840f;
        cv.a = 5.555556f;
        cv.b = 0.056443f;
        cv.c = 0.250219f;
        cv.d = 0.385040f;
        cv.e = 5.433426f;
        cv.f = 0.092805f;
    }
    else if (ei == EI800) {
        cv.ei = 800;
        cv.cut = 0.010591f;
        cv.a = 5.555556f;
        cv.b = 0.052272f;
        cv.c = 0.247190f;
        cv.d = 0.385537f;
        cv.e = 5.367655f;
        cv.f = 0.092809f;
    }
    else if (ei == EI1000) {
        cv.ei = 1000;
        cv.cut = 0.011361f;
        cv.a = 5.555556f;
        cv.b = 0.047996f;
        cv.c = 0.244161f;
        cv.d = 0.386036f;
        cv.e = 5.301883f;
        cv.f = 0.092814f;
    }
    else if (ei == EI1280) {
        cv.ei = 1280;
        cv.cut = 0.012235f;
        cv.a = 5.555556f;
        cv.b = 0.043137f;
        cv.c = 0.240810f;
        cv.d = 0.386590f;
        cv.e = 5.229121f;
        cv.f = 0.092819f;
    }
    else if (ei == EI1600) {
        cv.ei = 1600;
        cv.cut = 0.013047f;
        cv.a = 5.555556f;
        cv.b = 0.038625f;
        cv.c = 0.237781f;
        cv.d = 0.387093f;
        cv.e = 5.163350f;
        cv.f = 0.092824f;
    }
    return cv;
}

__DEVICE__ float
LogC3Curve_lin_logC3(struct LogC3Curve cv, float lin)
{
    return ((lin > cv.cut) ? cv.c * log10_f(cv.a * lin + cv.b) + cv.d : cv.e * lin + cv.f);
}

__DEVICE__ float
LogC3Curve_logC3_lin(struct LogC3Curve cv, float log)
{
    float lin = ((log > cv.e * cv.cut + cv.f) ? (pow_f(10.0f, (log - cv.d) / cv.c) - cv.b) / cv.a : (log - cv.f) / cv.e);
    return lin;
}

// logC4 curve
struct LogC4Curve {
    float a;
    float b;
    float c;
    float s;
    float t;
};

__DEVICE__ struct LogC4Curve
logC4_curve()
{
    struct LogC4Curve cv;
    cv.a = (pow_f(2.0f, 18.0f) - 16.0f) / 117.45f;
    cv.b = (1023.0f - 95.0f) / 1023;
    cv.c = 95.0f / 1023.0f;
    cv.s = (7 * log_f(2.0f) * pow_f(2.0f, 7 - 14 * cv.c / cv.b)) / (cv.a * cv.b);
    cv.t = (pow_f(2.0f, 14.0f * (-cv.c / cv.b) + 6.0f) - 64.0f) / cv.a;
    return cv;
}

__DEVICE__ float
LogC4Curve_lin_logC4(struct LogC4Curve cv, float lin)
{
    return ((lin < cv.t) ? ((lin - cv.t) / cv.s) : (log2_f(cv.a * lin + 64) - 6.0f) / 14.0f * cv.b + cv.c);
}

__DEVICE__ float
LogC4Curve_logC4_lin(struct LogC4Curve cv, float log)
{
    return ((log < 0.0f) ? (log * cv.s + cv.t) : (pow_f(2.0f, (14.0f * (log - cv.c) / cv.b + 6.0f)) - 64.0f) / cv.a);
}

// logC colorspace
struct LogCColorspace {
    struct Matrix logC_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
LogCColorspace_xyz_logC(struct LogCColorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.logC_matrix);
}

__DEVICE__ float3
LogCColorspace_logC_xyz(struct LogCColorspace cs, float3 logC3)
{
    return mult_matrix(logC3, cs.xyz_matrix);
}

__DEVICE__ struct LogCColorspace
logC3_colorspace()
{
    struct LogCColorspace cs;
    // colortool --inputcolorspace AWG3 -v
    // convert xyz to logC3 matrix
    cs.logC_matrix.m00 = 1.789066f;
    cs.logC_matrix.m01 = -0.482534f;
    cs.logC_matrix.m02 = -0.200076f;
    cs.logC_matrix.m03 = -0.639849f;
    cs.logC_matrix.m04 = 1.396400f;
    cs.logC_matrix.m05 = 0.194432f;
    cs.logC_matrix.m06 = -0.041532f;
    cs.logC_matrix.m07 = 0.082335f;
    cs.logC_matrix.m08 = 0.878868f;
    // convert logC3 to xyz matrix
    cs.xyz_matrix.m00 = 0.638008f;
    cs.xyz_matrix.m01 = 0.214704f;
    cs.xyz_matrix.m02 = 0.097744f;
    cs.xyz_matrix.m03 = 0.291954f;
    cs.xyz_matrix.m04 = 0.823841f;
    cs.xyz_matrix.m05 = -0.115795f;
    cs.xyz_matrix.m06 = 0.002798f;
    cs.xyz_matrix.m07 = -0.067034f;
    cs.xyz_matrix.m08 = 1.153294f;
    return cs;
}

__DEVICE__ float3
logc3_y_lum_coeff()
{
    struct LogCColorspace cs = logC3_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

__DEVICE__ struct LogCColorspace
logC4_colorspace()
{
    struct LogCColorspace cs;
    // colortool --inputcolorspace AWG4 -v
    // convert xyz to logC4 matrix
    cs.logC_matrix.m00 = 1.5092155f;
    cs.logC_matrix.m01 = -0.2505973f;
    cs.logC_matrix.m02 = -0.1688115f;
    cs.logC_matrix.m03 = -0.4915455f;
    cs.logC_matrix.m04 = 1.3612455f;
    cs.logC_matrix.m05 = 0.0972829f;
    cs.logC_matrix.m06 = 0.0000000f;
    cs.logC_matrix.m07 = 0.0000000f;
    cs.logC_matrix.m08 = 0.9182250f;
    // convert logC4 to xyz matrix
    cs.xyz_matrix.m00 = 0.7048583f;
    cs.xyz_matrix.m01 = 0.1297603f;
    cs.xyz_matrix.m02 = 0.1158373f;
    cs.xyz_matrix.m03 = 0.2545242f;
    cs.xyz_matrix.m04 = 0.7814777f;
    cs.xyz_matrix.m05 = -0.0360019f;
    cs.xyz_matrix.m06 = 0.0000000f;
    cs.xyz_matrix.m07 = 0.0000000f;
    cs.xyz_matrix.m08 = 1.0890578f;
    return cs;
}

__DEVICE__ float3
logc4_y_lum_coeff()
{
    struct LogCColorspace cs = logC4_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// convert linear to LogC3
__DEVICE__ float3
lin_logC3(float3 rgb, int ei)
{
    struct LogC3Curve cv = logC3_curve(ei);
    return make_float3(LogC3Curve_lin_logC3(cv, rgb.x), LogC3Curve_lin_logC3(cv, rgb.y),
                       LogC3Curve_lin_logC3(cv, rgb.z));
}

// convert LogC3 to linear
__DEVICE__ float3
logC3_lin(float3 rgb, int ei)
{
    struct LogC3Curve cv = logC3_curve(ei);
    return make_float3(LogC3Curve_logC3_lin(cv, rgb.x), LogC3Curve_logC3_lin(cv, rgb.y),
                       LogC3Curve_logC3_lin(cv, rgb.z));
}

// convert linear to logC4
__DEVICE__ float3
lin_logC4(float3 rgb)
{
    struct LogC4Curve cv = logC4_curve();
    return make_float3(LogC4Curve_lin_logC4(cv, rgb.x), LogC4Curve_lin_logC4(cv, rgb.y),
                       LogC4Curve_lin_logC4(cv, rgb.z));
}

// convert logC4 to linear
__DEVICE__ float3
logC4_lin(float3 rgb)
{
    struct LogC4Curve cv = logC4_curve();
    return make_float3(LogC4Curve_logC4_lin(cv, rgb.x), LogC4Curve_logC4_lin(cv, rgb.y),
                       LogC4Curve_logC4_lin(cv, rgb.z));
}

// convert xyz to logC3
__DEVICE__ float3
xyz_logC3(float3 rgb)
{
    struct LogCColorspace cs = logC3_colorspace();
    return LogCColorspace_xyz_logC(cs, rgb);
}

// convert logC3 to xyz
__DEVICE__ float3
logC3_xyz(float3 rgb)
{
    struct LogCColorspace cs = logC3_colorspace();
    return LogCColorspace_logC_xyz(cs, rgb);
}

// convert xyz to logC4
__DEVICE__ float3
xyz_logC4(float3 rgb)
{
    struct LogCColorspace cs = logC4_colorspace();
    return LogCColorspace_xyz_logC(cs, rgb);
}

// convert logc4 to xyz
__DEVICE__ float3
logC4_xyz(float3 rgb)
{
    struct LogCColorspace cs = logC4_colorspace();
    return LogCColorspace_logC_xyz(cs, rgb);
}
