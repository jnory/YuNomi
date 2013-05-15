#include<vector>
#include<fstream>
#include<fib_array.h>

int main(int argc, char **argv){
	char *input=NULL;
	char *output=NULL;
	if(argc==3){
		input = argv[1];
		output=argv[2];
	}else{
		std::cerr << "Usage: " << argv[0] << " input output" << std::endl;
		exit(1);
	}

	std::cerr << "Start building yunomi::fib_array" << std::endl;
	std::ifstream ifs(input);
	std::vector<uint32_t> vec;

	uint32_t value=0;
	
	ifs >> value;
	while(!ifs.eof()){
		vec.push_back(value);
		ifs >> value;
	}

	yunomi::fib_array<uint32_t> fibarray(&(vec[0]), vec.size());

	std::cerr << "Verifying..." << std::endl;
	for(size_t i = 0; i < vec.size(); i++){
		uint32_t result = fibarray[i];
		if(result!=vec[i]){
			std::cerr << "ERROR: value does not match! i=" << i << " original=" << vec[i] << " fibarray=" << result << std::endl;
			throw "error.";
		}
	}
	std::cerr << "Done!" << std::endl;

	std::cerr << "Dumping..." << std::endl;
	FILE *fp = std::fopen(output,"wb");
	fibarray.dump(fp);
	std::fclose(fp);

	return 0;
}
