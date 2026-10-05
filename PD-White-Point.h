// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

__DEVICE__ struct Matrix
whitepoint_d60_d65_adaptation()
{
    struct Matrix mt;
    // colortool --inputilluminant D60 --targetilluminant D65
    // cat02 adaptation matrix
    mt.m00 = 0.988254f;
    mt.m01 = -0.007883f;
    mt.m02 = 0.016710f;
    mt.m03 = -0.005691f;
    mt.m04 = 0.998709f;
    mt.m05 = 0.006653f;
    mt.m06 = 0.000352f;
    mt.m07 = 0.001120f;
    mt.m08 = 1.077838f;
    return mt;
}

__DEVICE__ struct Matrix
whitepoint_d63_d65_adaptation()
{
    struct Matrix mt;
    // colortool --inputilluminant D63 --targetilluminant D65
    // cat02 adaptation matrix
    mt.m00 = 1.009538f;
    mt.m01 = 0.026971f;
    mt.m02 = 0.021311f;
    mt.m03 = 0.018802f;
    mt.m04 = 0.975347f;
    mt.m05 = 0.008207f;
    mt.m06 = 0.000134f;
    mt.m07 = 0.002175f;
    mt.m08 = 1.138405f;
    return mt;
}

__DEVICE__ struct Matrix
whitepoint_d65_d60_adaptation()
{
    struct Matrix mt;
    // colortool --inputilluminant D65 --targetilluminant D60
    // cat02 adaptation matrix
    mt.m00 = 1.011938f;
    mt.m01 = 0.008005f;
    mt.m02 = -0.015737f;
    mt.m03 = 0.005769f;
    mt.m04 = 1.001345f;
    mt.m05 = -0.006271f;
    mt.m06 = -0.000337f;
    mt.m07 = -0.001043f;
    mt.m08 = 0.927795f;
    return mt;
}

__DEVICE__ struct Matrix
whitepoint_d65_d63_adaptation()
{
    struct Matrix mt;
    // colortool --inputilluminant D65 --targetilluminant D63
    // cat02 adaptation matrix
    mt.m00 = 0.991064f;
    mt.m01 = -0.027365f;
    mt.m02 = -0.018356f;
    mt.m03 = -0.019104f;
    mt.m04 = 1.025820f;
    mt.m05 = -0.007038f;
    mt.m06 = -0.000080f;
    mt.m07 = -0.001957f;
    mt.m08 = 0.878438f;
    return mt;
}

// whitepoint adaptation
__DEVICE__ float3
d60_d65_adaptation(float3 xyz)
{
    struct Matrix mt = whitepoint_d60_d65_adaptation();
    float3 result = mult_matrix(xyz, mt);
    return result;
}

__DEVICE__ float3
d63_d65_adaptation(float3 xyz)
{
    struct Matrix mt = whitepoint_d63_d65_adaptation();
    float3 result = mult_matrix(xyz, mt);
    return result;
}

__DEVICE__ float3
d65_d60_adaptation(float3 xyz)
{
    struct Matrix mt = whitepoint_d65_d60_adaptation();
    float3 result = mult_matrix(xyz, mt);
    return result;
}

__DEVICE__ float3
d65_d63_adaptation(float3 xyz)
{
    struct Matrix mt = whitepoint_d65_d63_adaptation();
    float3 result = mult_matrix(xyz, mt);
    return result;
}
