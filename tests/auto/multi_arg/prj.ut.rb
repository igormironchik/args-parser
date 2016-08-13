
require 'mxx_ru/binary_unittest'

Mxx_ru::setup_target(
	Mxx_ru::Binary_unittest_target.new(
		"tests/auto/multi_arg/prj.ut.rb",
		"tests/auto/multi_arg/prj.rb" ) )
