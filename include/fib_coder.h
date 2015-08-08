#ifndef YUNOMI_FIB_CODER_H_
#define YUNOMI_FIB_CODER_H_

#include<vector>
#include"y_defines.h"

namespace yunomi {
	template <class T> class fib_coder {
	public:
		fib_coder(){
			fibs.push_back(1);
			fibs.push_back(1);
		}

		fib_coder(FILE *fp){
			size_t size;
			fread(&size, sizeof(size_t), 1, fp);

			T *data = new T[size];
			fread(data, sizeof(T), size, fp);

			fibs.reserve(size);
			for(size_t i = 0; i < size; i++){
				fibs.push_back(data[i]);
			}
			delete [] data;
		}

		size_t get_bitsize(T value){
			return get_fib_index(value, fibs.size()-1)+1;
		}

		void encode(T value, uint64_t &code, size_t &bitsize){
			bitsize = get_fib_index(value, fibs.size()-1)+1;
			code = (0x3ULL << (bitsize-2));

			size_t max = bitsize-2;
			value -= fibs[bitsize-1];
			while(value>0){
				size_t index = get_fib_index(value, max);
				code |= (0x1ULL << (index-1));
				
				max = index-2;
				value -= fibs[index];
			}
		}

		T decode(uint64_t code, size_t bitsize){
			T ret=0;
			for(size_t i = 1; i < bitsize; i++){
				if((code&0x1ULL)==0x1ULL){
					ret += fibs[i];
					code>>=2;
					i++;
				}else{
					code>>=1;
				}
			}
			return ret;
		}

		void dump(FILE *fp){
			size_t size = fibs.size();
			fwrite(&size, sizeof(size_t), 1, fp);
			fwrite(&(fibs[0]), sizeof(T), fibs.size(), fp);
		}

	private:
		size_t get_fib_index(T value, size_t max){
			if(value > fibs.back()){
				size_t fibssize = fibs.size();
				while(value >= fibs.back()){
					fibs.push_back(fibs[fibssize-2]+fibs[fibssize-1]);
					fibssize++;
				}
					
				return fibssize-2;
			}else{
				size_t min = 0;

				while(min <= max){
					size_t mid = (min+max)/2;
					if(fibs[mid]>value){
						max=mid-1;
					}else if(fibs[mid]<value){
						min=mid+1;
					}else{
						if(mid==0) mid++;
						return mid;
					}
				}
			}

			return max;
		}

		std::vector<T> fibs;
	};
}

#endif // YUNOMI_FIB_CODER_H_

