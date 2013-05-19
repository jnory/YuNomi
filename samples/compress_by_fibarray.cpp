#include<vector>
#include<fstream>
#include<fib_array.h>
#include<file_value_reader.h>

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

	std::cerr << "Building yunomi::fib_array" << std::endl;
	std::string inputfile = input;
	yunomi::file_value_reader<uint32_t> fvr(inputfile);
	yunomi::fib_array<uint32_t> fibarray(fvr);

	std::cerr << "Dumping" << std::endl;
	FILE *fp = std::fopen(output,"wb");
	fibarray.dump(fp);
	std::fclose(fp);

	return 0;
}
