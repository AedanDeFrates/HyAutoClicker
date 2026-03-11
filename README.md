# HyAutoClicker

A lightweight, customizable auto-clicker application for Linux built with GTK and Glade. Features global hotkey activation, configurable click intervals, and thread-safe operation.

## Features

### Core Functionality
- **Automatic Mouse Clicking**: Simulates left or right mouse button clicks at configurable intervals
- **Global Hotkey Control**: Toggle auto-clicking on/off with F8 key (works system-wide)
- **Thread-Safe Operation**: Uses mutexes to prevent race conditions between GUI and background threads
- **Real-time Configuration**: Adjust settings while the application is running

### Configuration Options

#### Click Type Selection
- **Left Click**: Simulates left mouse button clicks (default)
- **Right Click**: Simulates right mouse button clicks

#### Click Interval Settings
Configure the delay between clicks using multiple time units:
- **Milliseconds**: 0-999 ms
- **Seconds**: 0-59 seconds
- **Minutes**: 0-59 minutes
- **Hours**: 0-23 hours

*Total interval = (hours × 3600000) + (minutes × 60000) + (seconds × 1000) + milliseconds*

#### Hotkey Configuration
- **Default Hotkey**: F8 (system-wide toggle)
- **Hotkey Change Mode**: Enter settings tab to reconfigure hotkey (feature in development)

### User Interface

#### Main Application Window
- **Fixed Size**: 400×400 pixels for consistent layout
- **Three Tabs**: Auto Clicker, Settings, Help

#### Auto Clicker Tab
- **Start/Stop Button**: Toggle listening mode
- **Status Display**: Shows current state ("Start" or "Listening...")
- **Visual Feedback**: Button label changes based on active state

#### Settings Tab
- **Click Type Radio Buttons**: Choose between left and right clicks
- **Interval Spin Buttons**: Configure click timing
- **Hotkey Change Toggle**: Enable hotkey reconfiguration mode

#### Help Tab
- **Application Information**: Usage instructions and feature descriptions

### Technical Implementation

#### Architecture
- **GTK GUI Framework**: Cross-platform widget toolkit
- **Glade UI Designer**: XML-based interface definition
- **Multi-threaded Design**: Separate threads for GUI, hotkey listening, and auto-clicking
- **X11 Integration**: Low-level display server interaction for global hotkeys

#### Key Components

##### Global Hotkey Listener (`global_listen.c`)
- Captures F8 keypresses system-wide using X11
- Thread-safe flag management with mutexes
- Automatic cleanup of X11 resources

##### Auto Clicker (`auto_click.c`)
- Creates virtual uinput device for mouse simulation
- Sends realistic mouse click events to kernel
- Configurable timing with microsecond precision

##### GUI Controller (`main.c`)
- GTK signal handlers for all user interactions
- Thread creation and management
- State synchronization between components

##### Shared Variables (`shared_vars.h`)
- Thread-safe global state management
- Mutex-protected shared variables
- Volatile declarations for thread communication

#### Thread Safety
- **GLib Mutex**: Protects all shared variable access
- **Atomic Operations**: Flag checks and updates are synchronized
- **Resource Cleanup**: Proper cleanup on thread exit

#### System Integration
- **uinput Kernel Module**: Required for mouse event simulation
- **X11 Display Server**: Required for global hotkey capture
- **Root Privileges**: May be needed for `/dev/uinput` access

### Getting Started

#### Prerequisites
- Linux operating system
- GTK 3 development libraries
- X11 development libraries
- uinput kernel module support
- GCC compiler

#### Building
```bash
# Clone or navigate to project directory
cd HyAutoClicker

# Run the compile script
./compile.script
```

#### Running
```bash
# May require root privileges for uinput access
sudo ./main-bin
```

### Usage Guide

1. **Launch Application**: Run `./main-bin` (with sudo if needed)
2. **Configure Settings**:
   - Switch to Settings tab
   - Choose click type (left/right)
   - Set desired click interval
3. **Start Listening**: Click "Start" button in Auto Clicker tab
4. **Activate Auto-Clicking**: Press F8 key anywhere in the system
5. **Stop Auto-Clicking**: Press F8 again to toggle off
6. **Stop Listening**: Click button again or switch tabs while listening

### Troubleshooting

#### Common Issues

**"Hotkey not activating"**
- Ensure application window has focus when pressing F8
- Check that listening mode is active ("Listening..." status)
- Verify X11 permissions and display connection

**"File Open Error" for uinput**
- Run with `sudo` for root access to `/dev/uinput`
- Ensure uinput kernel module is loaded: `lsmod | grep uinput`

**"Failed to open display"**
- Ensure X11 is running and DISPLAY environment variable is set
- Try running from a graphical terminal within X session

**Application won't start**
- Check GTK libraries are installed
- Verify all source files compiled successfully
- Check for missing dependencies in compile output

#### Debug Output
The application provides verbose console output for troubleshooting:
- Thread startup/shutdown messages
- Hotkey press detection
- Click simulation confirmations
- Error messages for failed operations

### Project Structure

```
HyAutoClicker/
├── README.md                 # This file
├── compile.script           # Build script
├── HyAutoClicker.glade     # GTK UI definition
├── main-bin                # Compiled executable
├── include/
│   ├── shared_vars.h       # Global variable declarations
│   ├── global_listen.h     # Hotkey listener interface
│   ├── change_hotkey.h     # Hotkey change interface
│   └── auto_click.h        # Auto-clicker interface
└── src/
    ├── main.c              # GTK GUI and main logic
    ├── global_listen.c     # Global hotkey capture
    ├── auto_click.c        # Mouse click simulation
    └── change_hotkey.c     # Hotkey reconfiguration
```

### Security Considerations

- **Root Access**: Requires elevated privileges for uinput device access
- **System-wide Input**: Can interfere with other applications
- **Resource Usage**: Minimal CPU and memory footprint
- **Thread Safety**: Protected against race conditions

### Requirements

#### Software Dependencies
- GTK 3.0+
- GLib 2.0+
- X11 development libraries
- Linux kernel with uinput support

#### Hardware Requirements
- Linux-compatible system
- Mouse input device
- Keyboard for hotkey input

### Contributing

This project demonstrates:
- GTK application development
- Multi-threaded programming in C
- X11 system integration
- Linux input subsystem usage
- Thread synchronization techniques