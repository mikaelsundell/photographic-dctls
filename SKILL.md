---
name: photographic-dctl
description: Create, review, and edit DCTL code for the photographic-dctls project while preserving its coding conventions and cross-backend compatibility. Use for .dctl/.h changes, DCTL math reviews, OpenCL/Metal/CUDA portability fixes, tone/color transform work, UI parameter edits, formatting cleanup, and requests to return complete DCTL files. Enforce compact formatting, float literal discipline, PD-Common math wrappers, minimal comments, exact transform signatures, and minimal diffs that preserve existing code structure.
---

# Photographic DCTL

Follow these rules when creating or editing DCTL code.

## Preserve the source

- Treat the file supplied in the current request as the source of truth.
- Do not carry changes forward from older versions unless the user explicitly asks.
- Make the smallest necessary diff. Do not reformat unrelated code.
- Preserve the existing ordering, indentation, braces, blank-line style, and UI parameter order unless the requested change requires otherwise.
- Be especially careful when inserting `DEFINE_UI_PARAMS`: changing established parameter order can break stored Resolve node values.

## Formatting

- Prefer compact code and one-line expressions when they remain readable.
- Do not expand simple expressions into multi-line formatting.
- Prefer:

```cpp
float regionStrength = clamp_f(totalMask * params.region_strength, 0.0f, 1.0f);
```

- Avoid:

```cpp
float regionStrength =
    clamp_f(
        totalMask * params.region_strength,
        0.0f,
        1.0f);
```

- Keep function signatures on one line when practical.
- Every DCTL transform entry point must use this exact one-line signature:

```cpp
__DEVICE__ float3 transform(int p_Width, int p_Height, int p_X, int p_Y, float p_R, float p_G, float p_B)
```

## Function naming

- Prefix helper functions defined inside a `.dctl` file with a short file-specific prefix to avoid collisions with shared header functions.
- Use the pattern `<prefix>_*` for local helpers.
- Do not prefix the required DCTL entry point `transform`.
- Shared header functions such as those in `PD-Common.h` keep their project-level names.

Example:

```cpp
__DEVICE__ float vignette_smoothstep(float edge0, float edge1, float x)
__DEVICE__ float vignette_distance(...)
__DEVICE__ float3 vignette_apply(...)
```

Avoid generic local names such as `smoothstep`, `distance`, or `apply`.

## Comments

- Add only the most important comments needed to understand intent or non-obvious math.
- Keep comments short and local.
- Comment concepts such as working space, exposure domain, pivot behavior, interpolation intent, or unusual safety handling.
- Do not narrate obvious assignments, branches, or arithmetic.
- Prefer one concise comment over several explanatory comments.

## Float literals

- Audit floating-point math whenever editing a file.
- Use `f` suffixes for decimal and scientific float literals:

```cpp
0.0f
1.0f
0.18f
2.0f
1e-6f
```

- Keep true integer literals as integers when an API or semantic value is integer-based:

```cpp
int thickness = 4;
tonemap_scurve(n, 1, c);
```

- Use float literals in `DCTLUI_SLIDER_FLOAT` declarations.
- Do not change enum indices, integer selectors, dimensions, or pixel offsets to floats without reason.

## Math functions

- Prefer the project wrappers in `PD-Common.h` over raw DCTL intrinsics beginning with `_`.
- Examples:

```cpp
exp_f(x)
exp2_f(x)
abs_f(x)
clamp_f(x, lo, hi)
hypot_f(x, y)
log_f(x)
log2_f(x)
log10_f(x)
pow_f(x, p)
pow10_f(x)
round_f(x)
min_f(x, y)
max_f(x, y)
mix_f(x, y, a)
atan2_f(y, x)
```

- Use the corresponding `float3` wrappers when available.
- Read [references/pd-common-math.md](references/pd-common-math.md) when choosing a wrapper.
- Do not add global input clamps to generic math wrappers to hide domain errors. Handle domain restrictions locally in the transform that requires them.

## Cross-backend compatibility

- Keep DCTL code portable across Metal, CUDA, and OpenCL.
- Prefer C-style casts for DCTL/OpenCL portability:

```cpp
(float)p_Width
(int)(value * (float)p_Height)
```

- Avoid C++ functional casts in shared DCTL code:

```cpp
float(p_Width)
int(value)
```

- When fixing backend errors, change only the incompatible syntax unless there is a demonstrated math issue.

## Math review

Before returning edited DCTL code, check:

1. Float literals use `f` where appropriate.
2. Generic math uses `PD-Common.h` wrappers instead of direct `_...` intrinsics.
3. Divisions, logarithms, powers, and interpolation domains are intentional.
4. Any epsilon/clamp is local, justified, and does not silently alter valid negative values.
5. RGB/linear/log conversions happen in the intended domain.
6. UI defaults and ranges match the implemented math.
7. OpenCL-safe casts are used.
8. Local `.dctl` helper functions use a file-specific prefix and do not collide with shared header functions.

Do not change the visual behavior merely because an alternative implementation seems cleaner. If a mathematically meaningful change would alter the look, explain it before applying it unless the user explicitly requested that change.

## Output

- When the user asks for the complete script, return the complete edited file, not a patch.
- When the user asks for inline code, provide the full DCTL inline.
- When producing a file, preserve the original filename unless the user asks for another name.
- Briefly mention only substantive changes and any behavior-changing math decisions.
