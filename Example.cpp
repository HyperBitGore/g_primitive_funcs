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

class ITTEST {
private:
	int x;
	float y;
public:
	ITTEST() {
		x = rand() % 100;
		y = randFloat(0.01f, 1000.0f);
	}
	void Update() {
		x += rand() % 2;
		y += 0.05f;
	}
	int getX() {
		return x;
	}
	float getY() {
		return y;
	}
};

void testWork(float sx, float sy) {
	Gore::Vector<EVERTYPE> vec1;
	std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
	for (int i = 0; i < 10000; i++) {
		vec1.push_back({ randFloat(sx, 5000), randFloat(sy, 5000), 25, 25, 255 });
	}
	std::chrono::steady_clock::time_point time2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(time2 - time1);
	std::cout << "test work time: " << time_span.count() << "\n";
}

int hashTest(std::string in) {
	int size = 0;
	for (int i = 0; i < in.size(); i++) {
		size += in[i];
	}
	return size % 10;
}
int hashTest2(int in) {
	return in % 10;
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
			vec1[i].x++;
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
	Gore::ForwardList<TESTTYPE> flist;
	flist.insert({ 101, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 }, "1");
	flist.insert({ 101, rand() % 100, randFloat(0.0001f, 100.42f), 101.524 }, "2");
	flist.insert({ 101, rand() % 100, randFloat(0.0001f, 100.42f), 105.524 }, "bol");
	flist.removeBoth("bol");
	if (flist.search("bol") != nullptr) {
		std::cout << "found\n";
	}
	std::cout << flist.search("1")->w << "\n";
	std::cout << flist.search("2")->w << "\n";

	Gore::FBList<TESTTYPE> fblist;
	fblist.insert({ 101, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 }, "1");
	fblist.insert({ 101, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 }, "2");
	fblist.insert({ 101, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 }, "3");
	fblist.removeBoth("1");

	Gore::HashMap<EVERTYPE, std::string> map1;
	map1.setHashFunction(hashTest);
	map1.insert("first", { randFloat(0.0001f, 1000.5f), randFloat(0.0001f, 1000.5f), rand() % 100, rand() % 100, 255 });
	map1.insert("roguf", { randFloat(0.0001f, 1000.5f), randFloat(0.0001f, 1000.5f), rand() % 100, rand() % 100, 255 });
	map1.insert("fugor", { randFloat(0.0001f, 1000.5f), randFloat(0.0001f, 1000.5f), rand() % 100, rand() % 100, 255 });
	std::cout << map1.get("first")->w << std::endl;
	std::cout << map1.get("fugor")->w << std::endl;
	map1.remove("fugor");
	Gore::HashMap<EVERTYPE, int> map2;
	map2.reserve(10);
	map2.setHashFunction(hashTest2);
	map2.insert(50, { randFloat(0.0001f, 1000.5f), randFloat(0.0001f, 1000.5f), rand() % 100, rand() % 100, 255 });
	map2.insert(5, { randFloat(0.0001f, 1000.5f), randFloat(0.0001f, 1000.5f), rand() % 100, rand() % 100, 255 });
	std::cout << map2.get(50)->w << std::endl;
	std::cout << map2.get(5)->w << std::endl;

	Gore::GStack<int> stack1;
	stack1.push(0);
	stack1.push(2);
	stack1.push(4);
	std::cout << stack1.pop() << "\n";
	std::cout << stack1.pop() << "\n";

	Gore::Queue<int> queue1;
	for (int i = 0; i < 10; i++) {
		queue1.enqueue(i);
	}
	std::cout << queue1.dequeue() << "\n";
	std::cout << queue1.dequeue() << "\n";
	std::cout << queue1.dequeue() << "\n";

	Gore::Vector<int> p;
	for (int i = 0; i < 10; i++) {
		p.push_back(rand() % 100);
	}
	Gore::Sort<int>::Selectionsort(p);
	//Gore::Vector<int> out = Gore::Sort<int>::Mergesort(p);
	std::cout << "begin: \n";
	for (int i = 0; i < p.size(); i++) {
		std::cout << p[i] << "\n";
	}
	std::vector<int> pp;
	for (int i = 0; i < 10; i++) {
		pp.push_back(rand() % 100);
	}
	Gore::Sort<int>::Selectionsort(pp);
	//std::vector<int> out2 = Gore::Sort<int>::Mergesort(pp);
	std::cout << pp.size() << "\n";


	Gore::Vector<TESTTYPE> vec1;
	/*Gore::ThreadPool pool;
	pool.addJob(vector_bench);
	pool.addJob(vector_bench);
	pool.addJob(testWork, { 100.0f, 200.0f });*/
	std::vector<TESTTYPE> vec2;
	//speed testing my vector against standard library one
	vector_bench();
	for (int i = 0; i < 1000; i++) {
		TESTTYPE tep = { 101, rand() % 100, randFloat(0.0001f, 100.42f), 100.524 };
		vec1.push_back(tep);
	}
	for (auto& i : vec1) {
		i.w++;
	}
	//testing classes in my vector
	Gore::Vector<ITTEST> vecC;
	for (int i = 0; i < 50; i++) {
		ITTEST t;
		vecC.push_back(t);
		std::cout << t.getX() << "\n";
	}
	std::cout << "After Update\n";
	for (auto& i : vecC) {
		i.Update();
		std::cout << i.getX() << "\n";
	}
	TESTTYPE t = { 100, 1000, 0.2f, 100.524 };
	//just testing my vector
	std::cout << vec1[20].x << std::endl;
	vec1[20].x = 100;
	std::cout << vec1[20].x << std::endl;
	vec1[21].x = 205;
	vec1.erase(20);
	std::cout << vec1[20].x << std::endl;
	vec1.push_back(t);
	vec1.insert(20, t);
	vec1.push_back(t);
	for (size_t i = 0; i < vec1.size(); i++) {
		std::cout << i << " :" << vec1[i].x << " \n";
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
	system("pause");
	return 0;
}