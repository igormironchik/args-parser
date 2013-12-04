require 'mxx_ru/cpp'

require 'UnitTest/version'

MxxRu::Cpp::lib_target {

	target( "UnitTest." + UnitTest::VERSION )
	target_root( "lib" )

	cpp_source( "unit_test.cpp" )
}
