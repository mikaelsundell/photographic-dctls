// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

struct AppleLogCurve {
    float r0;
    float rt;
    float c;
    float beta;
    float gamma;
    float delta;
    float log_cut;
};

__DEVICE__ struct AppleLogCurve
applelog_curve()
{
    struct AppleLogCurve cv;
    cv.r0 = -0.05641088f;
    cv.rt = 0.01000000f;
    cv.c = 47.28711236f;
    cv.beta = 0.00964052f;
    cv.gamma = 0.08550479f;
    cv.delta = 0.69336945f;
    cv.log_cut = cv.c * (cv.rt - cv.r0) * (cv.rt - cv.r0);
    return cv;
}

__DEVICE__ float
AppleLogCurve_lin_applelog(struct AppleLogCurve cv, float lin)
{
    return lin >= cv.rt ? cv.gamma * log2_f(max_f(lin + cv.beta, 1e-8f)) + cv.delta
                        : cv.c * (lin - cv.r0) * (lin - cv.r0);
}

__DEVICE__ float
AppleLogCurve_applelog_lin(struct AppleLogCurve cv, float value)
{
    return value >= cv.log_cut ? pow_f(2.0f, (value - cv.delta) / cv.gamma) - cv.beta
                               : cv.r0 + pow_f(max_f(value / cv.c, 0.0f), 0.5f);
}

// linear -> Apple Log
__DEVICE__ float3
lin_applelog(float3 rgb)
{
    struct AppleLogCurve cv = applelog_curve();
    return make_float3(AppleLogCurve_lin_applelog(cv, rgb.x), AppleLogCurve_lin_applelog(cv, rgb.y),
                       AppleLogCurve_lin_applelog(cv, rgb.z));
}

// Apple Log -> linear
__DEVICE__ float3
applelog_lin(float3 rgb)
{
    struct AppleLogCurve cv = applelog_curve();
    return make_float3(AppleLogCurve_applelog_lin(cv, rgb.x), AppleLogCurve_applelog_lin(cv, rgb.y),
                       AppleLogCurve_applelog_lin(cv, rgb.z));
}
