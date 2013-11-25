require 'mxx_ru/cpp'

require 'Args/version'

MxxRu::Cpp::lib_target {

	target( "Args." + Args::VERSION )
	implib_path( "lib" )

	cpp_source( "arg.cpp" )
	cpp_source( "arg_iface.cpp" )
	cpp_source( "cmd_line.cpp" )
	cpp_source( "context.cpp" )
	cpp_source( "exceptions.cpp" )
	cpp_source( "multi_arg.cpp" )
}
