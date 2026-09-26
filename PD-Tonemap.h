// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

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

// gamma 2.6 encoded -> Rec709 encoded
__DEVICE__ float3
tonemap_contrast_gamma26_rec709(float3 rgb)
{
    rgb = gamma26_lin(rgb);
    rgb = lin_rec709(rgb);
    return pow_f3(rgb, 2.2f / 2.6f);
}

// sRGB encoded -> Rec709 encoded
__DEVICE__ float3
tonemap_contrast_sRGB_rec709(float3 rgb)
{
    rgb = sRGBgamma22_lin(rgb);
    rgb = lin_rec709(rgb);
    return pow_f3(rgb, 2.2f / 2.4f);
}

// Rec709 encoded -> sRGB encoded
__DEVICE__ float3
tonemap_contrast_rec709_sRGB(float3 rgb)
{
    rgb = rec709_lin(rgb);
    rgb = lin_sRGBgamma22(rgb);
    return pow_f3(rgb, 2.4f / 2.2f);
}

// Rec709 encoded -> gamma 2.6 encoded
__DEVICE__ float3
tonemap_contrast_rec709_gamma26(float3 rgb)
{
    rgb = rec709_lin(rgb);
    rgb = lin_gamma26(rgb);
    return pow_f3(rgb, 2.6f / 2.2f);
}

// reinhard tone compression
__DEVICE__ float3
tonecompress_reinhard_simple(float3 lin, float exp)
{
    return lin / (1.0f + lin * exp);
}

__DEVICE__ float
tonecompress_reinhard_scalar(float Y, float exp)
{
    float Yp = Y / (1.0f + exp * Y);
    float denom = max_f(Y, 1e-6f);  // avoid divide-by-zero
    return Yp / denom;
}

__DEVICE__ float3
tonecompress_reinhard_luma(float3 rgb, float3 w, float exp, float sat)
{
    float Y = rgb.x * w.x + rgb.y * w.y + rgb.z * w.z;

    // luma-preserving scale
    float s = tonecompress_reinhard_scalar(Y, exp);
    float3 rgblp = rgb * s;

    // neutral at compressed luminance
    float Yp = s * Y;
    float3 gray = make_float3(Yp, Yp, Yp);

    // reduce saturation with compression
    float satmix = pow_f(s, sat);

    return gray + (rgblp - gray) * satmix;
}
