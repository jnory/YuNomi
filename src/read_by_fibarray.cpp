#include<vector>
#include<fstream>
#include<fib_array.h>

int main(int argc, char **argv){
	char *input_bin=NULL;
	char *input_text=NULL;
	if(argc==3){
		input_bin = argv[1];
		input_text= argv[2];
	}else{
		std::cerr << "Usage: " << argv[0] << " bin text" << std::endl;
		exit(1);
	}

	std::cerr << "Start building yunomi::fib_array" << std::endl;
	std::ifstream ifs(input_text);
	std::vector<uint32_t> vec;

	uint32_t value=0;
	
	ifs >> value;
	while(!ifs.eof()){
		vec.push_back(value);
		ifs >> value;
	}

	FILE *fp = std::fopen(input_bin,"rb");
	yunomi::fib_array<uint32_t> fibarray(fp);
	std::fclose(fp);

	std::cerr << "Verifying...";
	for(size_t i = 0; i < vec.size(); i++){
		uint32_t result = fibarray[i];
		if(result!=vec[i]){
			std::cerr << std::endl;
			std::cerr << "ERROR: value does not match! i=" << i << " original=" << vec[i] << " fibarray=" << result << std::endl;
			throw "error.";
		}
	}
	std::cerr << "Done!" << std::endl;

	return 0;
}
