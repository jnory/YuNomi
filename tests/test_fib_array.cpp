#include<vector>
#include<fib_array.h>

int main(int argc, char **argv){
	char *filename=NULL;
	if(argc==2){
		filename = argv[1];
	}

	std::cerr << "Start building yunomi::fib_array" << std::endl;
	std::vector<uint32_t> vec;
	for(size_t i = 0; i < 5000000; i++){
		vec.push_back(i+1);
	}

	yunomi::fib_array<uint32_t> fibarray(&(vec[0]), vec.size());

	std::cerr << "Verifying..." << std::endl;
	for(size_t i = 0; i < 5000000; i++){
		uint32_t result = fibarray[i];
		if(result!=vec[i]){
			std::cerr << "ERROR: value does not match! i=" << i << " original=" << vec[i] << " fibarray=" << result << std::endl;
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
