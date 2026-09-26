// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// Gamma transfer curve.
struct GammaCurve {
    float exp;
};

__DEVICE__ struct GammaCurve
gamma_curve(float exp)
{
    struct GammaCurve cv;
    cv.exp = exp;
    return cv;
}

__DEVICE__ float
GammaCurve_lin_gamma(struct GammaCurve cv, float lin)
{
    return pow_f(max_f(lin, 0.0f), 1.0f / cv.exp);
}

__DEVICE__ float
GammaCurve_gamma_lin(struct GammaCurve cv, float value)
{
    return pow_f(max_f(value, 0.0f), cv.exp);
}

// linear -> gamma
__DEVICE__ float3
lin_gamma(float3 rgb, float exp)
{
    struct GammaCurve cv = gamma_curve(exp);
    return make_float3(GammaCurve_lin_gamma(cv, rgb.x), GammaCurve_lin_gamma(cv, rgb.y),
                       GammaCurve_lin_gamma(cv, rgb.z));
}

// gamma -> linear
__DEVICE__ float3
gamma_lin(float3 rgb, float exp)
{
    struct GammaCurve cv = gamma_curve(exp);
    return make_float3(GammaCurve_gamma_lin(cv, rgb.x), GammaCurve_gamma_lin(cv, rgb.y),
                       GammaCurve_gamma_lin(cv, rgb.z));
}

// linear -> gamma 2.2
__DEVICE__ float3
lin_gamma22(float3 rgb)
{
    return lin_gamma(rgb, 2.2f);
}

// gamma 2.2 -> linear
__DEVICE__ float3
gamma22_lin(float3 rgb)
{
    return gamma_lin(rgb, 2.2f);
}

// linear -> gamma 2.4
__DEVICE__ float3
lin_gamma24(float3 rgb)
{
    return lin_gamma(rgb, 2.4f);
}

// gamma 2.4 -> linear
__DEVICE__ float3
gamma24_lin(float3 rgb)
{
    return gamma_lin(rgb, 2.4f);
}

// linear -> gamma 2.6
__DEVICE__ float3
lin_gamma26(float3 rgb)
{
    return lin_gamma(rgb, 2.6f);
}

// gamma 2.6 -> linear
__DEVICE__ float3
gamma26_lin(float3 rgb)
{
    return gamma_lin(rgb, 2.6f);
}
