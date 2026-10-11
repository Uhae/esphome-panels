# JC Panel touch coordinate diagnostic — 2026-10-11

Status: **v1 physical measurement PASS (data collection only)**; **v2 validated, compiled, OTA-installed with owner authorization, and 9-point coordinate test passed with minor edge deviations**. The original firmware ESPHome 2026.3.0 was rolled back successfully earlier. The isolated v1 diagnostic firmware ESPHome 2026.9.0 was later flashed with explicit owner authorization.

## Observed data

Panel held in landscape orientation, 480 × 320.
Nine blue point centers are fixed at x={40,240,440}, y={55,160,265}.
Owner touched each point once. v1 logged transformed `touch.x`, `touch.y` as `x`/`y`, and `touch.x_raw`, `touch.y_raw` as `raw_x`/`raw_y`.

| Point | Target x | Target y | v1 driver x | v1 driver y | v1 raw x | v1 raw y | v2 predicted LVGL x | v2 predicted LVGL y |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 1 | 40 | 55 | 0 | 114 | 14 | 239 | 14 | 53 |
| 2 | 240 | 55 | 163 | 114 | 243 | 239 | 240 | 53 |
| 3 | 440 | 55 | 312 | 112 | 450 | 240 | 444 | 51 |
| 4 | 40 | 160 | 2 | 273 | 18 | 140 | 18 | 163 |
| 5 | 240 | 160 | 168 | 270 | 250 | 142 | 246 | 161 |
| 6 | 440 | 160 | 302 | 280 | 437 | 136 | 431 | 167 |
| 7 | 40 | 265 | 0 | 454 | 14 | 28 | 14 | 288 |
| 8 | 240 | 265 | 158 | 459 | 236 | 25 | 233 | 291 |
| 9 | 440 | 265 | 314 | 405 | 454 | 58 | 447 | 254 |

The four corner measurements were supplied last:
- Point 1: `TAP x=0 y=114 raw_x=14 raw_y=239`
- Point 3: `TAP x=312 y=112 raw_x=450 raw_y=240`
- Point 7: `TAP x=0 y=454 raw_x=14 raw_y=28`
- Point 9: `TAP x=314 y=405 raw_x=454 raw_y=58`

## Source-code findings (ESPHome 2026.9.0)

- `esphome/components/touchscreen/touchscreen.cpp`: `Touchscreen::add_raw_touch_position_` swaps raw x/y **before** placing them in `tp.x_raw`, `tp.y_raw` when `swap_xy` is true. Logged `raw_*` is therefore post-swap rather than necessarily native-controller raw.
- `esphome/components/lvgl/lvgl_esphome.cpp`: `LVTouchListener` passes driver coordinates through `rotate_coordinates` to LVGL. For rotation 90° and native display 320×480: LVGL x = driver y; LVGL y = 319 - driver x.
- v1's red marker used pre-LVGL driver x/y, **not LVGL input coordinates**. Its physical location therefore cannot be used as the actual LVGL touch location.

## v2 experimental change

```yaml
transform:
  swap_xy: false
  mirror_x: false
  mirror_y: false
calibration:
  x_min: 0
  x_max: 287
  y_min: 0
  y_max: 487
```

Rationale: v1 raw_x increases from left to right; v1 raw_y decreases from top to bottom. Removing `swap_xy` feeds the original raw_y to native driver X and original raw_x to native driver Y. The LVGL 90° conversion then restores left-to-right X and top-to-bottom Y.

For the v2 marker, draw in LVGL coordinates: `x = touch.y`, `y = 319 - touch.x`.

All calibrations must be nonnegative per ESPHome schema. A linear regression of physical positions from nine v1 readings gives approximately:
- Screen X = `0.925 * v1_raw_x + 22.6`, residual RMSE ~7.2 px
- Screen Y = `302.2 - 1.026 * v1_raw_y`, residual RMSE ~9.0 px

The native normalized calibration cannot reproduce both of these nonzero offsets exactly with permissible nonnegative lower bounds. Chosen x_max/y_max are a **first-pass constrained approximation**, not a proven final calibration. Predicted v2 error is up to ~27 px near left/bottom. Point 9 has a raw-y outlier compared with other bottom points. Re-measure after new test before any production dashboard update.

## V2 hardware findings (11 Oct 2026, owner-confirmed)

**PASS — diagnostic coordinate mapping on the physical display**, subject to mild edge offsets. V2 was reported validated and compiled on HA Yellow and installed via one separately owner-authorized OTA. Panel's V2 display markers match intended touches substantially better than V1. Isolated UI has no commands to HA actuators.

| Point | Target x,y | V2 LVGL x,y | Distance |
|---:|---|---|---:|
| 1 | 40,55 | 23,53 | 17 px |
| 2 | 240,55 | 248,58 | 9 px |
| 3 | 440,55 | 443,55 | 3 px |
| 4 | 40,160 | 37,166 | 7 px |
| 5 | 240,160 | 236,170 | 11 px |
| 6 | 440,160 | 454,168 | 16 px |
| 7 | 40,265 | 50,257 | 13 px |
| 8, initial | 240,265 | 251,297 | 34 px (user possibly slipped) |
| 9, initial | 440,265 | 454,250 | 21 px (user possibly slipped) |
| 8, repeat 1 | 240,265 | 246,272 | 9 px |
| 9, repeat 1 | 440,265 | 454,284 | 24 px |
| 8, repeat 2 | 240,265 | 242,268 | 4 px |
| 9, repeat 2 | 440,265 | 454,275 | 17 px |

Owner qualitative feedback: Point 8 well hit after repeats; Point 9 slightly right/down but acceptable. All distances are approximate finger-tap errors and do not isolate calibration from user positioning. The v2 configuration is **not yet validated with real LVGL click regions or the functional dashboard**.

**Recommended next gate:** Prepare the corrected production-dashboard candidate on a separate branch using the v2 transform/calibration, retain `lvgl.rotation: 90`; review for any additional touch transforms. Compile only; next OTA requires separate owner authorization, and touch-testing actuator buttons warrants explicit safeguards. PR #2 remains a Draft with HARDWARE_TEST_FAIL until a full dashboard hardware test.

## Safety / governance

- Isolated diagnostic firmware does not include HA state imports, service calls, switches, pool/EVCC/Roborock controls, or device automations.
- API and OTA/Wi-Fi are shared with the actual device only to allow an owner-authorized flash.
- Maintain `main` and original recovery firmware untouched.
- **No merge, no OTA** of v2 without distinct explicit owner permission. First validate and compile on HA Yellow.
