// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// sRGB transfer curve
struct sRGBCurve {
    float threshold;
    float slope;
    float exp;
    float scale;
    float offset;
};

__DEVICE__ struct sRGBCurve
sRGB_curve()
{
    struct sRGBCurve cv;
    cv.threshold = 0.0031308f;
    cv.slope = 12.92f;
    cv.exp = 2.4f;
    cv.scale = 1.055f;
    cv.offset = 0.055f;
    return cv;
}

__DEVICE__ float
sRGBCurve_lin_sRGB(struct sRGBCurve cv, float lin)
{
    return lin <= cv.threshold ? lin * cv.slope : cv.scale * pow_f(lin, 1.0f / cv.exp) - cv.offset;
}

__DEVICE__ float
sRGBCurve_sRGB_lin(struct sRGBCurve cv, float val)
{
    return val <= sRGBCurve_lin_sRGB(cv, cv.threshold) ? val / cv.slope : pow_f((val + cv.offset) / cv.scale, cv.exp);
}

// sRGB colorspace
struct sRGBColorspace {
    struct Matrix sRGB_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
sRGBColorspace_xyz_sRGB(struct sRGBColorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.sRGB_matrix);
}

__DEVICE__ float3
sRGBColorspace_sRGB_xyz(struct sRGBColorspace cs, float3 rgb)
{
    return mult_matrix(rgb, cs.xyz_matrix);
}

__DEVICE__ struct sRGBColorspace
sRGB_colorspace()
{
    struct sRGBColorspace cs;

    // colortool --inputcolorspace sRGB -v
    cs.sRGB_matrix.m00 = 3.2406f;
    cs.sRGB_matrix.m01 = -1.5372f;
    cs.sRGB_matrix.m02 = -0.4986f;
    cs.sRGB_matrix.m03 = -0.9689f;
    cs.sRGB_matrix.m04 = 1.8758f;
    cs.sRGB_matrix.m05 = 0.0415f;
    cs.sRGB_matrix.m06 = 0.0557f;
    cs.sRGB_matrix.m07 = -0.2040f;
    cs.sRGB_matrix.m08 = 1.0570f;

    cs.xyz_matrix.m00 = 0.4124f;
    cs.xyz_matrix.m01 = 0.3576f;
    cs.xyz_matrix.m02 = 0.1805f;
    cs.xyz_matrix.m03 = 0.2126f;
    cs.xyz_matrix.m04 = 0.7152f;
    cs.xyz_matrix.m05 = 0.0722f;
    cs.xyz_matrix.m06 = 0.0193f;
    cs.xyz_matrix.m07 = 0.1192f;
    cs.xyz_matrix.m08 = 0.9505f;

    return cs;
}

__DEVICE__ float3
sRGB_y_lum_coeff()
{
    struct sRGBColorspace cs = sRGB_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// linear -> sRGB
__DEVICE__ float3
lin_sRGBgamma22(float3 rgb)
{
    struct sRGBCurve cv = sRGB_curve();
    return make_float3(sRGBCurve_lin_sRGB(cv, rgb.x), sRGBCurve_lin_sRGB(cv, rgb.y), sRGBCurve_lin_sRGB(cv, rgb.z));
}

// sRGB -> linear
__DEVICE__ float3
sRGBgamma22_lin(float3 rgb)
{
    struct sRGBCurve cv = sRGB_curve();
    return make_float3(sRGBCurve_sRGB_lin(cv, rgb.x), sRGBCurve_sRGB_lin(cv, rgb.y), sRGBCurve_sRGB_lin(cv, rgb.z));
}

// XYZ -> sRGB
__DEVICE__ float3
xyz_sRGB(float3 xyz)
{
    struct sRGBColorspace cs = sRGB_colorspace();
    return sRGBColorspace_xyz_sRGB(cs, xyz);
}

// sRGB -> XYZ
__DEVICE__ float3
sRGB_xyz(float3 rgb)
{
    struct sRGBColorspace cs = sRGB_colorspace();
    return sRGBColorspace_sRGB_xyz(cs, rgb);
}
