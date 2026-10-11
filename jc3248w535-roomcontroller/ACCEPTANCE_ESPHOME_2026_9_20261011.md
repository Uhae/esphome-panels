# Hardware acceptance: JC3248W535 combined panel on ESPHome 2026.9.0

**Test date:** 2026-10-11 (CEST)
**Hardware:** JC3248W535, ESP32-S3, at 192.168.2.171
**Candidate branch:** `feature/esphome-2026-9-touch-v2-production-candidate`
**Firmware source commit installed:** `40d8237e9e68849db681b678ff76171ea8392b6e`
**Compiled ESPHome version:** 2026.9.0
**Build fingerprint (from compile log):** `config_hash=0x685f2718`; compiled `2026-10-11 20:34:43 +0200`.
**Validation:** Owner confirmed successful.
**Compilation:** Owner-provided log includes `INFO Successfully compiled program.`
**OTA:** Owner separately authorized the one-time production candidate OTA. Owner-provided log includes `INFO OTA successful` and `INFO Successfully uploaded program.` Full combined dashboard visible afterwards.

## Hardware test matrix

All PASS results below are *owner-reported manual physical hardware tests*; they are not automated test evidence.

| Test | Status | Evidence / qualification |
|---|---|---|
| Isolated nine-point touch grid, diagnostic V2 | PASS | Physical nine-point taps and repeats (see `TOUCH_DIAGNOSTIC_20261011.md` on experiment diagnostic branch); approx. 3–17 px typical initial error except mis-hit edge points; repeat of point 8 ~4–9 px, point 9 ~17–24 px; acceptable to owner. |
| Navigation: EVCC -> Pool -> Roborock | PASS | Owner confirmed expected page cycling on flashed full dashboard. |
| Navigation without unintended actions | PASS | Owner confirmed no unintended button actions during page cycling. |
| Pool circulation/Pumpe | PASS | Owner confirmed pool functions. |
| Pool heater/Heizung | PASS | Owner confirmed pool functions. |
| EVCC Wallbox | PASS | Owner confirmed EVCC functions. |
| EVCC Smart Charger | PASS | Owner confirmed EVCC functions. |
| Roborock room buttons | PASS | Owner confirmed Roborock functions. |

**Scope qualification:** The owner stated the pool components were disconnected from mains during the tests. Thus the panel-to-HA control mapping was exercised, but motor/heater load operation was not tested. Do not represent this as a live electrical pool commissioning test.

**Not specifically evidenced:** exact final HA switch states, original EVCC mode restoration, detailed individual Roborock action logs, long-term stability or reconnection resilience. Do not infer these.

## Root-cause and source change

The previous ESPHome 2026.9.0 candidate (PR #2) moved display rotation to LVGL but left the old touch swap/mirror and calibration intact. That candidate was a hardware **FAIL**: touching the visible Next Page button unexpectedly operated the pool heater.

ESPHome 2026.9.0 touchscreen component swaps `x_raw` and `y_raw` before logging them when `swap_xy: true`, and ESPHome LVGL rotates input points when `lvgl.rotation: 90`. Isolated diagnostic V2 established that the corrected mapping works on physical panel.

Production candidate modifies only `common/hw_display_touch.yaml` relative to PR #2; combined UI retains `lvgl.rotation: 90`:

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

Against GitHub `main`, the candidate has 2 functional file changes: `common/hw_display_touch.yaml` and `variants/ui_combined.yaml`. The rest of combined HA, pool, EVCC, Roborock definitions remain unchanged.

## Residual items / follow-ups

- `qspi_dbi` component is deprecated; migrate to `mipi_spi` later **in a separate change**. Not a blocker for this accepted ESPHome 2026.9.0 firmware.
- Minor edge touch offsets remain (especially lower right); acceptable to owner during this test, but do not claim pixel-perfect calibration.
- Ongoing operation / reconnection behavior was not endurance-tested.
- Previous working Windows ESPHome 2026.3.0 source remains rollback baseline.

## Governance

**HARDWARE ACCEPTANCE PASS for tested scope.**
**No merge authorization has yet been granted.**
No additional OTA authorization is granted by this report.
GitHub `main` and existing Draft PR #2 must remain unchanged until separate owner direction. PR #2's previous HARDWARE_TEST_FAIL remains historically true for the old code at its own head SHA.
