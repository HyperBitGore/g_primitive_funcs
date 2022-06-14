#pragma once
#include <iostream>
#include <vector>



namespace Gore {

	struct FreeList {
		size_t offset;
		FreeList* next;
	};
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
			//remove if statement somehow for increased performance
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
			//move entire chunk back by one and offset is reduced by one 
			std::memcpy(stor + n, stor + n + 1, (offset - n) * sizeof(T));
			offset--;
			byte_size -= sizeof(T);
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
			//don't need to touch data will just be copied over next push_back
			byte_size -= sizeof(T);
			offset--;
		}
		void insert(int n, T in) {
			//checking if enough space to memcpy over
			if (byte_size + sizeof(T) > allocd) {
				T* temp = (T*)std::realloc(stor, allocd + sizeof(T));
				if (temp != NULL) {
					stor = temp;
				}
				allocd += sizeof(T);
			}
			//copy current data over by 1 offset
			std::memcpy(stor + n + 1, stor + n, (offset - n) * sizeof(T));
			*(stor + n) = in;
			byte_size += sizeof(T);
			offset++;
		}
		void clear() {
			//just reset entire block of data, without deallocating it
			std::memset(stor, 0, offset * sizeof(T));
			offset = 0;
			byte_size = 0;
		}
		T& operator[] (int n) {
			return *(stor + n);
		}
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
	//could probably optimize this pretty hard
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
		//the actual pointer to the data
		char* stor;
	public:
		MultiVector() {
			stor = (char*)std::malloc(32);
			index = stor;
			size = 0;
			allocd = 32;
		}
		void push_back(char* data, size_t insize, size_t type) {
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
		void insert(int n, char* data, size_t insize, size_t type) {
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
					elements.insert(elements.begin() + n, st);
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
				allocd += insize;
			}
			for (size_t i = 0; i < insize; i++) {
				*index = data[i];
				index++;
				size++;
			}
			elements.insert(elements.begin() + n, {off, insize, type});
			size += insize;
			index += insize;
		}
		void clear() {
			elements.clear();
			size = 0;
			index = stor;
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