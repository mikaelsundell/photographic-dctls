// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// dcip3 colorspace
struct DCIP3Colorspace {
    struct Matrix dcip3_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
DCIP3Colorspace_xyz_dcip3(struct DCIP3Colorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.dcip3_matrix);
}

__DEVICE__ float3
DCIP3Colorspace_dcip3_xyz(struct DCIP3Colorspace cs, float3 dcip3)
{
    return mult_matrix(dcip3, cs.xyz_matrix);
}

__DEVICE__ struct DCIP3Colorspace
dcip3_colorspace()
{
    struct DCIP3Colorspace cs;
    // colortool --inputcolorspace DCIP3 -v
    // convert xyz to dcip3 D63 matrix
    cs.dcip3_matrix.m00 = 2.7253940f;
    cs.dcip3_matrix.m01 = -1.0180030f;
    cs.dcip3_matrix.m02 = -0.4401632f;
    cs.dcip3_matrix.m03 = -0.7951680f;
    cs.dcip3_matrix.m04 = 1.6897321f;
    cs.dcip3_matrix.m05 = 0.0226472f;
    cs.dcip3_matrix.m06 = 0.0412419f;
    cs.dcip3_matrix.m07 = -0.0876390f;
    cs.dcip3_matrix.m08 = 1.1009294f;

    // convert dcip3 D63 to xyz matrix
    cs.xyz_matrix.m00 = 0.4451698f;
    cs.xyz_matrix.m01 = 0.2771344f;
    cs.xyz_matrix.m02 = 0.1722827f;
    cs.xyz_matrix.m03 = 0.2094917f;
    cs.xyz_matrix.m04 = 0.7215953f;
    cs.xyz_matrix.m05 = 0.0689131f;
    cs.xyz_matrix.m06 = 0.0000000f;
    cs.xyz_matrix.m07 = 0.0470606f;
    cs.xyz_matrix.m08 = 0.9073554f;

    return cs;
}

__DEVICE__ float3
dcip3_y_lum_coeff()
{
    struct DCIP3Colorspace cs = dcip3_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

__DEVICE__ struct DCIP3Colorspace
dcip3d60_colorspace()
{
    struct DCIP3Colorspace cs;
    // colortool --inputcolorspace DCIP3-D60 -v
    // convert xyz to dcip3 D60 matrix
    cs.dcip3_matrix.m00 = 2.4027414f;
    cs.dcip3_matrix.m01 = -0.8974842f;
    cs.dcip3_matrix.m02 = -0.3880534f;
    cs.dcip3_matrix.m03 = -0.8325796f;
    cs.dcip3_matrix.m04 = 1.7692318f;
    cs.dcip3_matrix.m05 = 0.0237127f;
    cs.dcip3_matrix.m06 = 0.0388234f;
    cs.dcip3_matrix.m07 = -0.0824997f;
    cs.dcip3_matrix.m08 = 1.0363686f;
    // convert dcip3 D60 to xyz matrix
    cs.xyz_matrix.m00 = 0.5049495f;
    cs.xyz_matrix.m01 = 0.2646815f;
    cs.xyz_matrix.m02 = 0.1830151f;
    cs.xyz_matrix.m03 = 0.2376233f;
    cs.xyz_matrix.m04 = 0.6891707f;
    cs.xyz_matrix.m05 = 0.0732060f;
    cs.xyz_matrix.m06 = 0.0000000f;
    cs.xyz_matrix.m07 = 0.0449459f;
    cs.xyz_matrix.m08 = 0.9638793f;
    return cs;
}

__DEVICE__ float3
dcip3d60_y_lum_coeff()
{
    struct DCIP3Colorspace cs = dcip3d60_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

__DEVICE__ struct DCIP3Colorspace
dcip3d65_colorspace()
{
    struct DCIP3Colorspace cs;
    // colortool --inputcolorspace DCIP3-D65 -v
    // convert xyz to dcip3 D65 matrix
    cs.dcip3_matrix.m00 = 2.4934969f;
    cs.dcip3_matrix.m01 = -0.9313836f;
    cs.dcip3_matrix.m02 = -0.4027108f;
    cs.dcip3_matrix.m03 = -0.8294890f;
    cs.dcip3_matrix.m04 = 1.7626641f;
    cs.dcip3_matrix.m05 = 0.0236247f;
    cs.dcip3_matrix.m06 = 0.0358458f;
    cs.dcip3_matrix.m07 = -0.0761724f;
    cs.dcip3_matrix.m08 = 0.9568845f;
    // convert dcip3 D65 to xyz matrix
    cs.xyz_matrix.m00 = 0.4865709f;
    cs.xyz_matrix.m01 = 0.2656677f;
    cs.xyz_matrix.m02 = 0.1982173f;
    cs.xyz_matrix.m03 = 0.2289746f;
    cs.xyz_matrix.m04 = 0.6917385f;
    cs.xyz_matrix.m05 = 0.0792869f;
    cs.xyz_matrix.m06 = 0.0000000f;
    cs.xyz_matrix.m07 = 0.0451134f;
    cs.xyz_matrix.m08 = 1.0439444f;
    return cs;
}

__DEVICE__ float3
dcip3d65_y_lum_coeff()
{
    struct DCIP3Colorspace cs = dcip3d65_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// convert xyz to dcip3
__DEVICE__ float3
xyz_dcip3(float3 rgb)
{
    struct DCIP3Colorspace cs = dcip3_colorspace();
    return DCIP3Colorspace_xyz_dcip3(cs, rgb);
}

// convert dcip3 to xyz
__DEVICE__ float3
dcip3_xyz(float3 rgb)
{
    struct DCIP3Colorspace cs = dcip3_colorspace();
    return DCIP3Colorspace_dcip3_xyz(cs, rgb);
}

// convert xyz to dcip3d60
__DEVICE__ float3
xyz_dcip3d60(float3 rgb)
{
    struct DCIP3Colorspace cs = dcip3d60_colorspace();
    return DCIP3Colorspace_xyz_dcip3(cs, rgb);
}

// convert dcip3d60 to xyz
__DEVICE__ float3
dcip3d60_xyz(float3 rgb)
{
    struct DCIP3Colorspace cs = dcip3d60_colorspace();
    return DCIP3Colorspace_dcip3_xyz(cs, rgb);
}

// convert xyz to dcip3d65
__DEVICE__ float3
xyz_dcip3d65(float3 rgb)
{
    struct DCIP3Colorspace cs = dcip3d65_colorspace();
    return DCIP3Colorspace_xyz_dcip3(cs, rgb);
}

// convert dcip3d65 to xyz
__DEVICE__ float3
dcip3d65_xyz(float3 rgb)
{
    struct DCIP3Colorspace cs = dcip3d65_colorspace();
    return DCIP3Colorspace_dcip3_xyz(cs, rgb);
}