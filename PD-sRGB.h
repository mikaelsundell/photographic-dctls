// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// clang-format on

// sRGB curve
struct sRGBCurve {
    float threshold;
    float slope;
    float exp;
    float scale;
    float offset;
};

__DEVICE__ struct sRGBCurve sRGB_curve() {
    struct sRGBCurve cv;
    cv.threshold = 0.0031308;
    cv.slope = 12.92;
    cv.exp = 2.4;
    cv.scale = 1.055;
    cv.offset = 0.055;
    return cv;
}

__DEVICE__ float sRGBCurve_lin_sRGB(struct sRGBCurve cv, float lin) {
    return (lin <= cv.threshold) ? (lin * cv.slope) : (cv.scale * pow_f(lin, 1.0 / cv.exp)) - cv.offset;
}

__DEVICE__ float sRGBCurve_sRGB_lin(struct sRGBCurve cv, float val) {
    return (val <= sRGBCurve_lin_sRGB(cv, cv.threshold)) ? (val / cv.slope) : pow_f((val + cv.offset) / cv.scale, cv.exp);
}

// sRGB colorspace
struct sRGBColorspace
{
    struct Matrix sRGB_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ struct sRGBColorspace sRGB_colorspace() {
    struct sRGBColorspace cs;
    // colortool --inputcolorspace sRGB -v
    // sRGB matrix
    cs.sRGB_matrix.m00 = 3.2406; cs.sRGB_matrix.m01 = -1.5372; cs.sRGB_matrix.m02 = -0.4986;
    cs.sRGB_matrix.m03 = -0.9689; cs.sRGB_matrix.m04 = 1.8758; cs.sRGB_matrix.m05 = 0.0415;
    cs.sRGB_matrix.m06 = 0.0557; cs.sRGB_matrix.m07 = -0.2040; cs.sRGB_matrix.m08 = 1.0570;
    // xyz matrix
    cs.xyz_matrix.m00 = 0.4124; cs.xyz_matrix.m01 = 0.3576; cs.xyz_matrix.m02 = 0.1805;
    cs.xyz_matrix.m03 = 0.2126; cs.xyz_matrix.m04 = 0.7152; cs.xyz_matrix.m05 = 0.0722;
    cs.xyz_matrix.m06 = 0.0193; cs.xyz_matrix.m07 = 0.1192; cs.xyz_matrix.m08 = 0.9505;
    return cs;
}

__DEVICE__ float3 sRGBColorspace_xyz_sRGB(struct sRGBColorspace cs, float3 xyz) {
    return mult_matrix(xyz, cs.sRGB_matrix);
}
__DEVICE__ float3 sRGBColorspace_sRGB_xyz(struct sRGBColorspace cs, float3 sRGB) {
    return mult_matrix(sRGB, cs.xyz_matrix);
}

// convert linear to sRGB gamma ~2.2
__DEVICE__ float3 lin_sRGBgamma22(float3 rgb) {
    struct sRGBCurve cv = sRGB_curve();
    return make_float3(sRGBCurve_lin_sRGB(cv, rgb.x), sRGBCurve_lin_sRGB(cv, rgb.y), sRGBCurve_lin_sRGB(cv, rgb.z));
}

// convert sRGB gamma ~2.2 to linear
__DEVICE__ float3 sRGBgamma22_lin(float3 rgb) {
    struct sRGBCurve cv = sRGB_curve();
    return make_float3(sRGBCurve_sRGB_lin(cv, rgb.x), sRGBCurve_sRGB_lin(cv, rgb.y), sRGBCurve_sRGB_lin(cv, rgb.z));
}    

// convert sRGB to xyz
__DEVICE__ float3 xyz_sRGB(float3 rgb) {
    struct sRGBColorspace cs = sRGB_colorspace();
    float3 sRGB = sRGBColorspace_xyz_sRGB(cs, rgb);
    return sRGB;
}

// convert xyz to sRGB
__DEVICE__ float3 sRGB_xyz(float3 rgb) {
    struct sRGBColorspace cs = sRGB_colorspace();
    return sRGBColorspace_sRGB_xyz(cs, rgb);
}    
