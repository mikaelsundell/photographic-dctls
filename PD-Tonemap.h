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

// ootf forward
__DEVICE__ float3 tonemap_ootf_dcip3_rec709(float3 log) {
    log = lin_rec709(dcip3_lin(log));
    return pow_f3(log, 2.2 / 2.6); // purely experiemental dcip3 to rec709
}

// ootf inverse
__DEVICE__ float3 tonemap_ootf_rec709_dcip3(float3 log) {
    log = lin_dcip3(rec709_lin(log));
    return pow_f3(log, 2.6 / 2.2); // purely experiemental rec709 to dcip3
}

// tonecompress reinhard
__DEVICE__ float3 tonecompress_reinhard(float3 lin, float exp) {
    return lin / (1.0 + lin * exp);
}

__DEVICE__ float3 tonecompress_inverse_reinhard(float3 compressed, float exp) {
    return compressed / (1.0 - compressed * exp);
}