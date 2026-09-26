# PD-Common math reference

Use these project helpers instead of calling raw `_...` DCTL intrinsics directly.

## Scalar and vector helpers

| Purpose | Scalar | float3 |
|---|---|---|
| exponential | `exp_f` | `exp_f3` |
| base-2 exponential | `exp2_f` | — |
| modulo | `mod_f` | — |
| absolute value | `abs_f` | `abs_f3` |
| clamp | `clamp_f` | `clamp_f3` |
| hypotenuse | `hypot_f` | — |
| natural log | `log_f` | `log_f3` |
| log2 | `log2_f` | `log2_f3` |
| log10 | `log10_f` | `log10_f3` |
| 10^x | `pow10_f` | `pow10_f3` |
| power | `pow_f` | `pow_f3` |
| round | `round_f` | — |
| min | `min_f` | `min_f3` |
| max | `max_f` | `max_f3` |
| linear interpolation | `mix_f` | `mix_f3` |
| atan2 | `atan2_f` | — |

`div_3f` also exists, but note that it protects denominators with a positive epsilon. Use it only when that positive-domain behavior is intended.

## Important behavior

The generic log/power wrappers are thin wrappers around the underlying DCTL math functions. Do not globally clamp their inputs. Negative or out-of-domain handling belongs in the specific color transform when required.

The project header uses float-suffixed constants such as `10.0f`, `1.0f`, and `1e-7f`; follow that convention in DCTL code.
