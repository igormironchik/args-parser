
require 'mxx_ru/cpp'

Mxx_ru::Cpp::exe_target {

    required_prj( "Args/prj.rb" )
    
	target( "sample.help" )

	cpp_source( "main.cpp" )
}
