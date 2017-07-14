
def find_tests( path )
	tests = Array.new

	Dir.foreach( path ) { |name|
		full = File.expand_path( name, path )

		if name != "." && name != ".."
			if File.directory?( full )
				tests.push( *find_tests( full ) )
			elsif File.executable?( full )
				tests.push( full )
			end
		end
	}

	return tests
end

tests = find_tests( "tests" )

tests.each { |t|
	Dir.chdir( File.dirname( t ) ) {
		if !system( t )
			break
		end
	}
}
