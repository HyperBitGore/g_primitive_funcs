#include "g_primitive_funcs.h"
#include <random>
#include <time.h>
#include <chrono>



char* serilizeStruct(char* ptr, int size) {
	char* mt = (char*)std::malloc(size);
	for (int i = 0; i < size; i++) {
		mt[i] = *ptr;
		ptr++;
	}
	return mt;
}
//overload 1, used for char arrays, so no use of malloc, make sure output array is big enough to hold size
void serilizeStruct(char* ptr, char output[], int size) {
	for (int i = 0; i < size; i++) {
		output[i] = *ptr;
		ptr++;
	}
}
void deserilizeStruct(char* dest, char* data, int size) {
	for (int i = 0; i < size; i++) {
		*dest = data[i];
		dest++;
	}
}

struct TESTTYPE {
	int x;
	int y;
	float z;
	double w;
};

struct EVERTYPE {
	float x;
	float y;
	int w;
	int h;
	uint8_t r;
};

float randFloat(float min, float max) {
	float out = rand() / static_cast<float>(RAND_MAX / (max - min));
	std::cout << out << "\n";
	return out;
}

int main(){
	MuliArray::Array ar1;
	srand(time(NULL));

	TESTTYPE t = { 100, 1000, 0.2f, 100.524 };
	ar1.push_back((char*)&t, sizeof(TESTTYPE), 0);
	for (int i = 0; i < 100; i++) {
		int r = rand() % 100;
		if (r > 51) {
			TESTTYPE t = { rand() % 1000, rand() % 1000, randFloat(0.0001f, 1000.5f), 100.524 };
			ar1.push_back((char*)&t, sizeof(TESTTYPE), 0);
		}
		else {
			EVERTYPE et = { randFloat(0.0001f, 1000.5f), randFloat(0.0001f, 1000.5f), rand() % 100, rand() % 100, 255 };
			ar1.push_back((char*)&et, sizeof(EVERTYPE), 1);
		}
	}
	TESTTYPE t1;
	deserilizeStruct((char*)&t1, ar1.stor + ar1[0].offset, ar1[0].size);
	t1.x += 1;

	std::vector<TESTTYPE> test1;
	std::vector<EVERTYPE> test2;
	bool exitf = false;
	//while (!exitf) {

		for (size_t i = 0; i < ar1.getSize(); i++) {
			//deserilize each set of data differently into their corresponding structs
			switch (ar1[i].type) {
			case 0:
				TESTTYPE tp;
				deserilizeStruct((char*)&tp, ar1.stor + ar1[i].offset, ar1[i].size);
				test1.push_back(tp);
				break;
			case 1:
				EVERTYPE te;
				deserilizeStruct((char*)&te, ar1.stor + ar1[i].offset, ar1[i].size);
				test2.push_back(te);
				break;
			}
		}
	//}
	test1;
	test2;

	std::cout << sizeof(MuliArray::Array) << std::endl;

	return 0;
}