
require 'mxx_ru/cpp'

Mxx_ru::Cpp::exe_target {
    
	target( "test.arg_as_command" )

	cpp_source( "main.cpp" )
}
