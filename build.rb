require 'mxx_ru/cpp'

MxxRu::Cpp::composite_target( Mxx_ru::BUILD_ROOT ) {

	toolset.force_cpp14
	global_include_path "."

#	global_define( "ARGS_WSTRING_BUILD" ) 

	required_prj( "tests/prj.rb" )
	required_prj( "samples/prj.rb" )
}
