// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

__DEVICE__ struct Matrix whitepoint_d60_d65_adaptation() {
    struct Matrix mt;
    // colortool --inputilluminant D60 --targetilluminant D65
    // cat02 adaptation matrix
    mt.m00 = 0.988254; mt.m01 = -0.007883; mt.m02 = 0.016710;
    mt.m03 = -0.005691; mt.m04 = 0.998709; mt.m05 = 0.006653;
    mt.m06 = 0.000352; mt.m07 = 0.001120; mt.m08 = 1.077838;
    return mt;
}

__DEVICE__ struct Matrix whitepoint_d63_d65_adaptation() {
    struct Matrix mt;
    // colortool --inputilluminant D63 --targetilluminant D65
    // cat02 adaptation matrix
    mt.m00 = 1.009538; mt.m01 = 0.026971; mt.m02 = 0.021311;
    mt.m03 = 0.018802; mt.m04 = 0.975347; mt.m05 = 0.008207;
    mt.m06 = 0.000134; mt.m07 = 0.002175; mt.m08 = 1.138405;
    return mt;
} 

__DEVICE__ struct Matrix whitepoint_d65_d60_adaptation() {
    struct Matrix mt;
    // colortool --inputilluminant D65 --targetilluminant D60
    // cat02 adaptation matrix
    mt.m00 = 1.011938; mt.m01 = 0.008005; mt.m02 = -0.015737;
    mt.m03 = 0.005769; mt.m04 = 1.001345; mt.m05 = -0.006271;
    mt.m06 = -0.000337; mt.m07 = -0.001043; mt.m08 = 0.927795;
    return mt;
}

__DEVICE__ struct Matrix whitepoint_d65_d63_adaptation() {
    struct Matrix mt;
    // colortool --inputilluminant D65 --targetilluminant D63
    // cat02 adaptation matrix
    mt.m00 = 0.991064; mt.m01 = -0.027365; mt.m02 = -0.018356;
    mt.m03 = -0.019104; mt.m04 = 1.025820; mt.m05 = -0.007038;
    mt.m06 = -0.000080; mt.m07 = -0.001957; mt.m08 = 0.878438;
    return mt;
}

// whitepoint adaptation
__DEVICE__ float3 d60_d65_adaptation(float3 rgb) {
    struct Matrix mt = whitepoint_d60_d65_adaptation();
    float3 result = mult_matrix(rgb, mt);
    return result;
}

__DEVICE__ float3 d63_d65_adaptation(float3 rgb) {
    struct Matrix mt = whitepoint_d63_d65_adaptation();
    float3 result = mult_matrix(rgb, mt);
    return result;
}

__DEVICE__ float3 d65_d60_adaptation(float3 rgb) {
    struct Matrix mt = whitepoint_d65_d60_adaptation();
    float3 result = mult_matrix(rgb, mt);
    return result;
}

__DEVICE__ float3 d65_d63_adaptation(float3 rgb) {
    struct Matrix mt = whitepoint_d65_d63_adaptation();
    float3 result = mult_matrix(rgb, mt);
    return result;
}
