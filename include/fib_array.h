#ifndef YUNOMI_FIB_ARRAY_H_
#define YUNOMI_FIB_ARRAY_H_

#include<iostream>
#include<math.h>
#include"y_bitarray.h"
#include"fib_coder.h"
#include"value_reader.h"
#include"y_select11.h"

namespace yunomi {
	template <class T> class fib_array{
	public:
		fib_array(value_reader<T> &vr){
			fc = new fib_coder<T>();

			size_t size = vr.size();
			size_t bitssize = count_bitssize(vr);
			vr.move_head();

			bits = new bitarray(bitssize);
			
			uint64_t code=0;
			size_t bitsize=0;
			for(size_t i = 0; i < size; i++){
				fc->encode(vr.pop_front(), code, bitsize);
				bits->push_back(code, bitsize);
			}
			bits->pack();
#ifdef YUNOMI_DEBUG
			std::cerr << "[fib_array] size=" << bits->size() << std::endl;
#endif
			
			sel11 = new select11(bits);
		}

		fib_array(FILE *fp){
			fc = new fib_coder<T>(fp);
			bits = new bitarray(fp);
			sel11 = new select11(fp,bits);
		}

		T operator[](size_t i){
			size_t start = sel11->select(i);
			size_t end = sel11->select(i+1)-1;

#ifdef YUNOMI_DEBUG
			std::cerr << "[" << i << "] (start,end)=(" << start << "," << end << ")" << std::endl;
#endif
			size_t size=end-start+1;
			
			uint64_t fibcode=bits->get(start,size);
			return fc->decode(fibcode, size);
		}
		
		~fib_array(){
			delete fc;
			delete bits;
			delete sel11;
		}

		void dump(FILE *fp){
			fc->dump(fp);
			bits->dump(fp);
			sel11->dump(fp);
		}

	private:
		size_t count_bitssize(value_reader<T> &vr){
			size_t size = vr.size();
			size_t bitssize=0;
			for(size_t i = 0; i < size; i++){
				bitssize+=fc->get_bitsize(vr.pop_front());
			}

			return bitssize;
		}
		
		bitarray *bits;
		select11 *sel11;
		fib_coder<T> *fc;
	};
}

#endif // YUNOMI_FIB_ARRAY_H_

