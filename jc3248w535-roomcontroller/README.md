# ESPHome Panels (Uhae)

Dieses Repository enthält ESPHome-Panel-Konfigurationen (YAMLs) für mehrere Touch-/Display-Panels, die über das ESPHome Dashboard geflasht und verwaltet werden.

## Struktur

Repo-Root:
- `panels/` (dieses Git-Repo)

Panel-Projekte liegen als Unterordner im Repo:
- `jc3248w535-roomcontroller/`
  - `jc3248w535-panel-01.yaml` (Panel-Konfiguration)
  - `secrets.yaml.example` (Stub-Template für Secrets)

Hinweis: Das ESPHome Dashboard wird auf dem Workspace `C:\projects\esphome` gestartet und nutzt dort Root-Stubs als Entry-Points.

## Zentrale Secrets-Strategie (wichtig)

Secrets werden **zentral außerhalb des Repos** gepflegt:

- **Zentrale Secrets-Datei (lokal, NICHT im Repo):**
  - `C:\projects\esphome\secrets.yaml`

Im Panel-Ordner liegt **keine echte Secrets-Datei im Git**, sondern nur ein Template/Stub:

- `jc3248w535-roomcontroller/secrets.yaml.example`

Inhalt:

```yaml
<<: !include ../../secrets.yaml
```

### Warum so?

ESPHome sucht standardmäßig `secrets.yaml` im gleichen Ordner wie die jeweilige YAML.  
Damit sowohl **Root-Stubs** als auch direkte Projekt-YAMLs funktionieren, wird pro Projektordner eine lokale `secrets.yaml` verwendet, die auf die zentrale Datei weiterleitet – aber **diese lokale secrets.yaml bleibt untracked**.

## Setup auf einem neuen Rechner

1) Stelle sicher, dass die zentrale Datei existiert:

- `C:\projects\esphome\secrets.yaml`

2) Lege im Panel-Ordner die lokale Secrets-Datei an (untracked):

- Kopiere das Template:
  - `jc3248w535-roomcontroller/secrets.yaml.example`
- nach:
  - `jc3248w535-roomcontroller/secrets.yaml`

3) Danach sollte `esphome config` ohne Secrets-Fehler laufen.

## Flash/OTA Workflow (empfohlen)

Für konsistente Builds und Uploads wird das Device über den **Root-Stub** im Workspace geflasht  
(der Stub liegt unter `C:\projects\esphome` und ist im ESPHome Dashboard sichtbar).

Beispiele:
- Root-Stub (Workspace): `C:\projects\esphome\JC3248w535_panel_01.yaml`
- Projekt-YAML (Repo): `C:\projects\esphome\panels\jc3248w535-roomcontroller\jc3248w535-panel-01.yaml`

Empfohlen:
- Install/OTA immer über den **Root-Stub** im Dashboard oder CLI starten.

CLI Beispiel:

```powershell
esphome config "C:\projects\esphome\JC3248w535_panel_01.yaml"
esphome upload "C:\projects\esphome\JC3248w535_panel_01.yaml" --device <IP>
```

## Git Hygiene / Sicherheit

- `secrets.yaml` wird absichtlich nicht committet.
- Es werden keine echten Passwörter, API-Keys oder OTA-Passwörter im Repo gespeichert.
- `secrets.yaml.example` ist ein Stub-Template ohne geheime Inhalte.

Optionaler Check (sollte keine echten Secrets im Repo finden):

```powershell
git grep -n "wifi_password|ota_password|api_key|password:" .
```

## Hinweise

- Wenn ESPHome über fehlende Secrets meckert: prüfen, ob im jeweiligen Projektordner eine `secrets.yaml` existiert,
  die auf `../../secrets.yaml` inkludiert.
- Wenn OTA Auth fehlschlägt: meist OTA-Passwort-Mismatch (Firmware erwartet altes Passwort).
  In dem Fall per USB neu flashen oder das alte OTA-Passwort rekonstruieren.




