// Copyright 2022-present Contributors to the photographic-dctl project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/mikaelsundell/photographic-dctls

// Rec709 transfer curve
struct Rec709Curve {
    float threshold;
    float slope;
    float exp;
    float scale;
    float offset;
};

__DEVICE__ struct Rec709Curve
rec709_curve()
{
    struct Rec709Curve cv;
    cv.threshold = 0.018f;
    cv.slope = 4.5f;
    cv.exp = 0.45f;
    cv.scale = 1.099f;
    cv.offset = 0.099f;
    return cv;
}

__DEVICE__ float
Rec709Curve_lin_rec709(struct Rec709Curve cv, float lin)
{
    return lin < cv.threshold ? lin * cv.slope : cv.scale * pow_f(lin, cv.exp) - cv.offset;
}

__DEVICE__ float
Rec709Curve_rec709_lin(struct Rec709Curve cv, float val)
{
    return val < Rec709Curve_lin_rec709(cv, cv.threshold) ? val / cv.slope
                                                          : pow_f((val + cv.offset) / cv.scale, 1.0f / cv.exp);
}

// Rec709 colorspace
struct Rec709Colorspace {
    struct Matrix rec709_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
Rec709Colorspace_xyz_rec709(struct Rec709Colorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.rec709_matrix);
}

__DEVICE__ float3
Rec709Colorspace_rec709_xyz(struct Rec709Colorspace cs, float3 rgb)
{
    return mult_matrix(rgb, cs.xyz_matrix);
}

__DEVICE__ struct Rec709Colorspace
rec709_colorspace()
{
    struct Rec709Colorspace cs;

    // colortool --inputcolorspace Rec709 -v
    cs.rec709_matrix.m00 = 3.240970f;
    cs.rec709_matrix.m01 = -1.537383f;
    cs.rec709_matrix.m02 = -0.498611f;
    cs.rec709_matrix.m03 = -0.969244f;
    cs.rec709_matrix.m04 = 1.875968f;
    cs.rec709_matrix.m05 = 0.041555f;
    cs.rec709_matrix.m06 = 0.055630f;
    cs.rec709_matrix.m07 = -0.203977f;
    cs.rec709_matrix.m08 = 1.056972f;

    cs.xyz_matrix.m00 = 0.412391f;
    cs.xyz_matrix.m01 = 0.357584f;
    cs.xyz_matrix.m02 = 0.180481f;
    cs.xyz_matrix.m03 = 0.212639f;
    cs.xyz_matrix.m04 = 0.715169f;
    cs.xyz_matrix.m05 = 0.072192f;
    cs.xyz_matrix.m06 = 0.019331f;
    cs.xyz_matrix.m07 = 0.119195f;
    cs.xyz_matrix.m08 = 0.950532f;

    return cs;
}

__DEVICE__ float3
rec709_y_lum_coeff()
{
    struct Rec709Colorspace cs = rec709_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// Rec2020 colorspace
struct Rec2020Colorspace {
    struct Matrix rec2020_matrix;
    struct Matrix xyz_matrix;
};

__DEVICE__ float3
Rec2020Colorspace_xyz_rec2020(struct Rec2020Colorspace cs, float3 xyz)
{
    return mult_matrix(xyz, cs.rec2020_matrix);
}

__DEVICE__ float3
Rec2020Colorspace_rec2020_xyz(struct Rec2020Colorspace cs, float3 rgb)
{
    return mult_matrix(rgb, cs.xyz_matrix);
}

__DEVICE__ struct Rec2020Colorspace
rec2020_colorspace()
{
    struct Rec2020Colorspace cs;

    // colortool --inputcolorspace Rec2020 -v
    cs.rec2020_matrix.m00 = 1.716651f;
    cs.rec2020_matrix.m01 = -0.355671f;
    cs.rec2020_matrix.m02 = -0.253366f;
    cs.rec2020_matrix.m03 = -0.666684f;
    cs.rec2020_matrix.m04 = 1.616481f;
    cs.rec2020_matrix.m05 = 0.015769f;
    cs.rec2020_matrix.m06 = 0.017640f;
    cs.rec2020_matrix.m07 = -0.042771f;
    cs.rec2020_matrix.m08 = 0.942103f;

    cs.xyz_matrix.m00 = 0.636958f;
    cs.xyz_matrix.m01 = 0.144617f;
    cs.xyz_matrix.m02 = 0.168881f;
    cs.xyz_matrix.m03 = 0.262700f;
    cs.xyz_matrix.m04 = 0.677998f;
    cs.xyz_matrix.m05 = 0.059302f;
    cs.xyz_matrix.m06 = 0.000000f;
    cs.xyz_matrix.m07 = 0.028073f;
    cs.xyz_matrix.m08 = 1.060985f;

    return cs;
}

__DEVICE__ float3
rec2020_y_lum_coeff()
{
    struct Rec2020Colorspace cs = rec2020_colorspace();
    return make_float3(cs.xyz_matrix.m03, cs.xyz_matrix.m04, cs.xyz_matrix.m05);
}

// linear -> Rec709
__DEVICE__ float3
lin_rec709(float3 rgb)
{
    struct Rec709Curve cv = rec709_curve();
    return make_float3(Rec709Curve_lin_rec709(cv, rgb.x), Rec709Curve_lin_rec709(cv, rgb.y),
                       Rec709Curve_lin_rec709(cv, rgb.z));
}

// Rec709 -> linear
__DEVICE__ float3
rec709_lin(float3 rgb)
{
    struct Rec709Curve cv = rec709_curve();
    return make_float3(Rec709Curve_rec709_lin(cv, rgb.x), Rec709Curve_rec709_lin(cv, rgb.y),
                       Rec709Curve_rec709_lin(cv, rgb.z));
}

// XYZ -> Rec709
__DEVICE__ float3
xyz_rec709(float3 xyz)
{
    struct Rec709Colorspace cs = rec709_colorspace();
    return Rec709Colorspace_xyz_rec709(cs, xyz);
}

// Rec709 -> XYZ
__DEVICE__ float3
rec709_xyz(float3 rgb)
{
    struct Rec709Colorspace cs = rec709_colorspace();
    return Rec709Colorspace_rec709_xyz(cs, rgb);
}

// XYZ -> Rec2020
__DEVICE__ float3
xyz_rec2020(float3 xyz)
{
    struct Rec2020Colorspace cs = rec2020_colorspace();
    return Rec2020Colorspace_xyz_rec2020(cs, xyz);
}

// Rec2020 -> XYZ
__DEVICE__ float3
rec2020_xyz(float3 rgb)
{
    struct Rec2020Colorspace cs = rec2020_colorspace();
    return Rec2020Colorspace_rec2020_xyz(cs, rgb);
}

// YCbCr -> Rec709
__DEVICE__ float3
ycbcr_rgb709(float3 ycbcr)
{
    float y = ycbcr.x;
    float cb = ycbcr.y - 0.5f;
    float cr = ycbcr.z - 0.5f;
    float r = y + 1.5748f * cr;
    float g = y - 0.1873f * cb - 0.4681f * cr;
    float b = y + 1.8556f * cb;
    return make_float3(r, g, b);
}

// Rec709 -> YCbCr
__DEVICE__ float3
rgb709_ycbcr(float3 rgb)
{
    float y = 0.2126f * rgb.x + 0.7152f * rgb.y + 0.0722f * rgb.z;
    float cb = (rgb.z - y) / 1.8556f + 0.5f;
    float cr = (rgb.x - y) / 1.5748f + 0.5f;
    return make_float3(y, cb, cr);
}
