#include "LoadBinaryData.h"
#include <string>
#include <fstream>

bool readBinaryData(std::vector<int> *departureDelays, std::vector<int> *arrivalDelays, std::string filename) {
	std::ifstream in(filename, std::ifstream::binary);
	if (in) {
		// get length of file:
		in.seekg(0, in.end);
		int length = in.tellg();
		in.seekg(0, in.beg);

		int oneArraySize = (length / 4) / 2; //byte to int32
		departureDelays->reserve(oneArraySize);
		arrivalDelays->reserve(oneArraySize);
		for (int i = 0; i < oneArraySize; i++) {
			int tmp;
			in >> tmp;
			departureDelays->at(i) = tmp;
			in >> tmp;
			arrivalDelays->at(i) = tmp;
		}

		return(true);

	}
	else {
		return false;
	}

	
	//departureDelay = new 

}