#ifndef YUNOMI_FIB_ARRAY_H_
#define YUNOMI_FIB_ARRAY_H_

#include<iostream>
#include<math.h>
#include"y_bitarray.h"
#include"fib_coder.h"

namespace yunomi {
	template <class T> class fib_array{
	public:
		fib_array(T *data, size_t size){
			fc = new fib_coder<T>();		
			size_t bitssize = count_bitssize(data, size);

			init(bitssize);

			uint64_t *select = new uint64_t[size];

			size_t current=0;
			for(size_t i = 0; i < size; i++){
				select[i] = current;
				
				uint64_t code=0;
				size_t bitsize=0;
				fc->encode(data[i], code, bitsize);
				bits->push_back(code,bitsize);
				current+=bitsize;
			}

      plsize = (size+l1-1)/l1;
			pl_unit_size = ceil(log2(bitssize));

			pl = new bitarray(plsize*pl_unit_size);

			uint64_t maxdiff=0;
			for(size_t blockid=0; blockid < plsize; blockid++){
				pl->push_back(select[blockid*l1], pl_unit_size);
				for(size_t i=blockid*l1+1; i < (blockid+1)*l1 && i < size; i++){
					if(select[i]-select[blockid*l1]>maxdiff){
						maxdiff = select[i] - select[blockid*l1];
					}
				}
			}

			sl_unit_size=ceil(log2(maxdiff));
			sl = new bitarray(sl_unit_size*size);

			ss_unit_size=ceil(log2(l2));
			size_t sssize = size/l3;
			ss = new bitarray(ss_unit_size*sssize);

			for(size_t blockid=0; blockid < plsize; blockid++){
				uint64_t current_block = select[blockid*l1];
				uint64_t post_block;
				if(blockid+1<plsize){
					post_block = select[(blockid+1)*l1];
				}else{
					post_block = bits->size();
				}
				if(post_block - current_block>l2){
					for(size_t i=blockid*l1+1; i < (blockid+1)*l1 && i < size; i++){
						sl->push_back(select[i]-current_block, sl_unit_size);
					}
				}else{
					size_t sstart = blockid*l1;
					while(sstart%l3!=0) sstart++;
					for(size_t i=sstart; i < (blockid+1)*l1 && i < size; i+=l3){
						ss->push_back(select[i]-current_block, ss_unit_size);
					}
				}
			}
			bits->pack();
			//std::cerr << "bits.size=" << bits->size() << std::endl;
			pl->pack();
			sl->pack();
			ss->pack();

			delete [] select;
		}
		fib_array(FILE *fp){
			fread(&plsize, sizeof(size_t), 1, fp);
			fread(&pl_unit_size, sizeof(size_t), 1, fp);
			fread(&sl_unit_size, sizeof(size_t), 1, fp);
			fread(&ss_unit_size, sizeof(size_t), 1, fp);
			fread(&l1, sizeof(uint64_t), 1, fp);
			fread(&l2, sizeof(uint64_t), 1, fp);
			fread(&l3, sizeof(uint64_t), 1, fp);
			fc = new bitarray(fp);
			bits = new bitarray(fp);
			pl = new bitarray(fp);
			sl = new bitarray(fp);
			ss = new bitarray(fp);
		}

		T operator[](size_t i){
			size_t start = select(i);
			size_t end = select(i+1)-1;
			size_t size=end-start+1;
			
			uint64_t fibcode=bits->get(start,size);
			return fc->decode(fibcode, size);
		}
		
		~fib_array(){
			delete fc;
			delete bits;
			delete pl;
			delete sl;
			delete ss;
		}

		void dump(FILE *fp){
			fwrite(&plsize, sizeof(size_t), 1, fp);
			fwrite(&pl_unit_size, sizeof(size_t), 1, fp);
			fwrite(&sl_unit_size, sizeof(size_t), 1, fp);
			fwrite(&ss_unit_size, sizeof(size_t), 1, fp);
			fwrite(&l1, sizeof(uint64_t), 1, fp);
			fwrite(&l2, sizeof(uint64_t), 1, fp);
			fwrite(&l3, sizeof(uint64_t), 1, fp);

			fc->dump(fp);
			bits->dump(fp);
			pl->dump(fp);
			sl->dump(fp);
			ss->dump(fp);
		}

	private:
		void init(size_t bitssize){
			bits = new bitarray(bitssize);

      l3 = ceil(log2(bitssize+1));
      l1 = l3*l3;
      l2 = l1*l1;
		}

		size_t count_bitssize(T *data, size_t size){
			size_t bitssize=0;
			for(size_t i = 0; i < size; i++){
				bitssize+=fc->get_bitsize(data[i]);
			}
			return bitssize;
		}

		size_t select(size_t i){
			size_t blockid = i/l1;
			uint64_t pl_current = pl->get(blockid*pl_unit_size, pl_unit_size);
			uint64_t pl_post;
			if(blockid+1<plsize){
				pl_post = pl->get((blockid+1)*pl_unit_size, pl_unit_size);
			}else{
				pl_post = bits->size();
			}
			
			if(pl_post-pl_current>l2){
				return pl_current+sl->get(i*sl_unit_size,sl_unit_size);
			}else{
				size_t ss_pos=i/l3;
				uint64_t sstart = pl_current + ss->get(ss_pos*ss_unit_size, ss_unit_size);
				uint64_t current = ss_pos*l3;
				
				if(current==i){
					return sstart;
				}
				for(size_t j=sstart; j < bits->size(); j++){
					uint64_t code = bits->get(j,2);
					if(code==0x3ULL){
						current++;
						if(current==i){
							return j+2;
						}
						j++;
					}
				}
				return (size_t)-1;
			}
		}

		bitarray *bits;

		size_t plsize;

		size_t pl_unit_size;
		size_t sl_unit_size;
		size_t ss_unit_size;

    uint64_t l1;
    uint64_t l2;
    uint64_t l3;

    bitarray *pl;
    bitarray *sl;
    bitarray *ss;

		fib_coder<T> *fc;
	};
}

#endif // YUNOMI_FIB_ARRAY_H_

