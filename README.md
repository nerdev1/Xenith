# 🌌 Xenith

A modern cross-platform mini-photoshop application built with **OpenGL**, **GLFW**, and **Dear ImGui**, designed to run seamlessly on **Linux**, **Windows**, and **macOS**.

---

## Table of Contents

- Features
- Prerequisites
- Installation
- Building
- Running
- Contributing
- License
- Links

---

## Features

- 🖥️ **Cross-platform** — Full support for Linux, Windows, and macOS  
- 🎨 **26+ Image Filters** — Extensive filter library powered by modern C++23  
- ⚡ **Modern OpenGL** — Hardware-accelerated rendering  
- 🎯 **Dear ImGui** — Clean, intuitive user interface  
- 🪟 **GLFW** — Robust window and input management  
- ↩️ **Undo/Redo** — Full edit history support  
- 🧹 **Clear and Align** — Quick image manipulation tools  
- ⌨️ **Keyboard Shortcuts** — Efficient workflow controls  
- 🔧 **Simple Build System** — Straightforward Make-based compilation  
- 📦 **Modular Architecture** — Clean, maintainable codebase

---

## Prerequisites

### Linux (Debian/Ubuntu)
```bash
sudo apt-get update
sudo apt-get install -y build-essential git pkg-config
sudo apt-get install -y libglfw3-dev libgl1-mesa-dev
```

### macOS
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew update
brew install glfw pkg-config

### Windows (MSYS2)
1. Download and install **MSYS2** from https://www.msys2.org/  
2. Open the **MSYS2 MINGW64** terminal  
3. Install dependencies:
```bash
pacman -Syu  # Update package database
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-glfw mingw-w64-x86_64-toolchain make git
```

---

## Installation

1. Clone the repository:
```bash
git clone https://github.com/nerdev1/Xenith.git
cd Xenith
```

2. Verify project structure
```
Xenith/
├── src/
│   ├── main.cpp                    # Application entry point
│   ├── filters.hpp                 # Image filter implementations
│   └── fonts/
│       └── Nunito-Bold.hpp         # Embedded font data
│
├── libs/
│   ├── imgui/                      # Dear ImGui library
│   │   ├── imgui.cpp
│   │   ├── imgui_draw.cpp
│   │   ├── imgui_tables.cpp
│   │   ├── imgui_widgets.cpp
│   │   ├── imgui.h
│   │   ├── imgui_internal.h
│   │   ├── imstb_*.h               # STB headers
│   │   ├── imconfig.h
│   │   └── backends/               # Platform backends
│   │       ├── imgui_impl_glfw.*
│   │       └── imgui_impl_opengl3.*
│   │
│   ├── stb/                        # STB image library
│   │   ├── Image_Class.h
│   │   ├── stb_image.h
│   │   └── stb_image_write.h
│   │   
│   └── portable_file_dialogs/      # File dialog library
│       └── portable-file-dialogs.h
│
├── misc/
│   ├── fonts/
│   │   └── Nunito-Bold.ttf
│   └── icon/
│       └── icon.png
│
├── Makefile                        # Build configuration
├── .gitignore
└── README.md
```

3. (Optional) Install platform dependencies:
```bash
make install-deps
```

---

## Building

### Build Commands
```bash
make          # Standard build (auto-detects platform)
make release  # Optimized build with -O3, LTO, no debug symbols
make debug    # Debug build with -g, no optimizations
make clean    # Remove all build artifacts
```

### Platform-Specific Build Details

#### Linux
- Uses system OpenGL libraries and GLFW
- Link Time Optimization (LTO) enabled for better performance
- Native architecture optimizations

#### macOS
- Links against OpenGL frameworks (Cocoa, IOKit, CoreVideo)
- Uses Homebrew-provided GLFW
- Optimized for native Apple Silicon and Intel architectures

#### Windows
- Compiles with **MinGW-w64** toolchain
- Static linking (no external DLL dependencies)
- Uses Windows OpenGL (`opengl32`)

---

## Running

After successful compilation:

**Linux/macOS:**
```bash
./Xenith
```

**Windows:**
```bash
.
/Xenith.exe
```

---

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

---

## License

This project is open source. Please check the repository for license details.

## Links

- **Repository:** https://github.com/nerdev1/Xenith  
- **Issues:** Report bugs or request features on GitHub

---

**Made with ❤️ using C++23, OpenGL, and Dear ImGui**