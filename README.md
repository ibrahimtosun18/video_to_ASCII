# Video to ASCII Converter

Convert any video file into ASCII art that plays directly in your terminal. This cross-platform application uses OpenCV to process video frames, mapping them to ASCII characters for a unique visual representation. It is designed to run on both Linux and Windows systems.

## Features

- **Video to ASCII Conversion**: Transforms video frames into ASCII art.
- **Automatic Terminal Resizing**: Adapts output to fit your terminal window, maximizing the use of available space.
- **Cross-Platform Support**: Works on Linux and Windows, utilizing platform-specific features to enhance usability.

## Prerequisites

To run the Video to ASCII Converter, you need:
- **C++ Compiler**: GCC or Clang on Linux, MSVC on Windows.
- **OpenCV**: Library for handling video processing.
- **CMake**: Optional for easier build configuration (especially on Windows).

## Installation

### Linux
1. **Install OpenCV**:
   ```bash
   sudo apt install libopencv-dev
   ```
2. **Compile the project***
      ```bash
      g++ -std=c++17 -o video_to_ascii video_to_ascii.cpp `pkg-config --cflags --libs opencv4`
      ```
### Windows Users

1. **Install OpenCV**:
   - Download and install from [OpenCV official site](https://opencv.org/releases/).
   - Set environment variables to include OpenCV directories.

2. **Compile with Visual Studio**:
   - Create a new project and include OpenCV directories in the project settings.
   - Add `video_to_ascii.cpp` to the project and build.



### Usage
      
      ./video_to_ascii /path/to/your/video.mp4  
      
Replace /path/to/your/video.mp4 with the actual path of your desired video


## Contact

For further guidance or any queries, feel free to contact:

- Ibrahim - [ibrahim.tsn18@gmail.com](mailto:ibrahim.tsn18@gmail.com)




      
