# GUI Components

This directory contains all GUI-related components for the Campo application.

## Grid Display

The `GridDisplay` class in `grid_display.hpp` manages how cameras are displayed in the application.

### Features

- **Grid View**: Displays multiple cameras in a grid layout
- **Single Camera View**: Allows selecting a camera to view it in full-panel size
- **Camera selection**: Click on a camera feed to expand it
- **Return to Grid**: Use the "Return to Grid View" button in the left panel to go back to grid view

### Usage

In single camera view:
1. Only the selected camera feed is updated
2. Other camera feeds are frozen (not updated) to conserve resources
3. The camera is displayed at the maximum possible size while maintaining aspect ratio

To return to grid view, click the "Return to Grid View" button at the bottom of the left panel.
