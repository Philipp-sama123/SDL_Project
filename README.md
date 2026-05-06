# SDL3 Animation Project 🎮

A fun C++ animation project using SDL3! This project demonstrates sprite animations and graphics programming with a modern, cross-platform setup.

## What This Project Does

- ✨ **Modern C++20** code that's clean and easy to read
- 🎨 **SDL3** for creating windows and handling graphics
- 🖼️ **SDL3_image** for loading different image formats
- 🌍 **Works everywhere** - Windows, macOS, and Linux
- ⚡ **Easy building** with CMake presets
- 🎬 **Smooth animations** with a built-in sprite system

## What You'll Need

### For Everyone
- **CMake** 3.20 or newer
- **A C++20 compiler** (most modern compilers work)
- **Git** to grab the code

### Windows Users 🪟
- **Visual Studio 2022** with C++ tools
- **Ninja** (faster builds - install with `pip install ninja`)
- **Git** (comes with Visual Studio or get it separately)

### macOS Users 🍎
- **Xcode** 14.0+ from the App Store
- **Command Line Tools** (run `sudo xcode-select --install`)
- **Homebrew** (makes life easier)

Quick install with Homebrew:
```bash
brew install cmake ninja
```

### Linux Users 🐧

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake ninja-build git
```

**Fedora/CentOS:**
```bash
sudo dnf install gcc-c++ cmake ninja-build git
```

## Let's Build It! 🚀

Getting this project running is super easy thanks to CMake presets.

### The Quick Way

1. **Grab the code:**
   ```bash
   git clone --recursive https://github.com/your-username/SDL_Project.git
   cd SDL_Project
   ```

2. **Configure and build:**
   ```bash
   # Pick your platform and build type
   cmake --preset windows-debug    # Windows Debug
   cmake --preset windows-release  # Windows Release
   cmake --preset mac-debug        # macOS Debug
   cmake --preset mac-release      # macOS Release
   cmake --preset linux-debug      # Linux Debug
   cmake --preset linux-release    # Linux Release

   # Build it!
   cmake --build build/[preset-name] --config [Debug|Release]
   ```

### Platform-Specific Steps

#### Windows 🪟
```bash
# Set up for Debug
cmake --preset windows-debug

# Build it
cmake --build build/windows-debug --config Debug

# Run your game!
./build/windows-debug/SDL_Project.exe
```

#### macOS 🍎
```bash
# Set up for Debug
cmake --preset mac-debug

# Build it
cmake --build build/mac-debug --config Debug

# Run your game!
./build/mac-debug/SDL_Project
```

#### Linux 🐧
```bash
# Set up for Debug
cmake --preset linux-debug

# Build it
cmake --build build/linux-debug

# Run your game!
./build/linux-debug/SDL_Project
```

## How the Build System Works 🛠️

### The Magic Behind It
- **Ninja** builds things super fast on all platforms
- **C++20** for modern, clean code
- **Debug & Release** modes for development and final builds

### Dependencies? Handled Automatically! ✨

The project automatically downloads and builds everything you need:
- **SDL3** (preview-3.1.8): The graphics engine
- **SDL3_image** (preview-3.1.0): For loading images

No manual setup required - CMake handles it all!

### Cool CMake Features

#### Works Everywhere
- **Windows**: Uses Visual Studio with strict warnings
- **macOS/Linux**: Uses GCC/Clang with comprehensive warnings  
- **macOS**: Creates a nice `.app` bundle automatically

#### Assets Just Work 📁
Your game assets are copied automatically to the right place, so they're always available when you run the game.

#### Windows DLL Magic 🪟
On Windows, all the necessary SDL3 DLLs are copied next to your executable so everything just runs without hassle.

### Want to Tweak Things? 🎛️

You can customize the build easily:

```bash
# Build SDL as static library (no DLLs)
cmake --preset windows-debug -DSDL_SHARED=OFF

# Turn off image formats you don't need
cmake --preset windows-debug -DSDLIMAGE_AVIF=OFF -DSDLIMAGE_WEBP=OFF
```

## What's Inside? 📁

```
SDL_Project/
├── CMakeLists.txt          # The main build recipe
├── CMakePresets.json       # Platform-specific build settings
├── README.md
├── src/                   # C++ code lives here
│   ├── main.cpp          # Where program starts
│   ├── Animator.cpp      # Animation logic
│   └── Animator.h        # Animation system header
├── assets/               # Game stuff (images, sounds, etc.)
│   └── textures/        # Your image files
├── external/             # Auto-downloaded dependencies
├── cmake/               # Custom CMake helpers
└── build/              # Where the compiled code goes (auto-made)
```

## Running Your Game 🎮

Once you've built it, find your game here:
- **Windows**: `build/windows-debug/SDL_Project.exe`
- **macOS**: `build/mac-debug/SDL_Project` (or `SDL_Project.app`)
- **Linux**: `build/linux-debug/SDL_Project`

Your game will open an 800x600 window and show off some smooth animations!

## Having Trouble? 🤔

### Common Hiccups

1. **CMake too old?** Make sure you have CMake 3.20 or newer
2. **Compiler missing?** Install a C++20 compatible compiler
3. **Ninja not found?** Install Ninja with `pip install ninja`
4. **Git submodules missing?** Run `git submodule update --init --recursive`

### Windows-Specific Stuff 🪟
- **DLL errors?** Don't worry - CMake copies them automatically!
- **Visual Studio missing?** Install Visual Studio 2022 with C++ tools

### macOS-Specific Stuff 🍎
- **Xcode not found?** Get it from the App Store, then run `sudo xcode-select --install`
- **Permission problems?** Check your file permissions

### Linux-Specific Stuff 🐧
- **Missing dev packages?** Install the `-dev` packages for your distro
- **Graphics acting weird?** Make sure your graphics drivers are up to date

