# Changelog

All notable changes to **ProjectPhone** will be documented in this file.

---

## [0.1.0-alpha] - 2026-06-04

### Added
* **Modular UI Architecture:** Created a 900x600 window layout using pure **C** and **GTK4** split into a multi-file structure (`main.c`, `ui.c`, `ui_ios.c`, `ui_android.c`).
* **Symmetrical Plugin Manager:** Implemented an interactive pop-up window split into iOS and Android blocks with a clear horizontal separator.
* **Smart System Scanning:** Integrated automated checking for missing or installed tools (`libimobiledevice`, `ifuse`, `uxplay`, `adb`, `scrcpy`, `go-mtpfs`) using the system `which` command.
* **Terminal Interface Remote:** Added instant spawning of `gnome-terminal --wait` to trigger `sudo apt install` or `sudo apt purge` on the fly based on what the user needs.
* **Smart Bulk Actions:** Built smart buttons to "Install All" or "Remove All" that dynamically scan the OS and bundle only the relevant packages into a single, compact terminal command string.
* **Automated Building:** Added a local `Makefile` to instantly build the multi-file environment via a simple `make` command.

### Fixed
* **Critical System Safety:** **Completely stripped and banned the `autoremove` script** from the codebase after it attempted to purge Linux kernel headers. All deletions are now safely restricted to isolated `purge` commands.
* **Buffer Stability:** Expanded the terminal command buffer to `4096` bytes and capped the package string array at `256` bytes to completely eliminate `gcc` compiler truncation warnings.


## [0.2.0-alpha] - 2026-06-04

### Added
* **Core System Plugins:** Added the final layout duo to the plugin manager—**SQLite3** and **LibCurl** for local database management and network operations.
* **SQLite3 Integration:** Developed `database.c` to permanently store, read, and manage connected mobile devices in `projectphone.db`.
* **Hardware USB Detector:** Implemented `usb_detector.c` to handle unstable cables and automatically track phone connections.

### Fixed
* **Sidebar Device Rendering:** Completely refactored the left panel (`ui.c`). Added full container widget tree cleanup before redrawing to fix the critical device duplication bug.
* **Memory Optimization:** Stopped memory bloat inside GTK4 loops when dynamically refreshing the connected devices list.
