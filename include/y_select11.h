#ifndef YUNOMI_SELECT11_H_
#define YUNOMI_SELECT11_H_

#include "y_defines.h"
#include "y_bitarray.h"

namespace yunomi {
	class select11{
	public:
		select11(bitarray *bits) : bits(bits){
			size_t size;
			prepare_selectdic(size);
			
			size_t current=0;
			size_t current_block=0;
			size_t pre_block=0;
			uint64_t *select=new uint64_t[l1];
			size_t sl_counter=0;
			size_t ss_counter=0;

			p->push_back(0, p_unit_size);
			for(size_t i = 0; i < size;){
				size_t blockid=i/l1;
				size_t pre_blockid=blockid-1;

				pre_block=current_block;
				current_block = current;
				
				pl->push_back(current, pl_unit_size);
				if(current_block-pre_block>l2){
					for(size_t j=pre_blockid*l1; blockid>0 && j<i; j++){
						uint64_t diff = select[j-pre_blockid*l1];
						if(j%l1!=0){
							sl->push_back(diff, sl_unit_size);
							sl_counter++;
						}
					}
					if(blockid!=0) p->push_back(sl_counter, p_unit_size);
				}else{
					for(size_t j=pre_blockid*l1; blockid>0 && j<i; j++){
						uint64_t blockpos = j-pre_blockid*l1;
						if(blockpos!=0 && blockpos%l3==0){
							uint64_t diff = select[j-pre_blockid*l1];
							ss->push_back(diff, ss_unit_size);
							ss_counter++;
						}
					}
					if(blockid!=0) p->push_back(ss_counter, p_unit_size);
				}

				for(; i < (blockid+1)*l1 && i < size; i++){
					select[i-blockid*l1] = current-current_block;
					current = bits->next11(current);
				}
			}
			size_t pre_blockid=(size-1)/l1;

			pre_block=current_block;
			current_block = current;
			if(current_block-pre_block>l2){
				p->push_back(sl_counter, p_unit_size);
				for(size_t j=pre_blockid*l1; pre_blockid>=0 && j<size; j++){
					uint64_t diff = select[j-pre_blockid*l1];
					if(j%l1!=0){
						sl->push_back(diff, sl_unit_size);
						sl_counter++;
					}
				}
			}else{
				p->push_back(ss_counter, p_unit_size);
				for(size_t j=pre_blockid*l1; pre_blockid>=0 && j<size; j++){
					uint64_t blockpos = j-pre_blockid*l1;
					if(blockpos!=0 && blockpos%l3==0){
						uint64_t diff = select[j-pre_blockid*l1];
						ss->push_back(diff, ss_unit_size);
						ss_counter++;
					}
				}
			}

			delete [] select;

			bits->pack();
			pl->pack();
			sl->pack();
			ss->pack();
			p->pack();
		}
		
		select11(FILE *fp, bitarray *bits) : bits(bits){
			fread(&plsize, sizeof(size_t), 1, fp);
			fread(&pl_unit_size, sizeof(size_t), 1, fp);
			fread(&sl_unit_size, sizeof(size_t), 1, fp);
			fread(&ss_unit_size, sizeof(size_t), 1, fp);
			fread(&p_unit_size, sizeof(size_t), 1, fp);
			fread(&l1, sizeof(uint64_t), 1, fp);
			fread(&l2, sizeof(uint64_t), 1, fp);
			fread(&l3, sizeof(uint64_t), 1, fp);
			
			pl = new bitarray(fp);
			sl = new bitarray(fp);
			ss = new bitarray(fp);
			p = new bitarray(fp);
		}
		
		virtual ~select11(){
			delete pl;
			delete sl;
			delete ss;
			delete p;
		}
		
		void dump(FILE *fp){
			fwrite(&plsize, sizeof(size_t), 1, fp);
			fwrite(&pl_unit_size, sizeof(size_t), 1, fp);
			fwrite(&sl_unit_size, sizeof(size_t), 1, fp);
			fwrite(&ss_unit_size, sizeof(size_t), 1, fp);
			fwrite(&p_unit_size, sizeof(size_t), 1, fp);
			fwrite(&l1, sizeof(uint64_t), 1, fp);
			fwrite(&l2, sizeof(uint64_t), 1, fp);
			fwrite(&l3, sizeof(uint64_t), 1, fp);

			pl->dump(fp);
			sl->dump(fp);
			ss->dump(fp);
			p->dump(fp);
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
					size_t sl_idx = p->get(blockid*p_unit_size, p_unit_size);
					return pl_current+sl->get((sl_idx+(i-blockid*l1-1))*sl_unit_size,sl_unit_size);
				}
			}else{
				uint64_t blockpos = i-blockid*l1;
				uint64_t sstart = pl_current;
				uint64_t smallblockid = blockpos/l3;
				
				if(blockpos >= l3){
					size_t ss_idx = p->get(blockid*p_unit_size, p_unit_size);
					sstart += ss->get((ss_idx+smallblockid-1)*ss_unit_size, ss_unit_size);
				}
				uint64_t current = blockid*l1+smallblockid*l3;
#ifdef YUNOMI_DEBUG
				std::cerr << "i=" << i << " blockid=" << blockid << " blockpos=" << blockpos << " pl_current=" << pl_current << " sstart=" << sstart << " current=" << current << std::endl;
#endif
				
				for(; current < i; current++){
					sstart = bits->next11(sstart);
				}

				return sstart;
			}
		}

	private:
		void set_ls(){
			l3 = (uint64_t) (ceil(log2(bits->size()+1))*L3_WEIGHT);
      l1 = (uint64_t) (l3*l3*L1_WEIGHT);
      l2 = (uint64_t) (l1*l1*L2_WEIGHT);
      
#ifdef YUNOMI_DEBUG
      std::cerr << "l1=" << l1 << " l2=" << l2 << " l3=" << l3 << std::endl;
#endif
		}
		
		size_t count_elements(size_t &maxdiff){
			maxdiff=0;
			size_t count=0;
			size_t pre_pos=0;
			while(true){
				size_t pos=bits->next11(pre_pos);
				
				if(pos==(size_t)-1) break;
				
				if(pos-pre_pos > maxdiff){
					maxdiff = pos-pre_pos;
				}
				pre_pos = pos;
				count++;
				if(pos>=bits->size()) break;
			}
			
			return count;
		}
		
		void prepare_selectdic(size_t &count){
			set_ls();

			size_t maxdiff;
			count = count_elements(maxdiff);
			plsize = (count+l1-1)/l1;
			
			pl_unit_size = (size_t) ceil(log2(bits->size()));
			pl = new bitarray(plsize*pl_unit_size);

			//配列数count個は怪しいので要確認
			sl_unit_size = (size_t) ceil(log2(maxdiff));
			sl = new bitarray(sl_unit_size*count);

			ss_unit_size = (size_t) ceil(log2(l2));
			size_t sssize = count/l3;
			ss = new bitarray(ss_unit_size*sssize);

			size_t p_max = (count>sssize)?count:sssize;
			p_unit_size = (size_t) ceil(log2(p_max));
			p = new bitarray(p_max*p_unit_size);
		}
		
		bitarray *bits;
		
		size_t plsize;

		size_t pl_unit_size;
		size_t sl_unit_size;
		size_t ss_unit_size;
		size_t p_unit_size;

    uint64_t l1;
    uint64_t l2;
    uint64_t l3;

    bitarray *pl;
    bitarray *sl;
    bitarray *ss;
		bitarray *p;
	};
}

#endif //YUNOMI_SELECT11_H_
