#ifndef YUNOMI_RANK11_H_
#define YUNOMI_RANK11_H_

#include "y_defines.h"
#include "y_bitarray.h"

namespace yunomi {
	class rank11{
	public:
		rank11(bitarray *bits) : bits(bits){
			size_t size;
			prepare_rankdic(size);
			
			pl->push_back(0, pl_unit_size);
			sl->push_back(0, sl_unit_size);
			sf->push_back(0, 1);
			
			size_t pre_pos=(size_t)-2;
			size_t pre_large_block_id = 0;
			size_t pre_small_block_id = 0;
			
			size_t pl_insert_num = 1;
			size_t sl_insert_num = 1;
			
			size_t counter=0;
			
			while(true){
				size_t pos=bits->next11(pre_pos+2);
				
				if(pos==(size_t)-1) break;
				pos-=2;
				size_t large_block_id = pos / l1;
				if(large_block_id != pre_large_block_id){
					while(pl_insert_num < large_block_id){
						pl->push_back(counter, pl_unit_size);
						pl_insert_num++;
					}
					pl->push_back(counter, pl_unit_size);
					pl_insert_num++;
				}
				
				size_t small_block_id = pos / l2;
				if(small_block_id != pre_small_block_id){
					while(sl_insert_num < small_block_id){
						size_t lidx = sl_insert_num*l2/l1;
						sl->push_back(counter-pl->get(lidx*pl_unit_size, pl_unit_size), sl_unit_size);
						if(pre_pos+1==sl_insert_num*l2){
							sf->push_back(1, 1);
						}else{
							sf->push_back(0, 1);
						}
						sl_insert_num++;
					}
					size_t lidx = sl_insert_num*l2/l1;
					sl->push_back(counter-pl->get(lidx*pl_unit_size, pl_unit_size), sl_unit_size);
					if(pre_pos+1==sl_insert_num*l2){
						sf->push_back(1, 1);
					}else{
						sf->push_back(0, 1);
					}
					sl_insert_num++;
				}
				
				pre_pos = pos;
				pre_large_block_id = large_block_id;
				pre_small_block_id = small_block_id;
				counter++;
				if(pos>=bits->size()) break;
			}
			
			for(size_t i = 0; i < psize; i++){
				uint64_t bs = i;
				size_t count = 0;
				while(bs != 0){
					switch(bs&0x3ULL){
					case 0x0ULL:
						bs = bs >> 2;
						break;
					case 0x1ULL:
						bs = bs >> 2;
						break;
					case 0x2ULL:
						bs = bs >> 1;
						break;
					case 0x3ULL:
						count++;
						bs = bs >> 2;
						break;
					}
				}
				p[i] = count;
			}
			
			bits->pack();
			pl->pack();
			sl->pack();
			sf->pack();
		}
		
		
		rank11(FILE *fp, bitarray *bits) : bits(bits){
			fread(&plsize, sizeof(size_t), 1, fp);
			fread(&slsize, sizeof(size_t), 1, fp);
			fread(&psize, sizeof(size_t), 1, fp);

			fread(&pl_unit_size, sizeof(size_t), 1, fp);
			fread(&sl_unit_size, sizeof(size_t), 1, fp);
			
			fread(&l1, sizeof(uint64_t), 1, fp);
			fread(&l2, sizeof(uint64_t), 1, fp);
			
			pl = new bitarray(fp);
			sl = new bitarray(fp);
			sf = new bitarray(fp);
			p = new uint32_t[psize];
			fread(p, sizeof(uint32_t), psize, fp);
		}
		
		virtual ~rank11(){
			delete pl;
			delete sl;
			delete sf;
			delete [] p;
		}
		
		void dump(FILE *fp){
			fwrite(&plsize, sizeof(size_t), 1, fp);
			fwrite(&slsize, sizeof(size_t), 1, fp);
			fwrite(&psize, sizeof(size_t), 1, fp);

			fwrite(&pl_unit_size, sizeof(size_t), 1, fp);
			fwrite(&sl_unit_size, sizeof(size_t), 1, fp);
			
			fwrite(&l1, sizeof(uint64_t), 1, fp);
			fwrite(&l2, sizeof(uint64_t), 1, fp);

			pl->dump(fp);
			sl->dump(fp);
			sf->dump(fp);
			fwrite(p, sizeof(uint32_t), psize, fp);
		}
		
		size_t rank(size_t i){
			i--;
			size_t large_block_id = i / l1;
			size_t small_block_id = i / l2;
			size_t small_block_pos = i % l2;

			size_t r = pl->get(large_block_id*pl_unit_size, pl_unit_size)
							   + sl->get(small_block_id*sl_unit_size, sl_unit_size);
			
			if(small_block_pos!=0){
				uint64_t b = bits->get(small_block_id*l2, small_block_pos+1);
				uint64_t flg = ~sf->getbit(small_block_id);
				r += p[b&flg];
			}
			return r;
		}

	private:
		void set_ls(){
			size_t l3 = (uint64_t) (ceil(log2(bits->size()+1))*L3_WEIGHT);
      l1 = (uint64_t) (l3*l3*L1_WEIGHT);
      l2 = (uint64_t) (l1/2*L2_WEIGHT);
      if(l2 > 32) l2 = 32;
      
#ifdef YUNOMI_DEBUG
      std::cerr << "l1=" << l1 << " l2=" << l2 << std::endl;
#endif
		}
		
		size_t count_elements(size_t &maxdiff){
			maxdiff=0;
			size_t count=0;
			size_t pre_pos=0;
			while(true){
				size_t pos=bits->next11(pre_pos);
				
				if(pos==(size_t)-1) break;
				
				size_t large_block_pos = pos % l1;
				
				if(large_block_pos > maxdiff){
					maxdiff = large_block_pos;
				}
				pre_pos = pos;
				count++;
				if(pos>=bits->size()) break;
			}
			
			return count;
		}
		
		void prepare_rankdic(size_t &count){
			set_ls();

			size_t maxdiff;
			count = count_elements(maxdiff);
			plsize = (bits->size()+l1-1)/l1;
			slsize = (bits->size()+l2-1)/l2;
			
			pl_unit_size = (size_t) ceil(log2(count));
			pl = new bitarray(plsize*pl_unit_size);

			sl_unit_size = (size_t) ceil(log2(maxdiff));
			sl = new bitarray(slsize*sl_unit_size);

			psize = (size_t) pow(2, l2);
			p = new uint32_t[psize];
			
			sf = new bitarray(slsize);
		}
		
		bitarray *bits;
		
		size_t plsize;
		size_t slsize;
		size_t psize;

		size_t pl_unit_size;
		size_t sl_unit_size;

    uint64_t l1;
    uint64_t l2;

    bitarray *pl;
    bitarray *sl;
		bitarray *sf;
		
		uint32_t *p;
	};
}

#endif //YUNOMI_RANK11_H_
