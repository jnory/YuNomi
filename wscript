APPNAME='YuNomi'
VERSION='0.001'

top='.'
out='.build'

def options(opt):
	opt.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')

def build(bld):
	test_fib_array_orig = bld.program(source='tests/test_fib_array_orig.cpp', target='test_fib_array_orig')
	test_fib_array_orig.includes=['include']
	test_fib_array_orig.cxxflags=['-O3']
	#test_fib_array_orig.cxxflags=['-O3', '-DYUNOMI_DEBUG']

	test_fib_array_rand = bld.program(source='tests/test_fib_array_rand.cpp', target='test_fib_array_rand')
	test_fib_array_rand.includes=['include']
	test_fib_array_rand.cxxflags=['-O3']

	test_bitarray = bld.program(source='tests/test_y_bitarray.cpp', target='test_y_bitarray')
	test_bitarray.includes=['include']
	test_bitarray.cxxflags=['-O3']

	test_gjc_array = bld.program(source='tests/test_gjc_array.cpp', target='test_gjc_array')
	test_gjc_array.includes=['include']
	test_gjc_array.cxxflags=['-O3']

	compress_by_fibarray = bld.program(source='samples/compress_by_fibarray.cpp', target='compress_by_fibarray')
	compress_by_fibarray.includes=['include']
	compress_by_fibarray.cxxflags=['-O3']
	#compress_by_fibarray.cxxflags=['-g', '-DYUNOMI_DEBUG']

	compress_by_gjcarray = bld.program(source='samples/compress_by_gjcarray.cpp', target='compress_by_gjcarray')
	compress_by_gjcarray.includes=['include']
	compress_by_gjcarray.cxxflags=['-O3']

	read_from_fibarray = bld.program(source='samples/read_from_fibarray.cpp', target='read_from_fibarray')
	read_from_fibarray.includes=['include']
	read_from_fibarray.cxxflags=['-O3']
	#read_from_fibarray.cxxflags=['-g', '-DYUNOMI_DEBUG']
	
	bld.install_files('${PREFIX}/include', ['include/fib_array.h', 'include/fib_coder.h', 'include/y_bitarray.h', 'include/y_defines.h', 'include/array_value_reader.h', 'include/file_value_reader.h', 'include/value_reader.h', 'include/gjc_array.h', 'include/y_select11.h'])

