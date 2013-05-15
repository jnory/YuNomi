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

	test_bitarray = bld.program(source='tests/test_y_bitarray.cpp', target='test_y_bitarray')
	test_bitarray.includes=['include']

	compress_by_fibarray = bld.program(source='tests/compress_by_fibarray.cpp', target='compress_by_fibarray')
	compress_by_fibarray.includes=['include']
	
	bld.install_files('${PREFIX}/include', ['include/fib_array.h', 'include/fib_coder.h', 'include/y_bitarray.h', 'include/y_defines.h'])
