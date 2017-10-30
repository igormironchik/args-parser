
require 'mxx_ru/binary_unittest'

Mxx_ru::setup_target(
	Mxx_ru::Binary_unittest_target.new(
		"tests/auto/api/prj.ut.rb",
		"tests/auto/api/prj.rb" ) )
