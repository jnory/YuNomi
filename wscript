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

	test_bitarray = bld.program(source='tests/test_y_bitarray.cpp', target='test_y_bitarray')
	test_bitarray.includes=['include']
	test_bitarray.cxxflags=['-O3']

	compress_by_fibarray = bld.program(source='src/compress_by_fibarray.cpp', target='compress_by_fibarray')
	compress_by_fibarray.includes=['include']
	compress_by_fibarray.cxxflags=['-O3']

	read_by_fibarray = bld.program(source='src/read_by_fibarray.cpp', target='read_by_fibarray')
	read_by_fibarray.includes=['include']
	read_by_fibarray.cxxflags=['-O3']
	
	bld.install_files('${PREFIX}/include', ['include/fib_array.h', 'include/fib_coder.h', 'include/y_bitarray.h', 'include/y_defines.h'])
