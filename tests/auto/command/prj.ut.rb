
require 'mxx_ru/binary_unittest'

Mxx_ru::setup_target(
	Mxx_ru::Binary_unittest_target.new(
		"tests/auto/command/prj.ut.rb",
		"tests/auto/command/prj.rb" ) )
