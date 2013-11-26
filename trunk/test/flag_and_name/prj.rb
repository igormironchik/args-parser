
require 'mxx_ru/cpp'

Mxx_ru::Cpp::exe_target {

    required_prj( "UnitTest/prj.rb" )
    required_prj( "Args/prj.rb" )
    
	target( "test.flag_and_name" )

	cpp_source( "main.cpp" )
}
