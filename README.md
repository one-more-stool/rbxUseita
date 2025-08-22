# rbxUseita v1.0.0 - RBX Multi-Instance Utility

![rbxUseita Logo](useita_512.png)
<img width="500" height="250" alt="screenshot" src="https://github.com/user-attachments/assets/c4e7f35d-266a-413d-b733-f6edce0bee96" />

A Windows application that allows you to run multiple RBX game instances simultaneously by managing the singleton event handles that normally prevent this.

## Features

- 🎮 **Multi-Instance Support**: Run multiple Rbx games at once
- 🔧 **Easy to Use**: Simple one-click unblocking with visual indicators
- 📦 **Portable**: Single executable with no external dependencies
- 🌍 **Multi-Language Support**: 29 languages with automatic system language detection
- ⚙️ **Smart Configuration**: Automatic settings persistence in `config.ini`
- 🚫 **Clean**: No unwanted files (no `imgui.ini` or log files)

## How It Works

Rbx normally prevents multiple game instances by creating a singleton event handle (`Rbx_singletonEvent`). This application:

1. Finds all running Rbx processes
2. Identifies which ones have the blocking singleton event
3. Allows you to close these handles with a single click
4. Enables new Rbx instances to start

## Usage

1. Launch the game
2. Run rbxUseita
3. Toggle autounblocking checkbox
4. Start new game as much as you want

## Configuration

The application automatically creates a `config.ini` file to store user preferences:

```ini
# rbxUseita Configuration File
# This file stores user preferences

# Auto-unblock setting (true/false)
autoUnblock=false

# User interface language
language=en
```

### First Run
- Language is automatically detected based on your Windows system language
- If your system language is not supported, English is used as default
- All settings are automatically saved when changed in the UI

## Building

### Prerequisites
- Windows 10/11
- Visual Studio 2019/2022 or Build Tools
- CMake 3.15+

### Build Steps
```bash
# Clone the repository
git clone <repository-url>
cd rbxUseita
# Clone Imgui
git clone https://github.com/ocornut/imgui

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build . --config Release
```

### Executable Compression with UPX

The project automatically compresses the Release executable using UPX for smaller file size. UPX is downloaded automatically during the first build.

#### Automatic UPX Integration
- UPX is automatically downloaded during CMake configuration
- Supports both 32-bit and 64-bit Windows systems
- Uses the latest stable version (5.0.2)
- Automatically detects system architecture

#### Manual UPX Usage (Optional)
```bash
# Compress executable manually
upx --best --lzma rbxUseita.exe
```

## Technical Details

- **Language**: C++17
- **UI Framework**: Dear ImGui (no `imgui.ini` file created)
- **Graphics API**: DirectX 11
- **Build System**: CMake
- **Architecture**: x32/x64
- **Localization**: 29 languages with automatic system detection
- **Configuration**: INI-based settings persistence
- **Compression**: UPX automatic compression (70% size reduction)
- **Clean Design**: No log files or temporary files created

### No Rbx Windows Detected
- Make sure Rbx is running
- Run program with admin rights

## License

This project is provided as-is for educational and personal use.
The developer is not responsible for your actions. There are no guarantees that you will not be banned for using this software, all on your risk.

## Contributing

Feel free to submit issues and enhancement requests!
