// Copyright 2022-present Contributors to the dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/dctl

// clang-format on

// roi
struct ROI
{
    int x1, x2, y1, y2;
};

// matrix
struct Matrix
{
    float m00, m01, m02;
    float m03, m04, m05;
    float m06, m07, m08;
};

// math
__DEVICE__ float clamp_f(float value, float min, float max) {
    return _clampf(value);
}

__DEVICE__ float exp_f(float value) {
    return _expf(value);
}

__DEVICE__ float mod_f(float value, float y) {
    return _fmod(value, y);
}

__DEVICE__ float abs_f(float value) {
    return _fabs(value);
}

__DEVICE__ float3 abs_f3(float3 value) {
    return make_float3(abs_f(value.x), abs_f(value.y), abs_f(value.z));
}

__DEVICE__ float log_f(float value) {
    return _logf(value);
}

__DEVICE__ float3 log_f3(float3 value) {
    return make_float3(log_f(value.x), log_f(value.y), log_f(value.z));
}

__DEVICE__ float log10_f(float value) {
    return _log10f(value);
}

__DEVICE__ float3 log10_f3(float3 value) {
    return make_float3(log10_f(value.x), log10_f(value.y), log10_f(value.z));
}

__DEVICE__ float pow10_f(float value) {
    return _powf(10.0, value);
}

__DEVICE__ float3 pow10_f3(float3 value) {
    return make_float3(pow10_f(value.x), pow10_f(value.y), pow10_f(value.z));
}

__DEVICE__ float pow_f(float value, float m) {
    return _powf(value, m);
}

__DEVICE__ float3 pow_f3(float3 value, float m) {
    return make_float3(pow_f(value.x, m), pow_f(value.y, m), pow_f(value.z, m));
}

__DEVICE__ float min_f(float value, float m) {
    return _fminf(value, m);
}

__DEVICE__ float3 min_f3(float3 value, float m) {
    return make_float3(min_f(value.x, m), min_f(value.y, m), min_f(value.z, m));
}

__DEVICE__ float max_f(float value, float m) {
    return _fmaxf(value, m);
}

__DEVICE__ float3 max_f3(float3 value, float m) {
    return make_float3(max_f(value.x, m), max_f(value.y, m), max_f(value.z, m));
}

__DEVICE__ float mix_f(float x, float y, float a) {
    return x * (1.0 - a) + y * a;
}

__DEVICE__ float3 mix_f3(float3 x, float3 y, float a) {
    return make_float3(mix_f(x.x, y.x, a), mix_f(x.y, y.y, a), mix_f(x.z, y.z, a));
}

__DEVICE__ float3 div_3f(float3 x, float3 y) {
    float eps = 1e-7f;
    float3 ey = max_f3(y, eps);
    return x / ey;
}

// Matrix math
__DEVICE__ float3 mult_matrix(float3 value, struct Matrix mat) {
    float3 result = make_float3(
        mat.m00 * value.x + mat.m01 * value.y + mat.m02 * value.z,
        mat.m03 * value.x + mat.m04 * value.y + mat.m05 * value.z,
        mat.m06 * value.x + mat.m07 * value.y + mat.m08 * value.z
    );
    return result;
}

// Convert hsv to rgb
__DEVICE__ float3 hsv_rgb(float3 hsv) {
    float hue = hsv.x;
    float sat = hsv.y;
    float val = hsv.z;
    hue = mod_f(hue + 360.0, 360.0);
    float c = val * sat;
    float x = c * (1.0 - abs_f(mod_f(hue / 60.0, 2.0) - 1.0));
    float m = val - c;
    float3 rgbp = make_float3(0.0, 0.0, 0.0);
    if (0.0 <= hue && hue < 60.0) {
        rgbp = make_float3(c, x, 0.0);
    }
    else if (60.0 <= hue && hue < 120.0) {
        rgbp = make_float3(x, c, 0.0);
    }
    else if (120.0 <= hue && hue < 180.0) {
        rgbp = make_float3(0.0, c, x);
    }
    else if (180.0 <= hue && hue < 240.0) {
        rgbp = make_float3(0.0, x, c);
    }
    else if (240.0 <= hue && hue < 300.0) {
        rgbp = make_float3(x, 0.0, c);
    }
    else if (300.0 < hue && hue < 360.0) {
        rgbp = make_float3(c, 0.0, x);
    }
    return rgbp + m;
}

