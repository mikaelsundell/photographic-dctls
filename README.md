# Photographic DCTLs #

[![License](https://img.shields.io/badge/license-BSD%203--Clause-blue.svg?style=flat-square)](https://github.com/mikaelsundell/photographic-dctls/blob/master/license.md)

- [Photographic DCTLs](#photographic-dctls)
  - [Introduction](#introduction)
    - [Change log:](#change-log)
  - [Installation and utils](#installation-and-utils)
    - [The run.py script](#the-runpy-script)
    - [Quick install for Windows users](#quick-install-for-windows-users)
    - [Compatibility](#compatibility)
  - [DCTLs](#dctls)
    - [Cineon DCTLs](#cineon-dctls)
      - [PD-Cineon-Exposure](#pd-cineon-exposure)
      - [PD-Cineon-Invert](#pd-cineon-invert)
    - [LogC3 and LogC4 DCTLs](#logc3-and-logc4-dctls)
      - [PD-LogC-Exposure](#pd-logc-exposure)
      - [PD-LogC-Print](#pd-logc-print)
      - [PD-LogC3-FilmMatrix](#pd-logc3-filmmatrix)
      - [PD-LogC-PrinterLights](#pd-logc-printerlights)
    - [Utility DCTLs](#utility-dctls)
      - [PD-Colorcube](#pd-colorcube)
      - [PD-Colorwarp](#pd-colorwarp)
      - [PD-GamutCompress](#pd-gamutcompress)
      - [PD-Grade](#pd-grade)
      - [PD-Matrix](#pd-matrix)
      - [PD-Saturation](#pd-saturation)
      - [PD-Stripify](#pd-stripify)
      - [PD-Tonecurve](#pd-tonecurve)
      - [PD-Vignette](#pd-vignette)
      - [PD-Transform](#pd-transform)
        - [Supported color spaces](#supported-color-spaces)
        - [Supported gamma encodings](#supported-gamma-encodings)
        - [Additional features](#additional-features)
      - [PD-VideoRange](#pd-videorange)
  - [Reference charts](#reference-charts)
      - [ACES Macbeth 50D 5203 EXR from rawtoaces](#aces-macbeth-50d-5203-exr-from-rawtoaces)
      - [LogC3 stepchart - EXR 2K LogC3 encoding](#logc3-stepchart---exr-2k-logc3-encoding)
      - [LogC3 colorchecker - EXR 2K LogC3 encoding](#logc3-colorchecker---exr-2k-logc3-encoding)
  - [Projects](#projects)
  - [References](#references)
  - [Web Resources](#web-resources)
  - [License](#license)
    - [Code](#code)
    - [References \& Specifications](#references--specifications)
    - [Third-Party Resources](#third-party-resources)
    - [Disclaimer](#disclaimer)

Introduction
---------

<img src="resources/dctl.jpg" width="100%" style="padding-bottom: 20px;" />

This set of DCTL scripts is about experimenting with the math behind color science — exploring spaces like LogC, ACES AP0, Cineon, and more. These are fundamental concepts for understanding modern color pipelines such as ACES, but the approach here is intentionally creative and photography-inspired. The focus is on looks, print emulation techniques, and exploration rather than strictly technical workflows.

The project is continuously evolving, with new ideas and techniques being added over time. Some parts overlap with Resolve’s built-in tools, and file names, interfaces, or implementations may change as things develop.

The repository also serves as a testbench for future production tools. Experimental code and work-in-progress ideas are included, so mistakes and inaccuracies may exist. While many ACES-related concepts and transforms are explored, this is not a complete implementation of the ACES framework. Components such as official RRT/ODT pipelines, ADX workflows, and other ACES infrastructure are generally outside the scope of what can be practically reproduced in DCTL. Always validate results before relying on them in production workflows.

### Change log:

| Date       | Description |
|------------|-------------|
| 2026-09-26 | Improved portability across Metal, CUDA, and OpenCL, including Intel Arc/OpenCL compatibility |
|            | Standardized single-precision floating-point literals throughout DCTLs and shared headers |
|            | Standardized use of `PD-Common.h` math wrappers for cross-backend compatibility |
|            | Added OpenCL-safe casts and removed backend-specific syntax where required |
|            | Improved math-domain handling while preserving valid negative values |
|            | Namespaced DCTL-local helper functions to avoid collisions |
|            | Improved shared color space and transfer-function headers |
|            | Improved tone curve, gamut compression, printer lights, exposure, Cineon, and vignette handling |
|            | Improved DCTL parser compatibility and explicit `float3` transform results |
| 2026-08-20 | Added generic `PD-Gamma.h` transfer function library |
|            | Added `PD-Apple.h` with Apple Log support |
|            | Refactored Rec.709 into the new `PD-Rec.h` library |
|            | Replaced OOTF display mapping with dedicated tone contrast transforms and experimental comments |
|            | Fixed D65 whitepoint adaptation for Lab output only |
|            | Fixed color space selection in `PD-GamutCompress` luma coefficients |
|            | Added consistent use of floating-point literals across the codebase |
|            | Improved use of ACES curve constants |
|            | Redesigned `PD-Tonecurve` with anchored black/white points, gamma-aware midgray, channel exposure controls, and improved toe/shoulder shaping |
|            | Added mid-gray smoothing to `PD-Tonecurve` |
|            | Added `PD-Vignette` with lens-based falloff |
|            | Fixed clang-format formatting |
| 2026-08-07 | Added Apple Log and Rec2020 |
| 2025-06-05 | Added logctool_colorcube.exr for validating the color cube DCTL |
|            | Added PD-Colorcube.dctl |
|            | Added PD-Colorwarp.dctl |
|            | Added PD-GamutCompress.dctl |
|            | Added PD-VideoRange.dctl |
|            | Improved PD-Saturation now uses luma axis and color spaces |
|            | Fixed OUTFILM5 in PD-Transform, previously used colorspace instead of gamma |
| 2025-08-19 | PD-Tonemap: Improved reinhard, now luma preserving |
| 2025-08-16 | Fixed typos in comments for color space matrices |
| 2025-08-05 | PD-Transform: Added support for YCbCr conversion |
| 2025-06-15 | PD-LogC3-FilmMatrix: Added legacy LogC3 film matrix |
| 2025-06-15 | PD-Transform: Fixed issue with sRGB in and out |
| 2025-06-06 | PD-Transform: Added support for DaVinci Wide Gamut |
| 2025-06-03 | PD-Transform: Fixed an issue with incorrect INGEN5 instead of INFILM5 enum |

Installation and utils
---------

To use these tools in your DaVinci Resolve setup, begin by cloning the git repository or download the release package, which contains the DCTLs and headers.

Run `python(.exe) run.py install` in your terminal to copy the `*.dctl` and `*.h` files to the DaVinci Resolve LUT DCTL folder, ensuring they're accessible within your Resolve environment. The files can also be manually copied to `/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/DCTL` on Mac and `C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\DCTL`on Windows.

For added convenience, the run.py script is provided, enabling easy access to various resources.

### The run.py script

__To open the DCTL readme documentation:__

```shell
./run.py readme
```

__For exploring DCTL examples:__

```shell
./run.py examples
```

__To access the DaVinci Resolve log file and initiate a trace:__

```shell
./run.py log
```

__To install *.dctl and *.h into DCTL directory:__

```shell
./run.py install
```

__To make a snapshot of the DaVinci Resolve DCTL folder:__

```shell
./run.py snapshot
```

### Quick install for Windows users

```shell
./install.bat
````

### Compatibility

The DCTLs are designed for cross-backend compatibility with Metal, CUDA, and OpenCL on Mac and Windows. The code uses single-precision math, shared math wrappers, and OpenCL-safe syntax where required. Backend and GPU-driver differences may still exist, so results should be validated on the target system.

## DCTLs

### Cineon DCTLs

#### PD-Cineon-Exposure

Cineon exposure from photographic stops, incorporating a zone based false color to aid in achieving correct exposure levels.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Cineon-Exposure.dctl

![PD-Cineon-Exposure figure](resources/PD-Cineon-Exposure.png "PD-Cineon-Exposure.dctl")

#### PD-Cineon-Invert

Cineon negative inversion with precise control over the dmin ratio based inversion process using adjustable parameters such as density, bit depth, offset, and density scale. You can specify dmin base values manually as floating-point numbers (obtained from tools like Pixel Analyzer in Nuke or similar software) or sample them using a rectangular sampler from the border or other dmin base areas.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Cineon-Invert.dctl

![PD-Cineon-Invert.dctl figure](resources/PD-Cineon-Invert.png "PD-Cineon-Invert.dctl")

### LogC3 and LogC4 DCTLs

#### PD-LogC-Exposure

LogC3 and LogC4 exposure from photographic stops, incorporating a zone based false color to aid in achieving correct exposure levels.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-LogC-Exposure.dctl

![PD-LogC3-Exposure.dctl figure](resources/PD-LogC3-Exposure.png "PD-LogC3-Exposure.dctl")

#### PD-LogC-Print

![PD-LogC3-Print.dctl figure](resources/PD-LogC3-Print.png "PD-LogC3-Print.dctl")

Made for print emulation, this DCTL applies DaVinci Resolve's built-in film looks to ARRI LogC3 and Cineon footage with adjustable controls for luminosity and color blending.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-LogC-Print.dctl

#### PD-LogC3-FilmMatrix

The film style matrix makes the color characteristics of the Log C image similar to negative film scanned on an ARRISCAN.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-LogC3-FilmMatrix.dctl

#### PD-LogC-PrinterLights

Printer light controls for LogC3 and LogC4, applying cyan, magenta, yellow, and master point adjustments as exposure changes in linear light.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-LogC-PrinterLights.dctl

### Utility DCTLs

#### PD-Colorcube

Visualize selected RGB color space as CIE XYZ or packed CIE Lab volume. Input is decoded to linear, converted to XYZ, optionally whitepoint-adapted, and optionally packed as Lab for display/export.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Colorcube.dctl

The included `logctool_colorcube.exr` can be used as a reference input for validating the color cube DCTL. It contains a structured RGB cube image that makes it easier to inspect color space conversion, XYZ mapping, Lab packing, gamut shape, and clipping behavior.

- Download: [logctool_colorcube.exr](resources/logctool_colorcube.exr)

#### PD-Colorwarp

Procedural RGB color cube warp using tetrahedral interpolation.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Colorwarp.dctl

#### PD-GamutCompress

Luma-axis gamut compression with selectable color space coefficients.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-GamutCompress.dctl

#### PD-Grade

Experimental grading controls for black and white point, lift, gain, multiply, offset, and gamma adjustments.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Grade.dctl

#### PD-Matrix

Matrix adjustments, a utility for copying matrix values.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Matrix.dctl

#### PD-Saturation

Luma-axis saturation test with selectable color space coefficients.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Saturation.dctl

#### PD-Stripify

Matrix adjustments, this DCTL simplifies the color palette by pushing colors into a warm and cool strip.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Stripify.dctl

#### PD-Tonecurve

Anchored tone curve with black and white point controls, gamma-aware midgray, channel exposure, contrast, adjustable toe and shoulder shaping, and mid-gray smoothing for a more natural transition between the lower and upper curve regions.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Tonecurve.dctl

#### PD-Vignette

Lens-based vignette for natural edge falloff. The effect is applied as a spatial exposure adjustment in linear light, with controls for amount, radius, softness, optical center, aspect, and rotation. Multiple lens-style variations can be used for natural, classic, anamorphic, or more mechanical falloff.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Vignette.dctl

#### PD-Transform

Color space transformations to and from CIE XYZ linear, tone compression, display contrast and whitepoint adaptation.

##### Supported color spaces

| Color space               | Description                         |
|---------------------------|-------------------------------------|
| CIE XYZ                   | Absolute colorimetric reference     |
| AcesAP0 D60               | ACES input primaries                |
| AcesAP1 D60               | ACES working space primaries        |
| ARRI AWG3 D65             | ARRI Wide Gamut 3                   |
| ARRI AWG4 D65             | ARRI Wide Gamut 4                   |
| Blackmagic Gen5 D65       | Blackmagic Gen 5 camera space       |
| DaVinci Wide Gamut D65    | Resolve-native wide gamut           |
| DCI-P3 'D60 sim' D60      | DCI-P3 simulated for D60 viewing    |
| DCI-P3 D65                | DCI-P3 adapted to D65               |
| DCI-P3 Theatrical D63     | For cinema projection               |
| Rec2020 D65               | UHD wide-gamut color space          |
| Rec709 D65                | HDTV color space                    |
| sRGB D65                  | Standard RGB                        |

##### Supported gamma encodings

| Gamma                   | Description                         |
|-------------------------|-------------------------------------|
| Linear                  | Scene-linear light                  |
| AcesCC                  | Log encoding for ACES               |
| AcesCCT                 | Log encoding, softer shadows        |
| Apple Log               | Apple camera log encoding           |
| ARRI LogC3              | ARRI legacy log curve               |
| ARRI LogC4              | ARRI LogC4 curve                    |
| Blackmagic Film         | Blackmagic Film Gen5 log            |
| Cineon                  | Kodak film scan log format          |
| DaVinci Intermediate    | Resolve-native log transfer         |
| Gamma 2.2               | Pure power-law gamma 2.2            |
| Gamma 2.4               | Pure power-law gamma 2.4            |
| Gamma 2.6               | Pure power-law gamma 2.6            |
| Rec709                  | Rec.709 piecewise transfer function |
| sRGB                    | sRGB piecewise transfer function    |

##### Additional features

| Feature                    | Options                                                     |
|----------------------------|-------------------------------------------------------------|
| **White Point Adaptation** | D60 → D65, D63 → D65, D65 → D60, D65 → D63                |
| **Tone Compression**       | Reinhard                                                    |
| **Display Contrast**       | Gamma 2.6 → Rec709, sRGB → Rec709, Rec709 → sRGB, Rec709 → Gamma 2.6 |
| **Exposure Index (EI)**    | For LogC3: EI 160 – EI 1600                                |

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-Transform.dctl

#### PD-VideoRange

Convert between legal/video range and full/data range.

- https://github.com/mikaelsundell/photographic-dctls/blob/master/PD-VideoRange.dctl

## Reference charts

Reference charts for testing and verifying DCTLs. Additional formats are available in the Logctool project README:

- https://github.com/mikaelsundell/logctool

#### ACES Macbeth 50D 5203 EXR from rawtoaces

![logctool_LogC3_DCI_2K figure](resources/macbeth_50D_5203.png "Macbeth 50D 5203 EXR")

- Download: [macbeth_50D_5203.exr](resources/macbeth_50D_5203.exr)

#### LogC3 stepchart - EXR 2K LogC3 encoding 

![llogctool_LogC3_stepchart_DCI_2K figure](resources/logctool_LogC3_stepchart_DCI_2K.png "ARRI LogC3 10-bit DPX")

- Download: [logctool_LogC3_stepchart_DCI_2K.exr](https://mikaelsundell.s3.eu-west-1.amazonaws.com/github/logctool/logctool_LogC3_stepchart_DCI_2K.exr)

More examples, stepcharts, and test charts are available in the separate **logctool** repository:

- [github.com/mikaelsundell/logctool](https://github.com/mikaelsundell/logctool)

#### LogC3 colorchecker - EXR 2K LogC3 encoding 

![logctool_LogC3_colorchecker_DCI_2K figure](resources/logctool_LogC3_colorchecker_DCI_2K.png "Cineon 10-bit DPX")

- Download: [logctool_LogC3_colorchecker_DCI_2K.exr](https://mikaelsundell.s3.eu-west-1.amazonaws.com/github/logctool/logctool_LogC3_colorchecker_DCI_2K.exr)



Projects
---------

* https://github.com/mikaelsundell/colortool
* https://github.com/mikaelsundell/logctool


References
---------

**Wikipedia and papers**
* https://en.wikipedia.org/wiki/Rec._709
* https://en.wikipedia.org/wiki/DCI-P3
* https://en.wikipedia.org/wiki/Grayscale
* https://dl.acm.org/doi/pdf/10.1145/566654.566575

**Format specifications**
* http://www.dcimovies.com/archives/spec_v1_1/DCI_DCinema_System_Spec_v1_1.pdf
* https://docs.acescentral.com/specifications/acescc/#color-space
* https://www.arri.com/resource/blob/278790/dc29f7399c1dc9553d329e27f1409a89/2022-05-arri-logc4-specification-data.pdf
* https://dicomp.arri.de/digital/digital_systems/DIcompanion/index.html
* https://documents.blackmagicdesign.com/InformationNotes/DaVinci_Resolve_17_Wide_Gamut_Intermediate.pdf?_v=1628751610000
* https://www.arri.com/en/learn-help/learn-help-camera-system/image-science/vfx-faq

**DCTLs, apps and resources**
* https://github.com/thatcherfreeman/utility-dctls
* https://github.com/AcademySoftwareFoundation/OpenRV/blob/main/src/plugins/rv-packages/additional_nodes
* https://forum.blackmagicdesign.com/viewtopic.php?t=61073&p=348482
* https://skia.googlesource.com/skia/+/70e432e72745/gm/hsl.cpp
* https://colour.readthedocs.io
* https://github.com/ampas/aces-dev
* https://mononodes.com

**Utilities**
* https://www.desmos.com/calculator
* https://www.desmos.com/calculator/3zhzwbfrxd
* https://www.russellcottrell.com/photo/matrixCalculator.htm

 Web Resources
-------------

* GitHub page:        https://github.com/mikaelsundell/photographic-dctls
* Issues              https://github.com/mikaelsundell/photographic-dctls/issues

## License

### Code

This project's source code is licensed under the BSD 3-Clause License.

### References & Specifications

The project includes implementations, experiments, and interpretations based on publicly available specifications, documentation, and technical publications from various sources, including but not limited to:

- ARRI LogC and ARRI Wide Gamut
- ACES (Academy Color Encoding System)
- Cineon
- DaVinci Wide Gamut
- Rec.709
- sRGB
- DCI-P3
- Blackmagic Design Generation 5 Color Science

All trademarks, specifications, and related intellectual property remain the property of their respective owners.

### Third-Party Resources

This project may use publicly available reference data, documentation, charts, or measurements for testing, validation, and educational purposes. Such materials remain the property of their respective copyright holders.

Examples include:

- ARRI technical documentation
- ACES documentation and specifications
- Manufacturer-published color science documentation

### Disclaimer

Users are responsible for complying with the licenses, terms, and intellectual property rights associated with any third-party specifications, documentation, datasets, or resources referenced by this project.