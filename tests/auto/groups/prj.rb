
require 'mxx_ru/cpp'

Mxx_ru::Cpp::exe_target {
    
	target( "tests/test.groups" )

	cpp_source( "main.cpp" )
}