// Convert rgb to hsv
__DEVICE__ float3 rgb_hsv(float3 rgb) {
    float r = rgb.x;
    float g = rgb.y;
    float b = rgb.z;
    float c_max = max_f(max_f(r, g), b);
    float c_min = min_f(min_f(r, g), b);
    float delta = c_max - c_min;
    float H;
    if (delta == 0.0f) {
        H = 0.0f;
    }
    else if (r >= g && r >= b) {
        H = mod_f((g - b) / delta + 6.0f, (6.0f));
    }
    else if (g >= r && g >= b) {
        H = (b - r) / delta + (2.0f);
    }
    else {
        H = (r - g) / delta + 4.0f;
    }
    H = H / 6.0f;
    float S;
    if (c_max == 0.0f) {
        S = 0.0f;
    } else {
        S = delta / c_max;
    }
    float V = c_max;
    float3 color = make_float3(H * 360.0, S, V);
    return color;
}

// Convert hsl to rgb
__DEVICE__ float3 hsl_rgb(float3 hsl) {
    float h = hsl.x / 360.0f; // Convert h to [0, 1] range
    float s = hsl.y;
    float l = hsl.z;
    float c = (1.0f - abs_f(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - abs_f(mod_f(h * 6.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;
    float3 rgb = make_float3(0.0f, 0.0f, 0.0f);
    if (0.0f <= h && h < 1.0f/6.0f) {
        rgb = make_float3(c, x, 0.0f);
    }
    else if (1.0f/6.0f <= h && h < 2.0f/6.0f) {
        rgb = make_float3(x, c, 0.0f);
    }
    else if (2.0f/6.0f <= h && h < 3.0f/6.0f) {
        rgb = make_float3(0.0f, c, x);
    }
    else if (3.0f/6.0f <= h && h < 4.0f/6.0f) {
        rgb = make_float3(0.0f, x, c);
    }
    else if (4.0f/6.0f <= h && h < 5.0f/6.0f) {
        rgb = make_float3(x, 0.0f, c);
    }
    else if (5.0f/6.0f <= h && h < 1.0f) {
        rgb = make_float3(c, 0.0f, x);
    }
    rgb += m;
    return rgb;
}

// Convert rgb to hsl
__DEVICE__ float3 rgb_hsl(float3 rgb) {
    float r = rgb.x;
    float g = rgb.y;
    float b = rgb.z;
    float max = max_f(max_f(r, g), b);
    float min = min_f(min_f(r, g), b);
    float h, s, l;
    l = (max + min) / 2.0f;
    float delta = max - min;
    if (delta == 0) {
        h = s = 0; // achromatic
    }
    else {
        s = l > 0.5f ? delta / (2.0f - max - min) : delta / (max + min);
        if (max == r) {
            h = (g - b) / delta + (g < b ? 6.0f : 0);
        } else if (max == g) {
            h = (b - r) / delta + 2.0f;
        } else {
            h = (r - g) / delta + 4.0f;
        }
        h /= 6.0f;
    }
    return make_float3(h * 360.0f, s, l); // H in [0, 360], S and L in [0, 1]
}

// Convert to luma in rec601
__DEVICE__ float luma_rec601(float3 rgb)
{
    float luma = 0.299f * rgb.x + 0.587f * rgb.y + 0.114f * rgb.z;
    return luma;
}

// Convert to luma in rec709
__DEVICE__ float luma_rec709(float3 rgb)
{
    float luma = 0.2126f * rgb.x + 0.7152f * rgb.y + 0.0722f * rgb.z;
    return luma;
}

// Convert to luma in rec2100
__DEVICE__ float luma_rec2100(float3 rgb)
{
    float luma = 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
    return luma;
}

// Adjust to luma in rec601
__DEVICE__ float3 adjust_luma_rec601(float3 rgb, float l)
{
    float diff = l - luma_rec601(rgb);
    float3 result = make_float3(rgb.x + diff, rgb.y + diff, rgb.z + diff);
    return result;
}

// Adjust to luma in rec709
__DEVICE__ float3 adjust_luma_rec709(float3 rgb, float l)
{
    float diff = l - luma_rec709(rgb);
    float3 result = make_float3(rgb.x + diff, rgb.y + diff, rgb.z + diff);
    return result;
}

// Adjust for display
__DEVICE__ float3 adjust_display(float3 rgb)
{
    return max_f3(rgb, 0.0f);
}
