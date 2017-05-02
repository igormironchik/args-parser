require 'mxx_ru/cpp'

MxxRu::Cpp::composite_target( Mxx_ru::BUILD_ROOT ) {

	toolset.force_cpp0x_std
	global_include_path "."

	required_prj( "tests/prj.rb" )
	required_prj( "samples/prj.rb" )
}