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
	string outputDirectory;
	int width = 0;
	int height = 0;

	// Parse arguments
	int opt;
	while ((opt = getopt(argc, argv, "p:o:w:h:")) != -1) {
		switch (opt) {
			case 'p':
				pictureDirectory = optarg;
				continue;
			case 'o':
				outputDirectory = optarg;
				continue;
				
			// Cant be bothered to swap width and height
			case 'w':
				width = stoi(optarg);
				continue;
			case 'h':
				height = stoi(optarg);
				continue;
		}
	}
	if (pictureDirectory == "" || outputDirectory == "" || width == 0 || height == 0) {
		cmdHelp();
		return 0;
	}

	// Parse bitmap into correct size
	bitMap.resize(height);
	for (int x = 0; x < height; x++) {
		bitMap[x].resize(width);
	}
	

	Mat image;
	image = imread(pictureDirectory, CV_8UC1);
	for (int y = 0; y < height; y++) {
		int yPixel = image.size().height / height * y;
		for (int x = 0; x < width; x++) {
			int xPixel = image.size().width / width * x;
			bitMap[y][x] = image.at<uchar>(yPixel, xPixel);
		}
	}

	// Parse bitmap into text 
	string outputString;
	for (int y = 0; y < bitMap.size(); y++) {
		for (int x = 0; x < bitMap[y].size(); x++) {
			outputString += characters[bitMap[y][x] / 4];
		}
		outputString += '\n';
	}

	// Print into file;
	ofstream file;
	file.open(outputDirectory, ios::out | ios::trunc);
	file << outputString;
	file.close();

}