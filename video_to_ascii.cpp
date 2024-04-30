#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace fs = std::filesystem;

char pixelToASCII(int pixelIntensity) {
    static const std::string ASCII_CHARS = ".%#*+=-:_^- ";
    return ASCII_CHARS[pixelIntensity * ASCII_CHARS.size() / 256];
}

void clearScreen() {
#ifdef _WIN32
    system("cls");  // Clear screen command for Windows
#else
    std::cout << "\033[2J\033[H";  // ANSI escape codes for Unix/Linux
#endif
}

void getTerminalSize(int& width, int& height) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
#endif
}

void displayFrame(const std::string& framePath, int frameRate) {
    auto timePerFrame = std::chrono::milliseconds(1000 / frameRate);
    std::string line;
    int t = 1;
    while (true) {
        std::ifstream frameFile(framePath + std::to_string(t) + ".txt");
        if (!frameFile) break;  // Exit if no more frames
        clearScreen();
        while (getline(frameFile, line)) {
            std::cout << line << '\n';
        }
        std::this_thread::sleep_for(timePerFrame);  // Control frame rate
        t++;
    }
}

void processVideo(const std::string& videoPath, int desiredWidth, int desiredHeight) {
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open video." << std::endl;
        return;
    }
    cv::Mat frame;
    int frameNumber = 0;
    std::string outputPath = "./output";
    fs::create_directories(outputPath);  // Ensure output directory exists
    while (cap.read(frame)) {
        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
        cv::resize(grayFrame, grayFrame, {desiredWidth, desiredHeight}, 0, 0, cv::INTER_AREA);
        std::ofstream outFile(outputPath + "/frame" + std::to_string(++frameNumber) + ".txt");
        if (!outFile) {
            std::cerr << "Failed to open output file." << std::endl;
            continue;
        }
        for (int i = 0; i < grayFrame.rows; ++i) {
            for (int j = 0; j < grayFrame.cols; ++j) {
                int pixelIntensity = grayFrame.at<uchar>(i, j);
                char asciiChar = pixelToASCII(pixelIntensity);
                outFile << asciiChar;
            }
            outFile << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <video_path>\n";
        return 1;
    }

    int terminalWidth, terminalHeight;
    getTerminalSize(terminalWidth, terminalHeight);
    std::string videoPath = argv[1];
    processVideo(videoPath, terminalWidth, terminalHeight);  // Divided by 2 to adjust for character height
    displayFrame("./output/frame", 24);  // Example frame rate
    return 0;
}
