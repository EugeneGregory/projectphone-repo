# ProjectPhone v0.1

**ProjectPhone** is an independent, lightweight, and interactive mobile device manager (an iTunes / iMazing analog) built for Ubuntu Linux. 

Developed by lead engineer **Eugene254 (Genie)** and his AI co-pilot.

---

## Description

The main goal of the application is to eliminate terminal routine for the user and provide a convenient graphical interface for file management, screen mirroring, and backing up smartphones based on iOS (Apple) and Android.

### Key Features:
* **Ultra-lightweight Stack:** Built using pure **C**, **GTK4** for the GUI, **CSS** for custom styling, and **SQLite** for the database. Optimized to run smoothly on legacy hardware with just **4GB of RAM** (like a MacBook Mid 2012). No bloated Electron or Node.js allowed.
* **Smart Terminal Remote:** The application detects connected devices via USB Vendor ID and automatically scans Ubuntu for required system utilities. Instead of inflating its own size, the program acts as a guide — it triggers `gnome-terminal` to install or purge official packages (`libimobiledevice`, `ifuse`, `uxplay`, `adb`, `scrcpy`, `go-mtpfs`) on the fly, transforming red crosses into green checkmarks.
* **Safe System Operations:** Deletions are strictly handled via safe `purge` commands, completely banning `autoremove` scripts to guarantee the ultimate safety of the Linux kernel and desktop environment.

---

## Project Structure

* `src/` — Contains all C source code files (`main.c`, `ui.c`, etc.).
* `Makefile` — Automated compilation script.
* `.gitignore` — Rules to keep the repository clean from system garbage.

---

## Compilation and Running

To compile and launch the project automatically, run the following commands in your Ubuntu terminal:

```bash
make
./projectphone
```
