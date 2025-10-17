# Detect operating system
UNAME_S := $(shell uname -s)
ifeq ($(OS), Windows_NT)
    PLATFORM = Windows
else
    ifeq ($(UNAME_S), Linux)
        PLATFORM = Linux
    endif
    ifeq ($(UNAME_S), Darwin)
        PLATFORM = MacOS
    endif
endif

# Target executable with platform-specific extension
EXE = Xenith
ifeq ($(PLATFORM), Windows)
    EXE := $(EXE).exe
endif

# Source files (same for all platforms)
SOURCES = src/main.cpp
SOURCES += libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp
SOURCES += libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp

# Object files
OBJS = $(addprefix build/, $(SOURCES:.cpp=.o))

# Common compiler flags for all platforms
CXXFLAGS = -std=c++23 -I. -Ilibs/imgui -Ilibs/imgui/backends -Istb
CXXFLAGS += -O2 -Wall -Wextra -Wno-missing-field-initializers

# Platform-specific settings
ifeq ($(PLATFORM), Linux)
    CXXFLAGS += -I/usr/include -march=native
    CXXFLAGS += $(shell pkg-config --cflags glfw3 2>/dev/null || echo "")
    LIBS = -lGL
    LIBS += $(shell pkg-config --static --libs glfw3 2>/dev/null || echo "-lglfw")
    LDFLAGS = -flto=auto
endif

ifeq ($(PLATFORM), MacOS)
    CXXFLAGS += -I/usr/local/include
    CXXFLAGS += -march=native
    # macOS uses different frameworks
    LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    LIBS += $(shell pkg-config --static --libs glfw3 2>/dev/null || echo "-lglfw")
    CXXFLAGS += $(shell pkg-config --cflags glfw3 2>/dev/null || echo "-I/usr/local/include")
    LDFLAGS = -flto=auto
endif

ifeq ($(PLATFORM), Windows)
    CXXFLAGS += -I/mingw64/include
    # Windows doesn't reliably support -march=native
    CXXFLAGS := $(filter-out -march=native,$(CXXFLAGS))
    CXXFLAGS += -O2
    LIBS = -lopengl32 -lglfw3 -lgdi32 -lole32 -luuid -lshell32
    # Static linking for Windows to avoid DLL dependencies
    LDFLAGS = -static -static-libgcc -static-libstdc++
endif

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: all

# Debug build
debug: CXXFLAGS += -g
debug: LDFLAGS :=
debug: all

# Build rules
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Main target
all: $(EXE)
	@echo "Build complete for $(PLATFORM)"

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -rf $(EXE) build/

# Installation helper targets
install-deps:
ifeq ($(PLATFORM), Linux)
	@echo "Installing dependencies for Linux..."
	sudo apt-get update && sudo apt-get install -y build-essential libglfw3-dev libgl1-mesa-dev pkg-config
endif
ifeq ($(PLATFORM), MacOS)
	@echo "Installing dependencies for macOS..."
	@if ! command -v brew >/dev/null 2>&1; then \
		echo "Homebrew not found. Please install from https://brew.sh"; \
		exit 1; \
	fi
	brew update && brew install glfw pkg-config
endif
ifeq ($(PLATFORM), Windows)
	@echo "For Windows, please install:"
	@echo "1. MSYS2: https://www.msys2.org/"
	@echo "2. Then run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-glfw mingw-w64-x86_64-toolchain"
endif

.PHONY: all clean release debug install-deps