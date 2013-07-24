#include<vector>
#include<fib_array.h>
#include<array_value_reader.h>

#define TESTSIZE 5000000
int main(int argc, char **argv){
	char *filename=NULL;
	if(argc==2){
		filename = argv[1];
	}

	std::cerr << "Start building yunomi::fib_array" << std::endl;
	std::vector<uint32_t> vec;
	for(size_t i = 0; i < TESTSIZE; i++){
		vec.push_back(rand()+1);
	}

	std::cerr << "Constructor Type1 : std::vector" << std::endl;
	yunomi::array_value_reader<uint32_t> avr(vec);
	yunomi::fib_array<uint32_t> fibarray(avr);

	std::cerr << "Constructor Type2 : value array" << std::endl;
	yunomi::array_value_reader<uint32_t> avr2(&(vec[0]), vec.size());
	yunomi::fib_array<uint32_t> fibarray2(avr2);

	std::cerr << "Verifying..." << std::endl;
	for(size_t i = 0; i < TESTSIZE; i++){
		uint32_t result = fibarray[i];
		uint32_t result2 = fibarray2[i];

		if(result!=vec[i] || result2!=vec[i]){
			std::cerr << "ERROR: value does not match! i=" << i;
			std::cerr << " original=" << vec[i];
			std::cerr << " fibarray=" << result;
			std::cerr << " fibarray2=" << result2;
			std::cerr << std::endl;
			throw "error.";
		}
	}
	std::cerr << "Done!" << std::endl;

	if(filename!=NULL){
		std::cerr << "Dumping..." << std::endl;
		FILE *fp = std::fopen(filename,"wb");
		fibarray.dump(fp);
		std::fclose(fp);
	}

	return 0;
}
