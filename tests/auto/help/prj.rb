
require 'mxx_ru/cpp'

Mxx_ru::Cpp::exe_target {
  
	define( 'ARGS_TESTING' )
    
	target( "tests/test.help" )

	cpp_source( "main.cpp" )
}
