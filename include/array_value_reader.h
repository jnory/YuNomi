#ifndef YUNOMI_ARRAY_VALUE_READER_H_
#define YUNOMI_ARRAY_VALUE_READER_H_

#include<vector>
#include"value_reader.h"

namespace yunomi{
	template <class T>
	class array_value_reader : public value_reader<T>{
	public:
		array_value_reader(std::vector<T> &array){
			this->array = &(array[0]);
			arraysize = array.size();
			front = 0;
		}

		array_value_reader(T *array, size_t arraysize){
			this->array = array;
			this->arraysize = arraysize;
			front = 0;
		}

		virtual ~array_value_reader(){}

		virtual size_t size(){
			return arraysize;
		}

		virtual T pop_front(){
			return array[front++];
		}

		virtual void move_head(){
			front = 0;
		}

	private:
		T *array;
		size_t arraysize;
		size_t front;
	};
}

#endif //YUNOMI_ARRAY_VALUE_READER_H_

