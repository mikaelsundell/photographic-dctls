// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// tonemap curve
struct TonemapCurve
{
    float low;
    float lowsoft;
    float high;
    float highsoft;
    float shoulder;
    float toe;
};

__DEVICE__ struct TonemapCurve tonemap_curve() {
    struct TonemapCurve tm;
    tm.low = 0.300;
    tm.lowsoft = 0.200;
    tm.high = 0.350;
    tm.highsoft = 0.200;
    tm.shoulder = 0.5;
    tm.toe = 0.4;
    return tm;
}

__DEVICE__ float tonemap_pow(float x, float c, float p) {
   return pow(x, c) / pow(p, c - 1.0);
}

__DEVICE__ float tonemap_scurve(float x, float p, float c) {
   if (x < p) {
       return tonemap_pow(x, c, p);
   } 
   else {
       return 1.0 - tonemap_pow(1.0 - x, c, 1.0 - p);
   }
}

// ootf
__DEVICE__ float3 tonemap_ootf_dcip3_rec709(float3 log) {
    log = lin_rec709gamma24(dcip3gamma26_lin(log));
    return pow_f3(log, 2.2 / 2.6); // purely experiemental dcip3 to rec709
}

__DEVICE__ float3 tonemap_ootf_sRGB_rec709(float3 log) {
    log = lin_rec709gamma24(sRGBgamma22_lin(log));
    return pow_f3(log, 2.2 / 2.4); // purely experiemental sRGB to rec709
}

__DEVICE__ float3 tonemap_ootf_rec709_sRGB(float3 log) {
    log = lin_sRGBgamma22(rec709gamma24_lin(log));
    return pow_f3(log, 2.4 / 2.2); // purely experiemental rec709 to srgb
}

__DEVICE__ float3 tonemap_ootf_rec709_dcip3(float3 log) {
    log = lin_dcip3gamma26(rec709gamma24_lin(log));
    return pow_f3(log, 2.6 / 2.2); // purely experiemental rec709 to dcip3
}

// tonecompress reinhard
__DEVICE__ float3 tonecompress_reinhard_simple(float3 lin, float exp) {
    return lin / (1.0 + lin * exp);
}

__DEVICE__ float tonecompress_reinhard_scalar(float Y, float exp) {
    float Yp = Y / (1.0f + exp * Y);
    float denom = max_f(Y, 1e-6f);        // safety for very small/odd Ys
    return Yp / denom;                    // s = Y'/Y  in (0,1]
}

__DEVICE__ float3 tonecompress_reinhard_luma(float3 rgb, float3 w, float exp, float sat)
{
    float  Y = rgb.x*w.x + rgb.y*w.y + rgb.z*w.z;
    // luma-preserving scale
    float  s  = tonecompress_reinhard_scalar(Y, exp);
    float3 rgblp = rgb * s;
    // build a neutral of the *new* luminance Y' = s*Y.
    // gray is (g,g,g) and w.x+w.y+w.z == 1, so (Y',Y',Y') has luminance Y'.
    float Yp = s * Y;
    float3 gray = make_float3(Yp, Yp, Yp);
    // desaturate into the shoulder proportional to compression.
    // sat_mix in [0..1]: 1 => keep chroma; 0 => fully gray.
    // using pow(s, kSat) keeps midtones intact and only tames compressed highs.
    float satmix = pow_f(s, sat);
    // blend between gray and luma-preserved RGB
    return gray + (rgblp - gray) * satmix;
}
