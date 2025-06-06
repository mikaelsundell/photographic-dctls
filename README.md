# Photographic DCTLs #

[![License](https://img.shields.io/badge/license-BSD%203--Clause-blue.svg?style=flat-square)](https://github.com/mikaelsundell/icloud-snapshot/blob/master/license.md)

- [Photographic DCTLs](#photographic-dctls)
  - [Introduction](#introduction)
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
    - [Utility DCTLs](#utility-dctls)
      - [PD-Grade](#pd-grade)
      - [PD-Matrix](#pd-matrix)
      - [PD-Stripify](#pd-stripify)
      - [PD-Saturation](#pd-saturation)
      - [PD-Tonecurve](#pd-tonecurve)
      - [PD-Transform](#pd-transform)
        - [Supported color spaces](#supported-color-spaces)
        - [Supported gamma encodings](#supported-gamma-encodings)
        - [Additional features](#additional-features)
  - [Reference charts](#reference-charts)
      - [ACES Macbeth 50D 5203 EXR from rawtoaces](#aces-macbeth-50d-5203-exr-from-rawtoaces)
      - [LogC3 stepchart - EXR 2K LogC3 encoding](#logc3-stepchart---exr-2k-logc3-encoding)
      - [LogC3 colorchecker - EXR 2K LogC3 encoding](#logc3-colorchecker---exr-2k-logc3-encoding)
  - [References](#references)
  - [Web Resources](#web-resources)

Introduction
---------

<img src="resources/dctl.jpg" width="100%" style="padding-bottom: 20px;" />

This set of DCTL scripts is about experimenting with the math behind color science, including LogC, ACES AP0, Cineon, and more. It's a creative, photography-inspired take, focusing on looks and print emulation techniques instead of strict technical workflows. The project is still evolving, so new ideas and techniques will be added over time. Some of the concepts are a bit experimental and might overlap with Resolve's built-in tools, and the file names or structure could change as things develop.

Change log:

| Date       | Description                             |
|------------|-----------------------------------------|
| 2025-06-06 | PD-Transform: Added support for Wide Gamut |
| 2025-06-03 | PD-Transform: Fixed an issue with incorrect INGEN5 instead of INFILM5 enum |


Installation and utils
---------

To use these tools into your DaVinci Resolve setup, begin by cloning the git repository or download the release package, which contains the DCTLs and headers.

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
./run.py install
```

__To install *.dctl and *.h into DCTL directory:__

```shell
./run.py log
```

__To make a snapshot of hte DaVinvi Resolve DCTL folder:__

```shell
./run.py snapshot
```

### Quick install for Windows users

```shell
./install.bat
````

### Compatibility

All DCTLs are tested and confirmed to work with Metal, CUDA, and OpenCL on both Mac and Windows platforms.

## DCTLs

### Cineon DCTLs

#### PD-Cineon-Exposure

Cineon exposure from photographic stops, incorporating a zone based false color to aid in achieving correct exposure levels.

- https://github.com/mikaelsundell/dctl/blob/master/PD-Cineon-Exposure.dctl

![PD-Cineon-Exposure figure](resources/PD-Cineon-Exposure.png "PD-Cineon-Exposure.dctl")

#### PD-Cineon-Invert

Cineon negative inversion with precise control over the dmin ratio based inversion process using adjustable parameters such as density, bit depth, offset, and density scale. You can specify dmin base values manually as floating-point numbers (obtained from tools like Pixel Analyzer in Nuke or similar software) or sample them using a rectangular sampler from the border or other dmin base areas.

![PD-Cineon-Invert.dctl figure](resources/PD-Cineon-Invert.png "PD-Cineon-Invert.dctl")

### LogC3 and LogC4 DCTLs

#### PD-LogC-Exposure

LogC3 exposure from photographic stops, incorporating a zone based false color to aid in achieving correct exposure levels.

- https://github.com/mikaelsundell/dctl/blob/master/PD-LogC3-Exposure.dctl

![PD-LogC3-Exposure.dctl figure](resources/PD-LogC3-Exposure.png "PD-LogC3-Exposure.dctl")

#### PD-LogC-Print

![PD-LogC3-Print.dctl figure](resources/PD-LogC3-Print.png "PD-LogC3-Print.dctl")

Made for print emulation, this DCTL applies DaVinci Resolve's built-in film looks to ARRI LogC3 and Cineon footage with adjustable controls for luminosity and color blending.

- https://github.com/mikaelsundell/dctl/blob/master/PD-LogC3-Print.dctl

### Utility DCTLs

#### PD-Grade

Grade adjustments, this DCTL is experimental code for lift, gamma, gain and log controls.

- https://github.com/mikaelsundell/dctl/blob/master/PD-Grade.dctl

#### PD-Matrix

Matrix adjustments, a utility for copying matrix values.

- https://github.com/mikaelsundell/dctl/blob/master/PD-Stripify.dctl

#### PD-Stripify

Matrix adjustments, this DCTL simplifies the color palette by pushing colors into a warm and cool strip.

- https://github.com/mikaelsundell/dctl/blob/master/PD-Stripify.dctl

#### PD-Saturation

HSV-based adjustments, this DCTL provides a tool for fine-tuning saturation using saturation in HSV.

- https://github.com/mikaelsundell/dctl/blob/master/PD-Saturation.dctl

#### PD-Tonecurve

Tone curve adjustments for contrast, shoulder and toe controls.

- https://github.com/mikaelsundell/dctl/blob/master/PD-Tonecurve.dctl

#### PD-Transform

Color space transformations to and from CIE XYZ linear, tone compress, ootf and white point adaptation.

##### Supported color spaces

| Color space               | Description                         |
|---------------------------|-------------------------------------|
| CIE XYZ                   | Absolute colorimetric reference     |
| AcesAP0 D60               | ACES input primaries                |
| AcesAP1 D60               | ACES working space primaries        |
| ARRI AWG3 D65             | ARRI Wide Gamut 3                   |
| ARRI AWG4 D65             | ARRI Wide Gamut 4                   |
| DaVinci Wide Gamut D65    | Resolve-native wide gamut           |
| Rec709 D65                | HDTV color space                    |
| sRGB D65                  | Standard RGB                        |
| DCI-P3 'D60 sim' D60      | DCI-P3 simulated for D60 viewing    |
| DCI-P3 Theatrical D63     | For cinema projection               |
| DCI-P3 D65                | DCI-P3 adapted to D65               |
| Blackmagic Gen5 D65       | Blackmagic Gen 5 camera space       |

##### Supported gamma encodings

| Gamma                   | Description                         |
|-------------------------|-------------------------------------|
| Linear                  | Scene-linear light                  |
| AcesCC                  | Log encoding for ACES               |
| AcesCCT                 | Log encoding, softer shadows        |
| ARRI LogC3              | ARRI’s legacy log curve             |
| ARRI LogC4              | ARRI’s updated log curve            |
| Cineon                  | Kodak's film scan log format        |
| DaVinci Intermediate    | Resolve-native log transfer         |
| sRGB / Gamma 2.2        | Common for displays                 |
| Rec709 / Gamma 2.4      | HDTV gamma                          |
| DCI-P3 / Gamma 2.6      | Theatrical gamma                    |
| Blackmagic Film         | Blackmagic Film Gen5 log            |

##### Additional features

| Feature                 | Options                              |
|-------------------------|--------------------------------------|
| **White Point Adaptation** | D60 → D65, D63 → D65, D65 → D60, D65 → D63 |
| **Tone Compression**       | Reinhard, Inverse Reinhard         |
| **Tone OOTF Mapping**      | DCI-P3 → Rec709, sRGB → Rec709, Rec709 → DCI-P3 |
| **Exposure Index (EI)**    | For LogC3: EI 160 – EI 1600        |


- https://github.com/mikaelsundell/dctl/blob/master/PD-Transform.dctl

## Reference charts

Reference charts for testing and verifying DCTLs. Additional formats are available in the Logctool project README:

- https://github.com/mikaelsundell/logctool

#### ACES Macbeth 50D 5203 EXR from rawtoaces

![logctool_LogC3_DCI_2K figure](resources/macbeth_50D_5203.png "Macbeth 50D 5203 EXR")

- Download: [macbeth_50D_5203.exr](resources/macbeth_50D_5203.exr)

#### LogC3 stepchart - EXR 2K LogC3 encoding 

![llogctool_LogC3_stepchart_DCI_2K figure](resources/logctool_LogC3_stepchart_DCI_2K.png "ARRI LogC3 10-bit DPX")

- Download: [logctool_LogC3_stepchart_DCI_2K.exr](https://mikaelsundell.s3.eu-west-1.amazonaws.com/github/logctool/logctool_LogC3_stepchart_DCI_2K.exr)

#### LogC3 colorchecker - EXR 2K LogC3 encoding 

![logctool_LogC3_colorchecker_DCI_2K figure](resources/logctool_LogC3_colorchecker_DCI_2K.png "Cineon 10-bit DPX")

- Download: [logctool_LogC3_colorchecker_DCI_2K.exr](https://mikaelsundell.s3.eu-west-1.amazonaws.com/github/logctool/logctool_LogC3_colorchecker_DCI_2K.exr)


References
---------

**Github projects**
* https://github.com/mikaelsundell/colortool
* https://github.com/mikaelsundell/logctool
  
**Wikipedia**
* https://en.wikipedia.org/wiki/Rec._709
* https://en.wikipedia.org/wiki/DCI-P3
* https://en.wikipedia.org/wiki/Grayscale

**Format specifications**
* http://www.dcimovies.com/archives/spec_v1_1/DCI_DCinema_System_Spec_v1_1.pdf
* https://docs.acescentral.com/specifications/acescc/#color-space
* https://www.arri.com/resource/blob/278790/dc29f7399c1dc9553d329e27f1409a89/2022-05-arri-logc4-specification-data.pdf
* https://dicomp.arri.de/digital/digital_systems/DIcompanion/index.html
* https://documents.blackmagicdesign.com/InformationNotes/DaVinci_Resolve_17_Wide_Gamut_Intermediate.pdf?_v=1628751610000

**DCTLs, apps and resources**
* https://github.com/thatcherfreeman/utility-dctls
* https://github.com/AcademySoftwareFoundation/OpenRV/blob/main/src/plugins/rv-packages/additional_nodes
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

* GitHub page:        https://github.com/mikaelsundell/dctl
* Issues              https://github.com/mikaelsundell/dctl/issues
