
require 'mxx_ru/binary_unittest'

Mxx_ru::setup_target(
	Mxx_ru::Binary_unittest_target.new(
		"tests/auto/context/prj.ut.rb",
		"tests/auto/context/prj.rb" ) )
