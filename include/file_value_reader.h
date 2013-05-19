#ifndef YUNOMI_FILE_VALUE_READER_H_
#define YUNOMI_FILE_VALUE_READER_H_

#include<string>
#include<fstream>
#include"value_reader.h"

namespace yunomi{
	template <class T>
	class file_value_reader : public value_reader<T>{
	public:
		file_value_reader(std::string &fname){
			ifs = new std::ifstream(fname.c_str());

			datasize = 0;
			T value=0;
			(*ifs) >> value;
			while(!ifs->eof()){
				datasize++;
				
				(*ifs) >> value;
			}

			move_head();
		}

		virtual ~file_value_reader(){
			delete ifs;
		}

		virtual size_t size(){
			return datasize;
		}

		virtual T pop_front(){
			T value=0;
			(*ifs) >> value;
			return value;
		}

		virtual void move_head(){
			ifs->clear();
			ifs->seekg(0, std::fstream::beg);
		}

	private:
		std::ifstream *ifs;
		size_t datasize;
	};
}

#endif //YUNOMI_FILE_VALUE_READER_H_

