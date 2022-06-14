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
	return out;
}


void vector_bench() {
	double vec1_total(0);
	double vec2_total(0);
	double vec1acc_total(0);
	double vec2acc_total(0);
	double vec1er_total(0);
	double vec2er_total(0);
	for (int k = 0; k < 30; k++) {
		Gore::Vector<TESTTYPE> vec1;
		std::vector<TESTTYPE> vec2;
		std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
		for (int i = 0; i < 1000; i++) {
			TESTTYPE tep = { rand() % 1000, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 };
			vec2.push_back(tep);
		}
		std::chrono::steady_clock::time_point time2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
		vec2_total += time_span.count();


		time1 = std::chrono::steady_clock::now();
		for (int i = 0; i < 1000; i++) {
			TESTTYPE tep = { 101, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 };
			vec1.push_back(tep);
		}
		time2 = std::chrono::steady_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
		vec1_total += time_span.count();


		
		time1 = std::chrono::steady_clock::now();
		for (int i = 0; i < vec1.size(); i++) {
			vec1[i]->x++;
		}
		time2 = std::chrono::steady_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
		vec1acc_total += time_span.count();

		time1 = std::chrono::steady_clock::now();
		for (int i = 0; i < vec2.size(); i++) {
			vec2[i].x++;
		}
		time2 = std::chrono::steady_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
		vec2acc_total += time_span.count();




		time1 = std::chrono::steady_clock::now();
		for (int i = 0; i < 100; i++) {
			int r = rand() % 10;
			vec1.erase(r);
		}
		time2 = std::chrono::steady_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
		vec1er_total += time_span.count();

		time1 = std::chrono::steady_clock::now();
		for (int i = 0; i < 100; i++) {
			int r = rand() % 10;
			vec2.erase(vec2.begin() + r);
		}
		time2 = std::chrono::steady_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
		vec2er_total += time_span.count();


		vec2.clear();
		vec1.clear();
	}
	double out = vec2_total / 30;
	std::cout << "Average time for std::vector, push_back: " << out << "\n";
	out = vec1_total / 30;
	std::cout << "Average time for Gore::vector, push_back: " << out << "\n";

	out = vec2acc_total / 30;
	std::cout << "Average time for std::vector, access: " << out << "\n";
	out = vec1acc_total / 30;
	std::cout << "Average time for Gore::vector, access: " << out << "\n";

	out = vec2er_total / 30;
	std::cout << "Average time for std::vector, erase: " << out << "\n";
	out = vec1er_total / 30;
	std::cout << "Average time for Gore::vector, erase: " << out << "\n";
}


int main(){
	srand(time(NULL));
	Gore::MultiVector ar1;
	ar1.reserve(200);
	Gore::Vector<TESTTYPE> vec1;
	std::vector<TESTTYPE> vec2;
	//speed testing my vector against standard library one
	vector_bench();
	for (int i = 0; i < 1000; i++) {
		TESTTYPE tep = { 101, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 };
		vec1.push_back(tep);
	}
	TESTTYPE t = { 100, 1000, 0.2f, 100.524 };
	//just testing my vector
	std::cout << vec1[20]->x << std::endl;
	vec1[20]->x = 100;
	std::cout << vec1[20]->x << std::endl;
	vec1[21]->x = 205;
	vec1.erase(20);
	std::cout << vec1[20]->x << std::endl;
	vec1.push_back(t);
	vec1.insert(20, t);
	vec1.push_back(t);
	for (size_t i = 0; i < vec1.size(); i++) {
		std::cout << i << " :" << vec1[i]->x << " \n";
	}
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
	TESTTYPE* t1;
	t1 = (TESTTYPE*)ar1[0].data;
	t1->x += 1;
	std::vector<TESTTYPE*> test1;
	std::vector<EVERTYPE*> test2;
	bool exitf = false;
	ar1.erase(20);
	ar1.push_back((char*)&t, sizeof(TESTTYPE), 0);
	ar1.insert(20, (char*)&t, sizeof(TESTTYPE), 0);
	for (size_t i = 0; i < ar1.getSize(); i++) {
		//deserilize each set of data differently into their corresponding structs
		switch (ar1[i].type) {
		case 0:
			//TESTTYPE tp;
			//deserilizeStruct((char*)&tp, ar1.stor + ar1[i].offset, ar1[i].size);
			test1.push_back((TESTTYPE*)ar1[i].data);
			break;
		case 1:
			//EVERTYPE te;
			//deserilizeStruct((char*)&te, ar1.stor + ar1[i].offset, ar1[i].size);
			test2.push_back((EVERTYPE*)ar1[i].data);
			break;
		}
	}
	test1;
	test2;

	std::cout << sizeof(Gore::MultiVector) << std::endl;

	return 0;
}