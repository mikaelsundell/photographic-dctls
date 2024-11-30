// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// dcip3 curve
struct DCIP3Curve
{
    float exp;
};

__DEVICE__ struct DCIP3Curve dcip3_curve() {
    struct DCIP3Curve cv;
    cv.exp = 2.6;
    return cv;
}

__DEVICE__ float DCIP3Curve_lin_dcip3(struct DCIP3Curve cv, float lin) {
    return pow_f(lin, 1 / cv.exp);
}

__DEVICE__ float DCIP3Curve_dcip3_lin(struct DCIP3Curve cv, float val) {
    return pow_f(val, cv.exp);
}

// dcip3 colorspace
struct DCIP3Colorspace
{
    struct Matrix dcip3_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ struct DCIP3Colorspace dcip3_colorspace() {
    struct DCIP3Colorspace cs;
    // colortool --inputcolorspace DCIP3 -v
    // dcip3 matrix D63
    cs.dcip3_matrix.m00 = 2.7253940; cs.dcip3_matrix.m01 = -1.0180030; cs.dcip3_matrix.m02 = -0.4401632;
    cs.dcip3_matrix.m03 = -0.7951680; cs.dcip3_matrix.m04 = 1.6897321; cs.dcip3_matrix.m05 = 0.0226472;
    cs.dcip3_matrix.m06 = 0.0412419; cs.dcip3_matrix.m07 = -0.0876390; cs.dcip3_matrix.m08 =  1.1009294;
    // xyz matrix D63
    cs.xyz_matrix.m00 = 0.4451698; cs.xyz_matrix.m01 = 0.2771344; cs.xyz_matrix.m02 = 0.1722827;
    cs.xyz_matrix.m03 = 0.2094917; cs.xyz_matrix.m04 = 0.7215953; cs.xyz_matrix.m05 = 0.0689131;
    cs.xyz_matrix.m06 = 0.0000000; cs.xyz_matrix.m07 = 0.0470606; cs.xyz_matrix.m08 = 0.9073554;
    return cs;
}

__DEVICE__ struct DCIP3Colorspace dcip3d60_colorspace() {
    struct DCIP3Colorspace cs;
    // colortool --inputcolorspace DCIP3-D60 -v
    // dcip3 matrix D60
    cs.dcip3_matrix.m00 = 2.4027414; cs.dcip3_matrix.m01 = -0.8974842; cs.dcip3_matrix.m02 = -0.3880534;
    cs.dcip3_matrix.m03 = -0.8325796; cs.dcip3_matrix.m04 = 1.7692318; cs.dcip3_matrix.m05 = 0.0237127;
    cs.dcip3_matrix.m06 = 0.0388234; cs.dcip3_matrix.m07 = -0.0824997; cs.dcip3_matrix.m08 = 1.0363686;
    // xyz matrix D60
    cs.xyz_matrix.m00 = 0.5049495; cs.xyz_matrix.m01 = 0.2646815; cs.xyz_matrix.m02 = 0.1830151;
    cs.xyz_matrix.m03 = 0.2376233; cs.xyz_matrix.m04 = 0.6891707; cs.xyz_matrix.m05 = 0.0732060;
    cs.xyz_matrix.m06 = 0.0000000; cs.xyz_matrix.m07 = 0.0449459; cs.xyz_matrix.m08 = 0.9638793;
    return cs;
}

__DEVICE__ struct DCIP3Colorspace dcip3d65_colorspace() {
    struct DCIP3Colorspace cs;
    // colortool --inputcolorspace DCIP3-D65 -v
    // dcip3 matrix D65
    cs.dcip3_matrix.m00 = 2.4934969; cs.dcip3_matrix.m01 = -0.9313836; cs.dcip3_matrix.m02 = -0.4027108;
    cs.dcip3_matrix.m03 = -0.8294890; cs.dcip3_matrix.m04 = 1.7626641; cs.dcip3_matrix.m05 = 0.0236247;
    cs.dcip3_matrix.m06 = 0.0358458; cs.dcip3_matrix.m07 = -0.0761724; cs.dcip3_matrix.m08 = 0.9568845;
    // xyz matrix D65
    cs.xyz_matrix.m00 =  0.4865709; cs.xyz_matrix.m01 = 0.2656677; cs.xyz_matrix.m02 = 0.1982173;
    cs.xyz_matrix.m03 =  0.2289746; cs.xyz_matrix.m04 = 0.6917385; cs.xyz_matrix.m05 = 0.0792869;
    cs.xyz_matrix.m06 =  0.0000000; cs.xyz_matrix.m07 = 0.0451134; cs.xyz_matrix.m08 = 1.0439444;
    return cs;
}

__DEVICE__ float3 DCIP3Colorspace_xyz_dcip3(struct DCIP3Colorspace cs, float3 xyz) {
    return mult_matrix(xyz, cs.dcip3_matrix);
}
__DEVICE__ float3 DCIP3Colorspace_dcip3_xyz(struct DCIP3Colorspace cs, float3 dcip3) {
    return mult_matrix(dcip3, cs.xyz_matrix);
}

// convert linear to dcip3
__DEVICE__ float3 lin_dcip3gamma26(float3 rgb) {
    struct DCIP3Curve cv = dcip3_curve();
    return make_float3(DCIP3Curve_lin_dcip3(cv, rgb.x), DCIP3Curve_lin_dcip3(cv, rgb.y), DCIP3Curve_lin_dcip3(cv, rgb.z));
}

// convert dcip3 to linear
__DEVICE__ float3 dcip3gamma26_lin(float3 rgb) {
    struct DCIP3Curve cv = dcip3_curve();
    return make_float3(DCIP3Curve_dcip3_lin(cv, rgb.x), DCIP3Curve_dcip3_lin(cv, rgb.y), DCIP3Curve_dcip3_lin(cv, rgb.z));
}

// convert dcip3 to xyz
__DEVICE__ float3 xyz_dcip3(float3 rgb) {
    struct DCIP3Colorspace cs = dcip3_colorspace();
    return DCIP3Colorspace_xyz_dcip3(cs, rgb);
}

// convert xyz to dcip3
__DEVICE__ float3 dcip3_xyz(float3 rgb) {
    struct DCIP3Colorspace cs = dcip3_colorspace();
    return DCIP3Colorspace_dcip3_xyz(cs, rgb);
}

// convert dcip3d60 to xyz
__DEVICE__ float3 xyz_dcip3d60(float3 rgb) {
    struct DCIP3Colorspace cs = dcip3d60_colorspace();
    return DCIP3Colorspace_xyz_dcip3(cs, rgb);
}

// convert xyz to dcip3d60
__DEVICE__ float3 dcip3d60_xyz(float3 rgb) {
    struct DCIP3Colorspace cs = dcip3d60_colorspace();
    return DCIP3Colorspace_dcip3_xyz(cs, rgb);
}

// convert dcip3d65 to xyz
__DEVICE__ float3 xyz_dcip3d65(float3 rgb) {
    struct DCIP3Colorspace cs = dcip3d65_colorspace();
    return DCIP3Colorspace_xyz_dcip3(cs, rgb);
}

// convert xyz to dcip3d65
__DEVICE__ float3 dcip3d65_xyz(float3 rgb) {
    struct DCIP3Colorspace cs = dcip3d65_colorspace();
    return DCIP3Colorspace_dcip3_xyz(cs, rgb);
}
