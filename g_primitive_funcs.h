#pragma once
#include <iostream>
#include <vector>


namespace MuliArray {
	//use offset within data instead to avoid the pointer being made useless if realloc moves chunk
	struct StoreElement {
		size_t offset;
		size_t size;
		size_t type;
	};
	//keep track of your own types, I don't feel like writing a reflection system
	class Array {
	private:
		std::vector<StoreElement> elements;
		//where to write in the data
		char* index;
		//the number of bytes written
		size_t size;
		//the number of bytes allocated
		size_t allocd;
	public:
		//the actual pointer to the data
		char* stor;
		Array() {
			stor = (char*)std::malloc(32);
			index = stor;
			size = 0;
			allocd = 32;
		}
		void push_back(char* data, size_t insize, size_t type) {
			char* pos = index;
			size_t off = size;
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
		//untested
		void erase(size_t n) {
			size -= elements[n].size;
			index -= elements[n].size;
			elements.erase(elements.begin() + n);
		}
		//untested
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

		StoreElement& operator[](int n) {
			//ReturnElement r = {stor + elements[n].offset, elements[n].size, elements[n].type};
			return elements[n];
		}

		const StoreElement& operator[](int n) const {
			//ReturnElement r = { stor + elements[n].offset, elements[n].size, elements[n].type };
			return elements[n];
		}

	};
}