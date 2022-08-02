#include <unistd.h>		// getopt
#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

vector<vector<uint8_t>> bitMap;

char characters[64] =  { 
    '$', '@', 'B', '%', '8', '&', 'W', 'M', 
    '#', '*', 'o', 'a', 'h', 'k', 'b', 'd', 
    'p', 'q', 'w', 'm', 'Z', 'O', '0', 'Q', 
    'L', 'C', 'J', 'U', 'Y', 'X', 'z', 'c', 
    'v', 'u', 'n', 'x', 'r', 'j', 'f', 't', 
    '/', '\\', '|', '1', '{', '[', '?', '-', 
    '_', '+', '~', '<', 'i', '!', 'l', 'I', 
    ';', ':', ',', '\"', '^', '`', '\'', ' ',
};

int width = 0;
int height = 0;

void textParser(string outputDirectory) {
    string outputString;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            outputString += characters[bitMap[x][y] / 4];
        }
        outputString += '\n';
    }

    // Open file
    ofstream file;
    file.open(outputDirectory, ios::out | ios::trunc);
    file << outputString;
    file.close();
}

void imageParser(string inputDirectory) {
	Mat preImage;
	preImage = imread(inputDirectory, CV_8UC1);
	Mat image;
	resize(preImage, image, Size(width, height), INTER_NEAREST);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			bitMap[x][y] = image.at<uchar>(x, y);
		}
	}
}

void cmdHelp() {
    cout << "ASCII Art generator" << endl;
    cout << "Parameters:" << endl;
    cout << "-p Directory of input picture" << endl;
    cout << "-o Output directory" << endl;
	cout << "-w Width of text image" << endl;
    cout << "-h Height of text image" << endl;
}

int main(int argc, char* argv[]) {
	string pictureDirectory;
	string output;

	// Parse arguments
	int opt;
	while ((opt = getopt(argc, argv, "p:o:w:h:")) != -1) {
		switch (opt) {
			case 'p':
				pictureDirectory = optarg;
				continue;
			case 'o':
				output = optarg;
				continue;
				
			// Cant be bothered to swap width and height
			case 'w':
				height = stoi(optarg);
				continue;
			case 'h':
				width = stoi(optarg);
				continue;
		}
	}
	if (pictureDirectory == "" || output == "" || width == 0 || height == 0) {
		cmdHelp();
		return 0;
	}

	// Parse bitmap into correct size
	bitMap.resize(width);
	for (int x = 0; x < width; x++) {
		bitMap[x].resize(height);
	}
	

	// Parse image into correct bitmap resolution
	imageParser(pictureDirectory);

	// Parse bitmap into text
	textParser(output);
}