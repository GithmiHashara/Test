# IMU Health Monitor

A Qt-based desktop application for monitoring IMU (Inertial Measurement Unit) devices with an enhanced toolbar interface.

## Features

### Enhanced Toolbar
- **Zoom In/Out**: Zoom controls for detailed view of IMU data visualization
- **Fit to Window**: Automatically fit content to window size
- **Hand Tool**: Pan/move content with click and drag functionality
- **Connect/Disconnect**: Toggle connection to IMU devices
- **Responsive Design**: Toolbar adapts to window width changes
- **Professional Styling**: Modern UI with consistent button states

### Toolbar Shortcuts
- **Ctrl++**: Zoom In
- **Ctrl+-**: Zoom Out
- **Ctrl+0**: Fit to Window
- **H**: Toggle Hand Tool
- **Ctrl+C**: Connect/Disconnect

## Technical Features

### Responsive Toolbar Design
- Automatically switches to compact mode when window width < 800px
- Icon sizes adapt from 24px (normal) to 16px (compact)
- Efficient space utilization with reduced padding and margins
- Maintains functionality across all window sizes

### Professional UI Elements
- Custom styling with hover and pressed states
- Visual grouping with separators
- Status bar with connection and zoom status
- Modern Fusion style with custom color scheme

## Building the Application

### Prerequisites
- Qt6 development libraries
- CMake 3.16 or higher
- C++17 compatible compiler

### Build Instructions

#### Using CMake
```bash
mkdir build
cd build
cmake ..
make
```

#### Using qmake
```bash
qmake IMUHealthMonitor.pro
make
```

## File Structure

- `imuhealthmonitor.h/cpp` - Main application window with IMU visualization
- `toolbarmanager.h/cpp` - Enhanced toolbar implementation with responsive design
- `main.cpp` - Application entry point with modern styling
- `CMakeLists.txt` - CMake build configuration
- `IMUHealthMonitor.pro` - Qt project file for qmake

## Icon Integration

The application is designed to use custom icons from:
`D:/1-A Excel Tech/AAA UI/NEW_IMU_V01/Images/`

Required icon files:
- `zoomin.png` - Zoom in icon
- `zoomout.png` - Zoom out icon
- `fittowindow.png` - Fit to window icon
- `handtool.png` - Hand tool icon
- `connect.png` - Connection toggle icon

If custom icons are not found, the application falls back to system icons.

## Usage

1. Launch the application
2. Use toolbar buttons or keyboard shortcuts for navigation
3. Click "Connect" to establish connection with IMU device
4. Use zoom controls to inspect data visualization
5. Enable Hand Tool for panning large diagrams
6. Window can be resized - toolbar automatically adapts

## Implementation Highlights

### Space-Efficient Design
- Reduced button padding and margins
- Compact icon sizes for narrow windows
- Intelligent spacing using CSS-style toolbar styling

### State Management
- Proper handling of checkable actions (Hand Tool, Connection)
- Visual feedback for all toolbar states
- Status bar updates for user guidance

### Extensible Architecture
- Clean separation between toolbar management and main application
- Easy to add new toolbar actions
- Modular design for future enhancements