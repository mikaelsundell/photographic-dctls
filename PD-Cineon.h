// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// cineonColor
struct CineonColor
{
    float stop;
    float r;
    float g;
    float b;
};

__CONSTANT__ struct CineonColor cineon_colors[] = {
    {-8, 0.01f, 0.01f, 0.01f},
    {-7, 0.05f, 0.05f, 0.05f},
    {-6, 0.10f, 0.10f, 0.10f},
    {-5, 0.40f, 0.25f, 0.60f},
    {-4, 0.20f, 0.45f, 0.70f},
    {-3, 0.40f, 0.60f, 0.95f},
    {-2, 0.40f, 0.60f, 0.25f},
    {-1, 0.60f, 0.90f, 0.55f},
    {0,  0.50f, 0.50f, 0.50f},
    {1,  1.00f, 0.95f, 0.25f},
    {2,  0.90f, 0.50f, 0.25f},
    {3,  0.90f, 0.60f, 0.25f},
    {4,  0.90f, 0.30f, 0.20f},
    {5,  0.90f, 0.35f, 0.30f},
    {6,  0.90f, 0.90f, 0.90f},
    {7,  0.95f, 0.95f, 0.95f},
    {8,  0.99f, 0.99f, 0.99f}
};
#define cineon_stops 17

// Cineon curve
struct CineonCurve
{
    float density;
    float gamma;
    float bitdepth;
    int offset;
    int white;
};

__DEVICE__ struct CineonCurve cineon_curve() {
    struct CineonCurve cv;
    cv.density = 2.046; cv.gamma = 0.6; cv.bitdepth = 1023; cv.offset = 95; cv.white = 685;
    return cv;
}

__DEVICE__  float CineonCurve_steps(struct CineonCurve cv) {
    return (cv.density / cv.bitdepth);
}

__DEVICE__ float3 CineonCurve_invert_cineon(struct CineonCurve cv, float3 rgb, float3 scale, float3 dmin) {
    float3 invert = make_float3(cv.offset, cv.offset, cv.offset);
    invert += scale * log10_f3(div_3f(dmin, rgb)) / CineonCurve_steps(cv);
    invert /= cv.bitdepth;
    return invert;
}

__DEVICE__ float3 CineonCurve_reverse_cineon(struct CineonCurve cv, float3 rgb, float3 scale, float3 dmin) {
    float3 reverse = rgb;
    reverse *= cv.bitdepth;
    reverse -= make_float3(cv.offset, cv.offset, cv.offset);
    float3 scaled = reverse * CineonCurve_steps(cv) / scale;
    rgb = dmin / pow10_f3(scaled);
    return rgb;
}

__DEVICE__ float3 CineonCurve_lin_cineon(struct CineonCurve cv, float3 rgb) {
    float scale = CineonCurve_steps(cv) / cv.gamma;
    float gain = 1.0 - pow10_f((cv.offset - cv.white) * scale);
    float offset = cv.white / cv.bitdepth;
    float r = offset + log_f(max_f((rgb.x - 1.0f) * gain + 1.0, 1e-7)) / (cv.bitdepth * log_f(10) * scale);
    float g = offset + log_f(max_f((rgb.y - 1.0f) * gain + 1.0, 1e-7)) / (cv.bitdepth * log_f(10) * scale);
    float b = offset + log_f(max_f((rgb.z - 1.0f) * gain + 1.0, 1e-7)) / (cv.bitdepth * log_f(10) * scale);
    rgb = make_float3(r, g, b);
    return rgb;
}

__DEVICE__ float3 CineonCurve_cineon_lin(struct CineonCurve cv, float3 rgb) {
    float scale = CineonCurve_steps(cv) / cv.gamma;
    rgb = min_f3(rgb, 1.0f) * cv.bitdepth;
    float black = pow_f(10.0f, cv.offset * scale);
    float diff = pow_f(10.0f, cv.white * scale) - black;
    float r = pow_f(10.0f, rgb.x * scale);
    float g = pow_f(10.0f, rgb.y * scale);
    float b = pow_f(10.0f, rgb.z * scale);
    rgb = (make_float3(r, g, b) - black) / diff;
    return rgb;
}

// invert negative to cineon
__DEVICE__ float3 invert_cineon(float3 rgb, float3 scale, float3 dmin) {
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_invert_cineon(cv, rgb, scale, dmin);
}

// reverse negative to cineon
__DEVICE__ float3 reverse_cineon(float3 rgb, float3 scale, float3 dmin) {
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_reverse_cineon(cv, rgb, scale, dmin);
}

// convert linear to cineon
__DEVICE__ float3 lin_cineon(float3 rgb) {
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_lin_cineon(cv, rgb);
}

// convert cineon to linear
__DEVICE__ float3 cineon_lin(float3 rgb) {
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_cineon_lin(cv, rgb);
}
