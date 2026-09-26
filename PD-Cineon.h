// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// Cineon curve and false-color stops.

struct CineonColor {
    float stop;
    float r;
    float g;
    float b;
};

__CONSTANT__ struct CineonColor cineon_colors[]
    = { { -8.0f, 0.01f, 0.01f, 0.01f }, { -7.0f, 0.05f, 0.05f, 0.05f }, { -6.0f, 0.10f, 0.10f, 0.10f },
        { -5.0f, 0.40f, 0.25f, 0.60f }, { -4.0f, 0.20f, 0.45f, 0.70f }, { -3.0f, 0.40f, 0.60f, 0.95f },
        { -2.0f, 0.40f, 0.60f, 0.25f }, { -1.0f, 0.60f, 0.90f, 0.55f }, { 0.0f, 0.50f, 0.50f, 0.50f },
        { 1.0f, 1.00f, 0.95f, 0.25f },  { 2.0f, 0.90f, 0.50f, 0.25f },  { 3.0f, 0.90f, 0.60f, 0.25f },
        { 4.0f, 0.90f, 0.30f, 0.20f },  { 5.0f, 0.90f, 0.35f, 0.30f },  { 6.0f, 0.90f, 0.90f, 0.90f },
        { 7.0f, 0.95f, 0.95f, 0.95f },  { 8.0f, 0.99f, 0.99f, 0.99f } };

#define cineon_stops 17

struct CineonCurve {
    float density;
    float gamma;
    float bitdepth;
    int offset;
    int white;
};

__DEVICE__ struct CineonCurve
cineon_curve()
{
    struct CineonCurve cv;
    cv.density = 2.046f;
    cv.gamma = 0.6f;
    cv.bitdepth = 1023.0f;
    cv.offset = 95;
    cv.white = 685;
    return cv;
}

__DEVICE__ float
CineonCurve_steps(struct CineonCurve cv)
{
    return cv.density / cv.bitdepth;
}

__DEVICE__ float3
CineonCurve_invert_cineon(struct CineonCurve cv, float3 rgb, float3 scale, float3 dmin)
{
    float3 result = make_float3((float)cv.offset, (float)cv.offset, (float)cv.offset);
    result += scale * log10_f3(div_3f(dmin, rgb)) / CineonCurve_steps(cv);
    result /= cv.bitdepth;
    return result;
}

__DEVICE__ float3
CineonCurve_reverse_cineon(struct CineonCurve cv, float3 rgb, float3 scale, float3 dmin)
{
    float3 result = rgb;
    result *= cv.bitdepth;
    result -= make_float3((float)cv.offset, (float)cv.offset, (float)cv.offset);
    float3 scaled = result * CineonCurve_steps(cv) / scale;
    return dmin / pow10_f3(scaled);
}

__DEVICE__ float3
CineonCurve_lin_cineon(struct CineonCurve cv, float3 rgb)
{
    const float eps = 1e-7f;
    float scale = CineonCurve_steps(cv) / cv.gamma;
    float gain = 1.0f - pow10_f(((float)cv.offset - (float)cv.white) * scale);
    float offset = (float)cv.white / cv.bitdepth;

    float r = offset + log_f(max_f((rgb.x - 1.0f) * gain + 1.0f, eps)) / (cv.bitdepth * log_f(10.0f) * scale);
    float g = offset + log_f(max_f((rgb.y - 1.0f) * gain + 1.0f, eps)) / (cv.bitdepth * log_f(10.0f) * scale);
    float b = offset + log_f(max_f((rgb.z - 1.0f) * gain + 1.0f, eps)) / (cv.bitdepth * log_f(10.0f) * scale);

    return make_float3(r, g, b);
}

__DEVICE__ float3
CineonCurve_cineon_lin(struct CineonCurve cv, float3 rgb)
{
    float scale = CineonCurve_steps(cv) / cv.gamma;
    rgb = min_f3(rgb, 1.0f) * cv.bitdepth;

    float black = pow10_f((float)cv.offset * scale);
    float diff = pow10_f((float)cv.white * scale) - black;
    float r = pow10_f(rgb.x * scale);
    float g = pow10_f(rgb.y * scale);
    float b = pow10_f(rgb.z * scale);

    return (make_float3(r, g, b) - black) / diff;
}

// invert negative -> Cineon
__DEVICE__ float3
invert_cineon(float3 rgb, float3 scale, float3 dmin)
{
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_invert_cineon(cv, rgb, scale, dmin);
}

// reverse Cineon -> negative
__DEVICE__ float3
reverse_cineon(float3 rgb, float3 scale, float3 dmin)
{
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_reverse_cineon(cv, rgb, scale, dmin);
}

// linear -> Cineon
__DEVICE__ float3
lin_cineon(float3 rgb)
{
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_lin_cineon(cv, rgb);
}

// Cineon -> linear
__DEVICE__ float3
cineon_lin(float3 rgb)
{
    struct CineonCurve cv = cineon_curve();
    return CineonCurve_cineon_lin(cv, rgb);
}
