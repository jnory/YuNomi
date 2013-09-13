#include<iostream>
#include<math.h>
#include<y_bitarray.h>
#include<y_select11.h>
#include<y_rank11.h>
#include<fib_coder.h>

int main(int argc, char **argv){
	char *filename=NULL;
	if(argc==2){
		filename = argv[1];
	}

	std::cerr << "Start building yunomi::bitarray" << std::endl;
	yunomi::fib_coder<uint64_t> *fc = new yunomi::fib_coder<uint64_t>();
	yunomi::bitarray vec(1000);
	uint64_t code=0;
	size_t bitsize=0;

	for(size_t i = 1; i < 100; i++){
		fc->encode(i, code, bitsize);
		vec.push_back(code, bitsize);
	}
	std::cerr << "Packing..." << std::endl;
	vec.pack();

	std::cerr << "Built bits:" << std::endl;
	vec.inspect();

	std::cerr << "Verifying..." << std::endl;
	size_t tail=0;
	for(size_t i = 1; i < 100; i++){
		fc->encode(i, code, bitsize);
		uint64_t result = vec.get(tail, bitsize);
		if(result!=code){
			std::cerr << "ERROR: value does not match! original=" << i << " return=" << result << std::endl;
		}
		tail+=bitsize;
	}
	
	std::cerr << "Building rank11..." << std::endl;
	yunomi::rank11 rank(&vec);
	std::cerr << "Verifying rank11..." << std::endl;
	size_t counter = 0;
	for(size_t i = 1; i < vec.size(); i++){
		size_t r = rank.rank(i);
		if(r != counter){
			std::cerr << "ERROR: rank does not match! i = " << i << " rank=" << r << " count=" << counter << std::endl;
		}
		
		if(vec.get(i-1,2)==0x3ULL){
			counter++;
			i++;
			r = rank.rank(i);
			if(r != counter){
				std::cerr << "ERROR: rank does not match! i = " << i << " rank=" << r << " count=" << counter << std::endl;
			}
		}
		
		//std::cerr << "rank11("<<i<<")=" << r << std::endl;
	}
	
	
	std::cerr << "Done!" << std::endl;

	if(filename!=NULL){
		std::cerr << "Dumping..." << std::endl;
		FILE *fp = std::fopen(filename,"wb");
		vec.dump(fp);
		std::fclose(fp);
	}

	return 0;
}
