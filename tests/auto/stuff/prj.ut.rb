
require 'mxx_ru/binary_unittest'

Mxx_ru::setup_target(
	Mxx_ru::Binary_unittest_target.new(
		"tests/auto/stuff/prj.ut.rb",
		"tests/auto/stuff/prj.rb" ) )
