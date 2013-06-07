APPNAME='YuNomi'
VERSION='0.001'

top='.'
out='.build'

def options(opt):
	opt.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')

def build(bld):
	test_fib_array = bld.program(source='tests/test_fib_array.cpp', target='test_fib_array')
	test_fib_array.includes=['include']
	test_fib_array.cxxflags=['-O3']
	#test_fib_array.cxxflags=['-g']

	test_fib_array2 = bld.program(source='tests/test_fib_array2.cpp', target='test_fib_array2')
	test_fib_array2.includes=['include']
	test_fib_array2.cxxflags=['-O3']

	test_bitarray = bld.program(source='tests/test_y_bitarray.cpp', target='test_y_bitarray')
	test_bitarray.includes=['include']
	test_bitarray.cxxflags=['-O3']

	test_fib_array = bld.program(source='tests/test_gjc_array.cpp', target='test_gjc_array')
	test_fib_array.includes=['include']
	test_fib_array.cxxflags=['-O3']

	compress_by_fibarray = bld.program(source='samples/compress_by_fibarray.cpp', target='compress_by_fibarray')
	compress_by_fibarray.includes=['include']
	compress_by_fibarray.cxxflags=['-O3']
	#compress_by_fibarray.cxxflags=['-g']

	compress_by_gjcarray = bld.program(source='samples/compress_by_gjcarray.cpp', target='compress_by_gjcarray')
	compress_by_gjcarray.includes=['include']
	compress_by_gjcarray.cxxflags=['-O3']

	read_from_fibarray = bld.program(source='samples/read_from_fibarray.cpp', target='read_from_fibarray')
	read_from_fibarray.includes=['include']
	read_from_fibarray.cxxflags=['-O3']
	#read_from_fibarray.cxxflags=['-g', '-DYUNOMI_DEBUG']
	
	bld.install_files('${PREFIX}/include', ['include/fib_array.h', 'include/fib_coder.h', 'include/y_bitarray.h', 'include/y_defines.h', 'include/array_value_reader.h', 'include/file_value_reader.h', 'include/value_reader.h', 'include/gjc_array.h'])
