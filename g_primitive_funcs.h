#pragma once
#include <iostream>
#include <vector>



namespace Gore {

	struct FreeList {
		size_t offset;
		FreeList* next;
	};

	//just memcpy data back instead of using freelist will be more efficent
	template<class T>
	class Vector {
	private:
		T* stor;
		size_t offset;
		size_t byte_size;
		size_t allocd;
	public:
		Vector() {
			stor = (T*)std::malloc(sizeof(T));
			offset = 0;
			byte_size = 0;
			allocd = sizeof(T);
		}
		void push_back(T in) {
			if (byte_size + sizeof(T) > allocd) {
				T* temp = (T*)std::realloc(stor, allocd + sizeof(T));
				if (temp != NULL) {
					stor = temp;
				}
				allocd += sizeof(T);
			}
			*(stor + offset) = in;
			offset++;
			byte_size += sizeof(T);
		}
		void erase(int n) {
			std::memcpy(stor + n, stor + n + 1, (offset - n) * sizeof(T));
			offset--;
		}
		size_t size() {
			return offset;
		}
		void reserve(size_t n) {
			T* temp = (T*)std::realloc(stor, allocd + (sizeof(T) * n));
			if (temp != NULL) {
				stor = temp;
			}
			allocd += sizeof(T) * n;
		}
		void pop_back() {
			offset--;
		}
		void insert() {
			//will involve memcpy and realloc depending on space available 
		}
		void clear() {
			offset = 0;
		}
		T& operator[] (int n) {
			return *(stor + n);
		}
		//const T& operator[] (int n) {
			//return stor + n;
		//}
		~Vector() {
			std::free(stor);
		}
	};


	//use offset within data instead to avoid the pointer being made useless if realloc moves chunk
	struct StoreElement {
		size_t offset;
		size_t size;
		size_t type;
	};
	//returns char pointer which you convert to the struct you want
	struct ReturnElement {
		char* data;
		size_t type;
	};
	//keep track of your own types, I don't feel like writing a reflection system
	class MultiVector {
	private:
		//stores the location of data and its size
		std::vector<StoreElement> elements;
		//stores gaps made by erase
		std::vector<StoreElement> gaps;
		//where to write in the data
		char* index;
		//the number of bytes written
		size_t size;
		//the number of bytes allocated
		size_t allocd;
	public:
		//the actual pointer to the data
		char* stor;
		MultiVector() {
			stor = (char*)std::malloc(32);
			index = stor;
			size = 0;
			allocd = 32;
		}
		void push_back(char* data, size_t insize, size_t type) {
			char* pos = index;
			size_t off = size;
			//checking if item fits into any of the gaps
			for (int i = 0; i < gaps.size(); i++) {
				if (gaps[i].size <= insize) {
					char* t = stor + gaps[i].offset;
					off = gaps[i].offset;
					for (size_t j = 0; j < gaps[i].size; j++) {
						*t = data[j];
						t++;
					}
					StoreElement st = { off, insize, type };
					elements.push_back(st);
					gaps.erase(gaps.begin() + i);
					return;
				}
			}

			if (size + insize > allocd) {
				char* temp = (char*)std::realloc(stor, allocd + insize);
				if (temp != NULL) {
					stor = temp;
					index = stor;
					//moving index back to where it was before we reallocd the memory
					for (int i = 0; i < size; i++) {
						index++;
					}
				}
				pos = index;
				allocd += insize;
			}
			for (size_t i = 0; i < insize; i++) {
				*index = data[i];
				index++;
				size++;
			}
			StoreElement st = { off, insize, type };
			elements.push_back(st);
		}
		void erase(size_t n) {
			gaps.push_back(elements[n]);
			elements.erase(elements.begin() + n);
		}
		void pop_back() {
			size -= elements[elements.size() - 1].size;
			index -= elements[elements.size() - 1].size;
			elements.erase(elements.begin() + (elements.size() - 1));
		}
		void reserve(size_t in) {
			char* temp = (char*)std::realloc(stor, allocd + in);
			if (temp != NULL) {
				stor = temp;
				index = stor;
				for (int i = 0; i < size; i++) {
					index++;
				}
			}
			allocd += in;
		}

		size_t getSize() {
			return elements.size();
		}

		ReturnElement operator[](int n) {
			return { stor + elements[n].offset, elements[n].type };
		}

		const ReturnElement operator[](int n) const {
			return{ stor + elements[n].offset, elements[n].type };
		}

	};
}