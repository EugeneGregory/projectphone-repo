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

## [0.3.0-alpha] - 2026-06-09

### Added
* **Data-Driven Dependency Architecture:** Completely eliminated copy-paste duplication by replacing 3 loose window files with a unified static registry array (`SystemDependency`).
* **Strict Terminology Shift:** Renamed the ambiguous "Plugins" subsystem to "Dependencies Management" to accurately reflect system-level `apt` package interactions.
* **Granular Directory Structure:** Decoupled the monolithic layout into a professional file tree inside `src/`, separating core backend logic (`src/core/`) from independent GTK4 graphic modules (`src/ui/`).
* **Full English UI Localization:** Removed all remaining Cyrillic strings from the interface layout, making the desktop environment fully compliant with international open-source repository standards.

### Fixed
* **Safe Bulk Memory Buffers:** Replaced the unsafe raw string literal allocation with a proper zero-initialized character array (`char pkg_list = {0}`) to prevent random memory corruption or segmentation faults during `strcat` operations.
* **Compiler Warning Silence:** Expanded the execution command string buffer to `8192` bytes inside bulk callback handlers, completely eliminating `gcc` compile-time `-Wformat-truncation` diagnostic alerts.

## [0.4.0-alpha] - 2026-06-09

### Added
* **Dynamic Device Dashboard:** Replaced the empty right-side static placeholder label with a live hardware telemetry information hub.
* **Multi-Page Tab Interface:** Integrated a `GtkNotebook` container creating four independent workspaces: *About*, *Photos & Videos*, *Applications*, and *Files*.
* **Deep Memory Categorization:** Added a hierarchical breakdown tree beneath the main storage progress bar, listing dedicated sizes for *System Memory*, *Applications Size*, and *Media and Photos*.
* **Hardware Serial Masking:** Implemented a security masking pipeline for sensitive serial keys (UDID/ADB Serial) utilizing bullet points (`•`) by default.
* **Interactive Privacy Toggle:** Appended an interactive `👁️ Show` / `🔒 Hide` button adjacent to the serial field to safely toggle raw hardware keys visibility.
* **Targeted Pipeline Execution:** Optimised the background 1Hz USB polling tracking loop (`ui_usb_timer.c`) to trigger intensive subprocess data extraction strictly on physical connection events, saving CPU cycles.

### Fixed
* **Monolithic Dashboard Decoupling:** Split the expanding dashboard interface into granular, atomic modules: `ui_dashboard_tabs.c` (tab layouts) and `ui_dashboard_update.c` (data presentation logic).
* **Cyrillic Technical Debt:** Completely purged hidden Cyrillic notes inside the global `SYSTEM_DEPS` registry array allocation, achieving international code compliance.
* **GTK Widget Re-parenting Errors:** Fixed a critical UI layout crash by removing a double-rendering bug inside the custom tab generator utility.
* **Compiler Warning Elimination:** Fixed all hidden `-Wsign-compare` and `-Wunused-parameter` diagnostic alerts by switching loop iterators to unified `size_t` and introducing explicit generic argument suppression.

## [0.4.0.5-alpha] - 2026-06-11

### Added
* **Asynchronous GLib Threading Pipeline:** Completely decoupled the blocking filesystem recursive scanner and `ifuse` storage mounting pipeline from the main GTK4 layout context via `g_thread_new`, completely eliminating desktop freezing and "Force Quit" exceptions.
* **Thread-Safe UI Dispatcher:** Integrated a non-blocking main loop event synchronizer utilizing `g_idle_add` to safely push cross-thread memory buffers into graphic widgets only after background worker execution terminates.
* **Dynamic Media Tile Grid Layout:** Replaced the empty workspace with an adaptive, scrollable plitka system using `GtkFlowBox` wrapped inside a `GtkScrolledWindow`, strictly enforcing a clean horizontal boundary of exactly 3 folder buttons per line.
* **Interactive Path Tooltips:** Appended native `GtkBox` tooltip attributes (`gtk_widget_set_tooltip_text`) to folder buttons, moving heavy, multi-layered Apple physical directory paths inside floating preview overlays to maintain UI design cleanliness.
* **Two-Way Virtual File Browser:** Implemented a granular folder selection handler that hides the parent grid container and inflates a flat file row list context (`📄 IMG_xxxx.PNG`) capped at 150 items to prevent rendering lag, including a functional `GtkButton` callback to route back to root workspace.
* **Background Bulk Downloader Tool:** Integrated an asynchronous execution bridge connected to a large action button (`📥 Download All Files`) that dispatches a non-blocking `cp -r` system subprocess, copying live device media streams directly into local machine target trees without a single layout frame freeze.

### Fixed
* **Stack Smashing Memory Defect:** Replaced the highly unstable single character definitions (`char mount_cmd`) with rigid zero-initialized character arrays (`char mount_cmd` / `char full_mount_path`), permanently curing data corruption, thread segmentation faults, and stack smash warnings during execution formatting.
* **Apple Virtual Storage Bypass:** Completely purged the restrictive `stat()` checks and POSIX `d_type` constraints that natively reported empty folder trees on FUSE/ifuse points, switching the inner tracking mechanism to a robust recursive `opendir` / `readdir` extension parsing lookup loop.
* **Anti-Recursion iOS Guard Barrier:** Engineered an strict hardware token filter loop inside directory scanning loops to detect and automatically drop heavy circular symlink pointers (`PhotoData`, `CPLAssets`, `Thumbnails`), breaking infinite loops inside clean device environments.
* **Cross-Repository Path Subversion:** Completely audited all inner `cat` pipeline injection routines, permanently redirecting broken `projectphone-repo` directory arguments back to the active execution sandbox workspace (`projectphone`).
