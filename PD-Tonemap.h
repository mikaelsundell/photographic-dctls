// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// tonemap curve
struct TonemapCurve {
    float low;
    float lowsoft;
    float high;
    float highsoft;
    float shoulder;
    float toe;
};

__DEVICE__ struct TonemapCurve
tonemap_curve()
{
    struct TonemapCurve tm;
    tm.low = 0.300f;
    tm.lowsoft = 0.200f;
    tm.high = 0.350f;
    tm.highsoft = 0.200f;
    tm.shoulder = 0.5f;
    tm.toe = 0.4f;
    return tm;
}

__DEVICE__ float
tonemap_pow(float x, float c, float p)
{
    return pow_f(x, c) / pow_f(p, c - 1.0f);
}

__DEVICE__ float
tonemap_scurve(float x, float p, float c)
{
    if (x < p) {
        return tonemap_pow(x, c, p);
    }
    else {
        return 1.0f - tonemap_pow(1.0f - x, c, 1.0f - p);
    }
}

// experimental contrast / transfer-response shaping
//
// These functions reshape encoded RGB values between common display transfer
// responses for appearance matching. They do not perform RGB primary or gamut
// conversion. Colorspace conversion, when required, must be handled separately.

// gamma 2.6 encoded -> Rec709 encoded, with midtone response adjustment
__DEVICE__ float3
tonemap_contrast_gamma26_rec709(float3 rgb)
{
    rgb = gamma26_lin(rgb);           // decode gamma 2.6 to linear
    rgb = lin_rec709(rgb);            // encode with Rec709 transfer function
    return pow_f3(rgb, 2.2f / 2.6f);  // experimental appearance adjustment
}

// sRGB encoded -> Rec709 encoded, with midtone response adjustment
__DEVICE__ float3
tonemap_contrast_sRGB_rec709(float3 rgb)
{
    rgb = sRGBgamma22_lin(rgb);        // decode sRGB transfer function to linear
    rgb = lin_rec709(rgb);             // encode with Rec709 transfer function
    return pow_f3(rgb, 2.2f / 2.4f);   // experimental appearance adjustment
}

// Rec709 encoded -> sRGB encoded, with midtone response adjustment
__DEVICE__ float3
tonemap_contrast_rec709_sRGB(float3 rgb)
{
    rgb = rec709_lin(rgb);             // decode Rec709 transfer function to linear
    rgb = lin_sRGBgamma22(rgb);         // encode with sRGB transfer function
    return pow_f3(rgb, 2.4f / 2.2f);   // experimental appearance adjustment
}

// Rec709 encoded -> gamma 2.6 encoded, with midtone response adjustment
__DEVICE__ float3
tonemap_contrast_rec709_gamma26(float3 rgb)
{
    rgb = rec709_lin(rgb);             // decode Rec709 transfer function to linear
    rgb = lin_gamma26(rgb);            // encode with gamma 2.6
    return pow_f3(rgb, 2.6f / 2.2f);   // experimental appearance adjustment
}

// tonecompress reinhard
__DEVICE__ float3
tonecompress_reinhard_simple(float3 lin, float exp)
{
    return lin / (1.0f + lin * exp);
}

__DEVICE__ float
tonecompress_reinhard_scalar(float Y, float exp)
{
    float Yp = Y / (1.0f + exp * Y);
    float denom = max_f(Y, 1e-6f);  // safety for very small/odd Ys
    return Yp / denom;              // s = Y'/Y in 0-1
}

__DEVICE__ float3
tonecompress_reinhard_luma(float3 rgb, float3 w, float exp, float sat)
{
    float Y = rgb.x * w.x + rgb.y * w.y + rgb.z * w.z;
    // luma-preserving scale
    float s = tonecompress_reinhard_scalar(Y, exp);
    float3 rgblp = rgb * s;
    // build a neutral of the new luminance Y' = s * Y
    // gray is (g,g,g) and w.x+w.y+w.z == 1, so (Y',Y',Y') has luminance Y'
    float Yp = s * Y;
    float3 gray = make_float3(Yp, Yp, Yp);
    // desaturate into the shoulder proportional to compression
    // satmix in [0..1]: 1 => keep chroma; 0 => fully gray
    // using pow(s, sat) keeps midtones intact and only tames compressed highs
    float satmix = pow_f(s, sat);
    // blend between gray and luma-preserved RGB
    return gray + (rgblp - gray) * satmix;
}