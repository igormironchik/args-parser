
require 'mxx_ru/cpp'

Mxx_ru::Cpp::exe_target {
    
	target( "tests/test.exceptions" )

	cpp_source( "main.cpp" )
}
