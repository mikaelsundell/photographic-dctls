// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

__DEVICE__ float
xyz_lab_f(float t)
{
    const float epsilon = 216.0f / 24389.0f;
    const float kappa = 24389.0f / 27.0f;

    return t > epsilon ? pow_f(t, 1.0f / 3.0f) : (kappa * t + 16.0f) / 116.0f;
}

__DEVICE__ float3
xyz_lab_d65(float3 xyz)
{
    const float Xn = 0.95047f;
    const float Yn = 1.00000f;
    const float Zn = 1.08883f;

    float fx = xyz_lab_f(xyz.x / Xn);
    float fy = xyz_lab_f(xyz.y / Yn);
    float fz = xyz_lab_f(xyz.z / Zn);

    return make_float3(116.0f * fy - 16.0f, 500.0f * (fx - fy), 200.0f * (fy - fz));
}

__DEVICE__ float3
pack_lab_to_rgb(float3 lab)
{
    return make_float3(lab.x / 100.0f, (lab.y + 128.0f) / 255.0f, (lab.z + 128.0f) / 255.0f);
}
