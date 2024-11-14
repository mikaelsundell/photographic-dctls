// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

__DEVICE__ struct Matrix d50_d65_adaptation() {
    struct Matrix mt;
    // colortool --inputilluminant D50 --targetilluminant D65
    // cat02 adaptation matrix
    mt.cat02_matrix.m00 = 0.959929; mt.cat02_matrix.m01 = -0.029309; mt.cat02_matrix.m02 = 0.065638;
    mt.cat02_matrix.m03 = -0.021189; mt.cat02_matrix.m04 = 0.998874; mt.cat02_matrix.m05 = 0.026123;
    mt.cat02_matrix.m06 = 0.001370; mt.cat02_matrix.m07 = 0.004434; mt.cat02_matrix.m08 = 1.312489;
    return ct;
}  

// cat02 adaptation
__DEVICE__ float3 D50_D65_adaptation(float3 rgb, float kelvin) {
    struct Matrix mt = d50_d65_adaptation();
    float3 result = mult_matrix(rgb, mt);
    return result;
}
