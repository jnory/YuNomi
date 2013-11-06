#ifndef YUNOMI_BITARRAY_H_
#define YUNOMI_BITARRAY_H_

#include<cstdlib>
#include"y_defines.h"

namespace yunomi{
	class bitarray{
	public:
		bitarray(size_t bitsize):bitsize(bitsize), tail(0){
			size_t bitssize = (bitsize+UINT64_T_SIZE)>>UINT64_T_SIZE_BITS;
			bitssize++;
			bits = new uint64_t[bitssize];
			memset(bits, 0, sizeof(uint64_t)*bitssize);
		}

		bitarray(FILE *fp){
			fread(&bitsize, sizeof(size_t), 1, fp);
			fread(&tail, sizeof(size_t), 1, fp);
			size_t bitssize = (bitsize+UINT64_T_SIZE)>>UINT64_T_SIZE_BITS;
			bits = new uint64_t[bitssize];
			fread(bits, sizeof(uint64_t), bitssize, fp);
		}

		virtual ~bitarray(){
			delete [] bits;
		}

		uint64_t get(size_t position, size_t size){
			size_t start_index = position>>UINT64_T_SIZE_BITS;
			size_t start_bit = position&UINT64_T_SIZE_MASK;
			size_t end_index = (position+size-1)>>UINT64_T_SIZE_BITS;
			size_t end_bit = (position+size-1)&UINT64_T_SIZE_MASK;
			
			uint64_t ret=0;
			ret = bits[start_index]>>start_bit;
			if(start_index==end_index){
				ret &= (0x1ULL<<(size-1))|((0x1ULL<<(size-1))-1);
			}else{
				ret |= (bits[end_index]&((0x1ULL<<(end_bit+1))-1))<<(UINT64_T_SIZE-start_bit);
			}

			return ret;
		}
		
		uint64_t getbit(size_t position){
			size_t pos = position>>UINT64_T_SIZE_BITS;
			size_t bit = position& UINT64_T_SIZE_MASK;
			return ((bits[pos]>>bit)&0x1ULL);
		}

		void push_back(uint64_t data, size_t size, bool flag=false){
			if(size==0) return;
			while(tail+size>bitsize){
				expand();
			}

			size_t start_index = tail>>UINT64_T_SIZE_BITS;
			size_t start_bit = tail&UINT64_T_SIZE_MASK;
			size_t end_index = (tail+size-1)>>UINT64_T_SIZE_BITS;

			bits[start_index] |= data<<start_bit;
			if(start_index!=end_index){
				bits[end_index] |= data>>(UINT64_T_SIZE-start_bit);
			}
			tail+=size;
		}

		void pack(){
			if(tail < bitsize){
				size_t new_bitsize = tail;
				size_t new_bitssize = (new_bitsize+UINT64_T_SIZE)>>UINT64_T_SIZE_BITS;
				uint64_t *new_bits = new uint64_t[new_bitssize];
				memcpy(new_bits, bits, sizeof(uint64_t)*new_bitssize);
				
				delete [] bits;
				bits = new_bits;
				bitsize = new_bitsize;
			}
		}
		
		size_t next11(size_t begin){
			size_t bitsize = 64;
			if(begin>=size()) return -1;
			if(begin+63>=size()) bitsize = size()-begin+1;
			
			uint64_t bs = get(begin,bitsize);

#ifdef YUNOMI_DEBUG
			std::cerr << "ORIGINAL:" << std::hex << bs << std::dec << std::endl;
#endif

			// detect the most right "11".
			// see a book "Hacker's Delight" by Henry S. Warren, Jr. Section 2-1.
			bs = bs&(bs>>1);
			bs = (bs|(bs-1))-bs;

#ifdef YUNOMI_DEBUG
			std::cerr << "MOST-RIGHT:" << std::hex << bs << std::dec << std::endl;
#endif

			if(bs==0xFFFFFFFFFFFFFFFFULL){
				return -1;
			}else{
				// popcount
				// see a paper "Broadword Implementation of Rank/Select Queries" by Sebastiano Vigna.
				bs = bs - ((bs&0xAAAAAAAAAAAAAAAAULL)>>1);
				bs = (bs&0x3333333333333333ULL) + ((bs >> 2) & 0x3333333333333333ULL);
				bs = (bs + (bs >> 4))&0x0F0F0F0F0F0F0F0FULL;
				bs = (bs*0x0101010101010101ULL) >> 56;

#ifdef YUNOMI_DEBUG
				std::cerr << "COUNT:" << std::hex << bs << std::dec << std::endl;
#endif
				return begin+bs+2;
			}
		}
		
		void inspect(){
			for(size_t i=0; i < bitsize; i++){
				size_t pos = i>>UINT64_T_SIZE_BITS;
				size_t bit = i& UINT64_T_SIZE_MASK;
				std::cerr << ((bits[pos]>>bit)&0x1ULL);
				if((i+1)%4 == 0) std::cerr << " ";
				if((i+1)%32 == 0) std::cerr << std::endl;
			}
			std::cerr << std::endl;
		}

		void dump(FILE *fp){
			fwrite(&bitsize, sizeof(size_t), 1, fp);
			fwrite(&tail, sizeof(size_t), 1, fp);
			size_t bitssize = (bitsize+UINT64_T_SIZE)>>UINT64_T_SIZE_BITS;
			fwrite(bits, sizeof(uint64_t), bitssize, fp);
		}

		size_t size(){
			return tail;
		}

	private:
		void expand(){
			size_t bitssize = (bitsize+UINT64_T_SIZE)>>UINT64_T_SIZE_BITS;

			size_t new_bitsize = bitsize*2;
			size_t new_bitssize = (new_bitsize+UINT64_T_SIZE)>>UINT64_T_SIZE_BITS;
			uint64_t *new_bits = new uint64_t[new_bitssize];
			memcpy(new_bits, bits, sizeof(uint64_t)*bitssize);
			memset(new_bits+bitssize, 0, sizeof(uint64_t)*(new_bitssize-bitssize));

			delete [] bits;
			bits = new_bits;
			bitsize = new_bitsize;
		}

		uint64_t *bits;
		size_t bitsize;
		size_t tail;
	};
}

#endif //YUNOMI_BITARRAY_H_
