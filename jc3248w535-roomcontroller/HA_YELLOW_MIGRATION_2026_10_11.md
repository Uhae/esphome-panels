# JC3248W535 Combined – Migration auf HA Yellow (As-built 2026-10-11)

**Status:** Firmware auf ESPHome **2026.9.0** migriert, erfolgreich per OTA installiert, hardwaregetestet, nach Owner-Freigabe gemergt.  
**Quellcode-SoT:** `main`, Merge-Commit [`d84362f`](https://github.com/Uhae/esphome-panels/commit/d84362ff9ee6a4c199b2eef3f49b09fa93d6f49a).  
**Gerät:** JC3248W535 (ESP32-S3), `192.168.2.171`; EVCC / Pool / Roborock.

## Betriebs- und Build-Umgebung

- **Regulärer Build/OTA:** ESPHome Device Builder auf Home Assistant Yellow, ESPHome **2026.9.0**.
- **Repository auf HA:** `/config/esphome/panels`, lokaler Branch `main`, verifiziert auf `d84362f`.
- **Einziger regulärer Dashboard-Stub:** `/config/esphome/jc-panel-01-combined.yaml`:
  ~~~yaml
  <<: !include panels/jc3248w535-roomcontroller/jc3248w535-panel-01-combined.yaml
  ~~~
- **Windows-Quellcode:** `C:\projects\esphome\panels` wurde nach Sicherung der Altversion ebenfalls auf `main`/`d84362f` aktualisiert. Dortiges ESPHome **2026.3.0** ist *nur* Recovery; nicht mit dem neuen `main` unkontrolliert bauen/flashen.
- Änderungen nur versioniert über Branch/PR. OTA, Live-Geräteaktionen und Merge nur nach jeweils gesonderter Owner-Freigabe.

## Warum die Umstellung nicht nur eine Rotation war

Der erste ESPHome-2026.9.0-Kandidat [PR #2](https://github.com/Uhae/esphome-panels/pull/2) verlagerte `rotation: 90` von `display` nach `lvgl`, behielt aber alte Touch-Transformationen; **Hardware-FAIL**: „Next Page“ betätigte versehentlich die Pool-Heizung. Keine Wiederverwendung dieses Zwischenstands für OTA/Recovery.

Erfolgreiche Lösung nach isolierter [Touch-Diagnose V2](https://github.com/Uhae/esphome-panels/blob/experiment/jc-panel-touch-diagnostic-v2-20261011/jc3248w535-roomcontroller/TOUCH_DIAGNOSTIC_20261011.md):

~~~yaml
# variants/ui_combined.yaml
lvgl:
  rotation: 90

# common/hw_display_touch.yaml -> touchscreen
transform:
  swap_xy: false
  mirror_x: false
  mirror_y: false
calibration:
  x_min: 0
  x_max: 287
  y_min: 0
  y_max: 487
~~~

Der produktive OTA-Kandidat war Source-Commit `40d8237e9e68849db681b678ff76171ea8392b6e`, ESPHome **2026.9.0**, `config_hash=0x685f2718` (Build 2026-10-11 20:34:43 +0200). Log: `OTA successful`; vollständiges Dashboard gestartet.

**Owner-Hardwaretest PASS:** Seitennavigation EVCC → Pool → Roborock ohne unbeabsichtigte Aktionen; Pool-Pumpe und -Heizung; EVCC-Wallbox und Smart Charger; Roborock-Raumaktionen. Pool-Verbraucher waren stromlos, daher **kein Lasttest**. Touch hat geringe akzeptierte Randabweichungen; kein Langzeittest.

Der anschließend dokumentierte Kandidat wurde über [PR #3](https://github.com/Uhae/esphome-panels/pull/3) nach `main` gemergt; **Firmware-Dateien entsprechen dem installierten Source-Commit** (zusätzlich nur Abnahmebericht). GitHub markierte PR #2 wegen Commit-Ancestry ebenfalls als gemergt; es gab **keinen separaten Merge von PR #2**.

Vollständige [Hardware-Abnahme](ACCEPTANCE_ESPHOME_2026_9_20261011.md). Deren damaliger Merge-Holdpoint ist historisch; Owner erteilte danach eine gesonderte Merge-Freigabe, PR #3 wurde gemergt.

## Recovery-Baseline vor Migration

Windows-ESPHome **2026.3.0**, früherer Git-Stand `9e44db353845f2be28b2f3093c6c564c1d0e6735`. Sicherung:

`C:\projects\esphome-recovery\jc-panel-2026-3-0-20261011-214212`

Inhalt: verifiziertes `panels-main-9e44db3.bundle`, Root-Stub, vier SHA-256-geprüfte `firmware-candidates`, `manifest.json`, `candidate-02.build_info.json` und `firmware-identification.json`.

**Bevorzugter Rollback-Kandidat:** `firmware-candidates\candidate-02.bin`; **1.205.792 Byte**, Build-Metadaten ESPHome **2026.3.0**, `config_hash=0xe218091f` (2026-10-11 18:13:40 +0200), SHA-256 im Archivmanifest. Die Übereinstimmung mit den tatsächlich früher installierten Binär-Bytes ist **nicht** kryptografisch bewiesen. Vor Flash Integrität prüfen, alte Secrets und `C:\esphome\venv` bereithalten, erneute ausdrückliche OTA-Freigabe einholen. Firmware-Binaries können eingebettete Zugangsdaten enthalten.

## Offen

- `qspi_dbi` ist deprecated. `mipi_spi` erst in eigenem Branch, mit separatem Hardwaretest/OTA-Freigabe; nicht mit der erfolgreichen Touch-Migration vermischen.
- Eventuell eintretende Reconnect-/Langzeitprobleme separat testen.
- Dokumentation des Gesamt-Workflows in privater [Homelab Knowledge Base](https://github.com/Uhae/homelab-knowledge-base/blob/main/esphome/ha-yellow-migration-2026-10-11.md) (Link nach Merge der Dokumentations-PR).
