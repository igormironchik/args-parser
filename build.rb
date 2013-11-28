require 'mxx_ru/cpp'

MxxRu::Cpp::composite_target( Mxx_ru::BUILD_ROOT ) {

	global_include_path "."

	required_prj( "test/prj.rb" )
	required_prj( "samples/prj.rb" )
}
