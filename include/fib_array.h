#ifndef YUNOMI_FIB_ARRAY_H_
#define YUNOMI_FIB_ARRAY_H_

#include<iostream>
#include<math.h>
#include"y_bitarray.h"
#include"fib_coder.h"
#include"value_reader.h"

namespace yunomi {
	template <class T> class fib_array{
	public:
		fib_array(value_reader<T> &vr){
			fc = new fib_coder<T>();

			size_t size = vr.size();
			size_t bitssize = count_bitssize(vr); vr.move_head();
			size_t maxdiff=get_maxdiff(vr); vr.move_head();

			bits = new bitarray(bitssize);

			prepare_selectdic(size, bitssize, maxdiff);

			size_t current=0;
			size_t current_block=0;
			size_t pre_block=0;
			uint64_t code=0;
			size_t bitsize=0;
			uint64_t *select = new uint64_t[l1];

			for(size_t i = 0; i < size;){
				size_t blockid=i/l1;
				size_t pre_blockid=blockid-1;

				pre_block=current_block;
				current_block = current;

				pl->push_back(current, pl_unit_size);

				for(size_t j=pre_blockid*l1; blockid>0 && j<i; j++){
					uint64_t diff = select[j-pre_blockid*l1];
					if(current_block-pre_block>l2){
						sl->push_back(diff, sl_unit_size);
						if(j%l3==0) ss->push_back(0, ss_unit_size);
					}else{
						sl->push_back(0, sl_unit_size);
						if(j%l3==0) ss->push_back(diff, ss_unit_size);
					}
				}

				for(; i < (blockid+1)*l1 && i < size; i++){
					fc->encode(vr.pop_front(), code, bitsize);
					bits->push_back(code, bitsize);
					select[i-blockid*l1] = current-current_block;

					current += bitsize;
				}
			}
			size_t blockid=(size-1)/l1;
			size_t pre_blockid=blockid-1;

			pre_block=current_block;
			current_block = current;
			for(size_t j=pre_blockid*l1; blockid>0 && j<size; j++){
				uint64_t diff = select[j-pre_blockid*l1];
				if(current_block-pre_block>l2){
					sl->push_back(diff, sl_unit_size);
					if(j%l3==0) ss->push_back(0, ss_unit_size);
				}else{
					sl->push_back(0, sl_unit_size);
					if(j%l3==0) ss->push_back(diff, ss_unit_size);
				}
			}

			delete [] select;

			bits->pack();
			pl->pack();
			sl->pack();
			ss->pack();
		}

		fib_array(FILE *fp){
			fread(&plsize, sizeof(size_t), 1, fp);
			fread(&pl_unit_size, sizeof(size_t), 1, fp);
			fread(&sl_unit_size, sizeof(size_t), 1, fp);
			fread(&ss_unit_size, sizeof(size_t), 1, fp);
			fread(&l1, sizeof(uint64_t), 1, fp);
			fread(&l2, sizeof(uint64_t), 1, fp);
			fread(&l3, sizeof(uint64_t), 1, fp);

			fc = new fib_coder<T>(fp);
			bits = new bitarray(fp);
			pl = new bitarray(fp);
			sl = new bitarray(fp);
			ss = new bitarray(fp);
		}

		T operator[](size_t i){
			size_t start = select(i);
			size_t end = select(i+1)-1;

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
		size_t get_maxdiff(value_reader<T> &vr){
			size_t size = vr.size();
			size_t current=0;
			size_t block_head=0;
			uint64_t maxdiff=0;
			for(size_t i = 0; i < size; i++){
				size_t bitsize=fc->get_bitsize(vr.pop_front());
				if(i%l1==0){
					block_head=current;
				}else{
					if(current-block_head>maxdiff){
						maxdiff = current-block_head;
					}
				}

				current+=bitsize;
			}

			return maxdiff;
		}

		size_t count_bitssize(value_reader<T> &vr){
			size_t size = vr.size();
			size_t bitssize=0;
			for(size_t i = 0; i < size; i++){
				bitssize+=fc->get_bitsize(vr.pop_front());
			}

			return bitssize;
		}

		void prepare_selectdic(size_t vrsize, size_t bitssize, size_t maxdiff){
      l3 = ceil(log2(bitssize+1));
      l1 = l3*l3;
      l2 = l1*l1;

			plsize = (vrsize+l1-1)/l1;
			pl_unit_size = ceil(log2(bitssize));

			pl = new bitarray(plsize*pl_unit_size);

			sl_unit_size=ceil(log2(maxdiff));
			sl = new bitarray(sl_unit_size*vrsize);

			ss_unit_size=ceil(log2(l2));
			size_t sssize = vrsize/l3;
			ss = new bitarray(ss_unit_size*sssize);
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
				if(i%l1==0){
					return pl_current;
				}else{
					return pl_current+sl->get(i*sl_unit_size,sl_unit_size);
				}
			}else{
				size_t ss_pos=i/l3;
				uint64_t sstart;
				if(i%(l3*l1)==0){
					sstart = pl_current;
				}else{
					sstart = pl_current + ss->get(ss_pos*ss_unit_size, ss_unit_size);
				}
				uint64_t current = ss_pos*l3;
#ifdef YUNOMI_DEBUG
				std::cerr << "ss_pos=" << ss_pos << " pl_current=" << pl_current << " sstart=" << sstart << " current=" << current << std::endl;
#endif
				
				if(current==i){
					return sstart;
				}
				for(size_t j=sstart; j < bits->size()-1; j++){
					switch(bits->get(j,2)){
					case 0x0ULL:
					case 0x1ULL:
						j++;
						break;
					case 0x3ULL:
						current++;
						if(current==i) return j+2;
						j++;
						break;
					default:
						break;
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

