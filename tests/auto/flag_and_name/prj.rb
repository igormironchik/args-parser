
require 'mxx_ru/cpp'

Mxx_ru::Cpp::exe_target {
    
	target( "tests/test.flag_and_name" )

	cpp_source( "main.cpp" )
}
