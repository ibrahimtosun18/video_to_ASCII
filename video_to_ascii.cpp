#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>

namespace fs = std::filesystem;

char pixelToASCII(int pixelIntensity) {
    static const std::string ASCII_CHARS = ".%*+=-:- ";
    return ASCII_CHARS[pixelIntensity * ASCII_CHARS.size() / 256];
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
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

    // Calculate optimal width and height based on your terminal size
    int terminalWidth = 200;  // Example, adjust to your terminal's width capacity
    int terminalHeight = 60;  // Example, adjust to your terminal's height capacity
    std::string videoPath = argv[1];
    processVideo(videoPath, terminalWidth, terminalHeight);
    displayFrame("./output/frame", 24);  // Example frame rate
    return 0;
}
