# ESPHome Panels (Uhae)

Dieses Repository enthält ESPHome-Panel-Konfigurationen (YAMLs) für mehrere Touch-/Display-Panels, die über das ESPHome Dashboard geflasht und verwaltet werden.

## Struktur

Repo-Root:
- `panels/` (dieses Git-Repo)

Panel-Projekte liegen als Unterordner im Repo:
- `jc3248w535-roomcontroller/`
  - `common/` (gemeinsame Building Blocks: Netzwerk, Hardware, HA Sources, Logic, LVGL Switches)
  - `variants/` (UI-Varianten)
    - `ui_stable.yaml`
    - `ui_alt.yaml`
  - `jc3248w535-panel-01-stable.yaml` (Stable Entry YAML)
  - `jc3248w535-panel-01-alt.yaml` (Alt Entry YAML)
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

Warum so?

ESPHome sucht standardmäßig secrets.yaml im gleichen Ordner wie die jeweilige YAML.
Damit sowohl Root-Stubs als auch direkte Projekt-YAMLs funktionieren, wird pro Projektordner eine lokale secrets.yaml verwendet, die auf die zentrale Datei weiterleitet – aber diese lokale secrets.yaml bleibt untracked.

Setup auf einem neuen Rechner

Stelle sicher, dass die zentrale Datei existiert:

C:\projects\esphome\secrets.yaml

Lege im Panel-Ordner die lokale Secrets-Datei an (untracked):

Kopiere das Template:

jc3248w535-roomcontroller/secrets.yaml.example

nach:

jc3248w535-roomcontroller/secrets.yaml

Danach sollte esphome config ohne Secrets-Fehler laufen.

Flash/OTA Workflow (empfohlen)

Dieses Projekt unterstützt zwei flashbare Varianten (Stable / Alt), die du gegeneinander testen kannst.
Die Varianten werden über Root-Stub YAMLs im Workspace gestartet (diese erscheinen im ESPHome Dashboard als eigene Geräte).

Varianten im Repo

Stable: panels/jc3248w535-roomcontroller/jc3248w535-panel-01-stable.yaml

Alt: panels/jc3248w535-roomcontroller/jc3248w535-panel-01-alt.yaml

UI-spezifische Definitionen liegen unter:

panels/jc3248w535-roomcontroller/variants/ui_stable.yaml

panels/jc3248w535-roomcontroller/variants/ui_alt.yaml

Root-Stub YAMLs (Workspace / ESPHome Dashboard)

Diese Dateien liegen unter C:\projects\esphome und erscheinen im ESPHome Dashboard als separate Geräte:

C:\projects\esphome\jc-panel-01-stable.yaml

C:\projects\esphome\jc-panel-01-alt.yaml

Stub-Inhalt (Beispiel Stable):

<<: !include panels/jc3248w535-roomcontroller/jc3248w535-panel-01-stable.yaml

OTA Flash per IP (Windows-friendly)

mDNS (*.local) kann unter Windows unzuverlässig sein. Für robuste OTA-Updates nutze feste IP:

Stable:

esphome upload "C:\projects\esphome\jc-panel-01-stable.yaml" --device 192.168.2.171


Alt:

esphome upload "C:\projects\esphome\jc-panel-01-alt.yaml" --device 192.168.2.171

Git Hygiene / Sicherheit

secrets.yaml wird absichtlich nicht committet.

Es werden keine echten Passwörter, API-Keys oder OTA-Passwörter im Repo gespeichert.

secrets.yaml.example ist ein Stub-Template ohne geheime Inhalte.

Optionaler Check (sollte keine echten Secrets im Repo finden):

git grep -n "wifi_password|ota_password|api_key|password:" .

Hinweise

Wenn ESPHome über fehlende Secrets meckert: prüfen, ob im jeweiligen Projektordner eine secrets.yaml existiert,
die auf ../../secrets.yaml inkludiert.

Wenn OTA Auth fehlschlägt: meist OTA-Passwort-Mismatch (Firmware erwartet altes Passwort).
In dem Fall per USB neu flashen oder das alte OTA-Passwort rekonstruiere