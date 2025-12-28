# 🌌 Xenith — Mini-Photoshop Application
Xenith is a modern cross platform mini-photoshop application built with **OpenGL**, **GLFW**, and **Dear ImGui**, designed to run on **Linux**, **Windows**, and **macOS**.

---

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building](#building)
- [Running](#running)
- [Links](#links)

---

## Features
-  🖥️ **Cross-platform** (Linux, Windows, macOS)
-  🎨 **26+ Image filters** with C++23
-  ⚡ **Modern OpenGL** rendering
-  🎯 **Dear ImGui** for elegant user interfaces
-  🪟 **GLFW** for window and input management
-  ↩️ **Undo/Redo** for full edit history support
-  🧹 **Clear and Align** quick image manipulation tools
-  ⌨️ **Keyboard shortcuts** for ease of use
-  🔧 **Simple Make build system**
-  📦 **Clean and modular architecture**

---

##  Prerequisites
###  Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y build-essential git
sudo apt-get install -y libglfw3-dev libgl1-mesa-dev pkg-config
```
---

###  macOS
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# Install dependencies
brew update
brew install glfw pkg-config
```
---
### Windows (via MSYS2)
1. Download and install **MSYS2** from [https://www.msys2.org/](https://www.msys2.org/)
2. Open the **MSYS2 terminal**
3. Install dependencies:
```bash
pacman -Syu       # update package database
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-glfw mingw-w64-x86_64-toolchain make git
```
---
## Installation
1. **Clone the repository**
```bash
git clone https://github.com/nerdev1/Xenith.git
cd Xenith
```
2. **Verify project structure**
```
Xenith/
├── src/
│   ├── main.cpp
│   ├── filters.hpp
│   └── fonts/
│       └── Nunito-Bold.hpp
├── libs/
│   ├── imgui/
│   │   ├── imgui.cpp
│   │   ├── imgui_draw.cpp
│   │   ├── imgui_tables.cpp
│   │   ├── imgui_widgets.cpp
│   │   ├── imgui.h
│   │   ├── imgui_internal.h
│   │   ├── imstb_truetype.h
│   │   ├── imstb_textedit.h
│   │   ├── imstb_rectpack.h
│   │   ├── imconfig.h
│   │   └── backends/
│   │       ├── imgui_impl_glfw.cpp
│   │       ├── imgui_impl_glfw.h
│   │       ├── imgui_impl_opengl3.cpp
│   │       ├── imgui_impl_opengl3_loader.h
│   │       └── imgui_impl_opengl3.h
│   ├── stb/
│   │   ├── Image_Class.h
│   │   ├── stb_image.h
│   │   └── stb_image_write.h
│   └── portable_file_dialogs/
│       └── portable-file-dialogs.h
│
├── misc/
│   ├── fonts/
│   │   └── Nunito-Bold.ttf
│   └── icon/
│       └── icon.png
├── Makefile
├── .gitignore
└── README.md
```
---
## Building
### Automated dependency installation
```bash
make install-deps
```
### Build commands
```bash
make          # Standard build (auto-detects platform)
make release  # Optimized build (-O3, LTO, no debug)
make debug    # Debug build (-g, no optimizations)
make clean    # Remove build artifacts
```
---
### Platform notes
#### Linux
- Uses system OpenGL and GLFW 
- Link Time Optimization (LTO) enabled 
- Native architecture optimizations 
#### macOS
- Uses OpenGL frameworks (Cocoa, IOKit, CoreVideo)
- Homebrew-provided GLFW
- Native architecture optimizations
#### Windows
- Compiles with **MinGW-w64**
- Static linking (no external DLLs)
- Uses Windows OpenGL (`opengl32`)
---
## Running
After a successful build: **Linux/macOS**
```bash
./Xenith
```
**Windows**
```bash
.\Xenith.exe
```
---
## Links
- **Repository:** https://github.com/nerdev1/Xenith  
- **Issues:** Report bugs or request features on GitHub
---
**Made with ❤️ using C++23, OpenGL, and Dear ImGui**
