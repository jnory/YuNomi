#include<vector>
#include<gjc_array.h>
#include<array_value_reader.h>

#define TESTSIZE 5000000
int main(int argc, char **argv){
	char *filename=NULL;
	if(argc==2){
		filename = argv[1];
	}

	std::cerr << "Start building yunomi::gjc_array" << std::endl;
	std::vector<int> vec;
	for(size_t i = 0; i < TESTSIZE; i++){
		vec.push_back(rand()-RAND_MAX/2);
	}

	yunomi::array_value_reader<int> avr(vec);
	yunomi::gjc_array<int> array(avr);

	std::cerr << "Verifying..." << std::endl;
	for(size_t i = 0; i < TESTSIZE; i++){
		uint32_t result = array[i];

		if(result!=vec[i]){
			std::cerr << "ERROR: value does not match! i=" << i;
			std::cerr << " original=" << vec[i];
			std::cerr << " array=" << result;
			std::cerr << std::endl;
			throw "error.";
		}
	}
	std::cerr << "Done!" << std::endl;

	if(filename!=NULL){
		std::cerr << "Dumping..." << std::endl;
		FILE *fp = std::fopen(filename,"wb");
		array.dump(fp);
		std::fclose(fp);
	}

	return 0;
}
