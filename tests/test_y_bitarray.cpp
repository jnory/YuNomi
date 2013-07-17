#include<iostream>
#include<math.h>
#include<y_bitarray.h>

int main(int argc, char **argv){
	char *filename=NULL;
	if(argc==2){
		filename = argv[1];
	}

	std::cerr << "Start building yunomi::bitarray" << std::endl;
	yunomi::bitarray vec(1000);
	for(size_t i = 1; i < 1000; i++){
		size_t bits = (size_t) ceil(log2(i+1));
		vec.push_back(i, bits);
	}
	std::cerr << "Packing..." << std::endl;
	vec.pack();

	std::cerr << "Built bits:" << std::endl;
	vec.inspect();

	std::cerr << "Verifying..." << std::endl;
	size_t tail=0;
	for(size_t i = 1; i < 1000; i++){
		size_t bits = (size_t) ceil(log2(i+1));
		uint64_t result = vec.get(tail,bits);
		if(result!=i){
			std::cerr << "ERROR: value does not match! original=" << i << " return=" << result << std::endl;
		}
		tail+=bits;
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
