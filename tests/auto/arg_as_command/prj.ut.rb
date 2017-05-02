
require 'mxx_ru/binary_unittest'

Mxx_ru::setup_target(
	Mxx_ru::Binary_unittest_target.new(
		"tests/auto/arg_as_command/prj.ut.rb",
		"tests/auto/arg_as_command/prj.rb" ) )
