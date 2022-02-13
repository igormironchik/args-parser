
#undef NDEBUG
#include <iostream>
#include <chrono>
#include <cassert>
#include <cstdlib>
#include "args.hxx"
#include <tclap/CmdLine.h>
#include <boost/program_options.hpp>
#include <args-parser/all.hpp>
namespace po = boost::program_options;
using namespace std::chrono;
inline bool doubleequals(const double a, const double b)
{
    static const double delta = 0.0001;
    const double diff = a - b;
    return diff < delta && diff > -delta;
}
int main()
{
    const std::vector<std::string> carguments({"-i", "7", "-c", "a", "2.7", "--char", "b", "8.4", "-c", "c", "8.8", "--char", "d"});
    const std::vector<std::string> pcarguments({"progname", "-i", "7", "-c", "a", "2.7", "--char", "b", "8.4", "-c", "c", "8.8", "--char", "d"});
	const int argc = 14;
	const char * argv[ argc ] = { "progname", "-i", "7", "-c", "a", "2.7", "--char", "b", "8.4", "-c", "c", "8.8", "--char", "d" };
	// args-parser
    {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (unsigned int x = 0; x < 100000; ++x)
        {
            Args::CmdLine parser( argc, argv );
            Args::Arg integer( 'i', "int", true );
            Args::MultiArg characters( 'c', "char", true );
			parser.addArg(integer);
			parser.addArg(characters);
            parser.parse();
            const auto i = std::atoi(integer.value().c_str());
            const auto & c = characters.values();
            assert(i == 7);
            assert(c[0] == "a");
            assert(c[2] == "b");
            assert(c[4] == "c");
            assert(c[6] == "d");
			assert(doubleequals(std::atof(c[1].c_str()), 2.7));
			assert(doubleequals(std::atof(c[3].c_str()), 8.4));
			assert(doubleequals(std::atof(c[5].c_str()), 8.8));
        }
        high_resolution_clock::duration runtime = high_resolution_clock::now() - start;
        std::cout << "args-parser seconds to run: " << duration_cast<duration<double>>(runtime).count() << std::endl;
    }
    // args
    {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (unsigned int x = 0; x < 100000; ++x)
        {
            std::vector<std::string> arguments(carguments);
            args::ArgumentParser parser("This is a test program.", "This goes after the options.");
            args::ValueFlag<int> integer(parser, "integer", "The integer flag", {'i', "int"});
            args::ValueFlagList<char> characters(parser, "characters", "The character flag", {'c', "char"});
            args::PositionalList<double> numbers(parser, "numbers", "The numbers position list");
            parser.ParseArgs(arguments);
            const int i = args::get(integer);
            const std::vector<char> c(args::get(characters));
            const std::vector<double> n(args::get(numbers));
            assert(i == 7);
            assert(c[0] == 'a');
            assert(c[1] == 'b');
            assert(c[2] == 'c');
            assert(c[3] == 'd');
            assert(doubleequals(n[0], 2.7));
            assert(doubleequals(n[1], 8.4));
            assert(doubleequals(n[2], 8.8));
        }
        high_resolution_clock::duration runtime = high_resolution_clock::now() - start;
        std::cout << "Taywee/args seconds to run: " << duration_cast<duration<double>>(runtime).count() << std::endl;
    }
    // tclap
    {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (unsigned int x = 0; x < 100000; ++x)
        {
            std::vector<std::string> arguments(pcarguments);
            TCLAP::CmdLine cmd("Command description message", ' ', "0.9");
            TCLAP::ValueArg<int> integer("i", "int", "The integer flag", false, 0, "integer", cmd);
            TCLAP::MultiArg<char> characters("c", "char", "The character flag", false, "characters", cmd);
            TCLAP::UnlabeledMultiArg<double> numbers("numbers", "The numbers position list", false, "foo", cmd, false);
            cmd.parse(arguments);
            const int i = integer.getValue();
            const std::vector<char> c(characters.getValue());
            const std::vector<double> n(numbers.getValue());
            assert(i == 7);
            assert(c[0] == 'a');
            assert(c[1] == 'b');
            assert(c[2] == 'c');
            assert(c[3] == 'd');
            assert(doubleequals(n[0], 2.7));
            assert(doubleequals(n[1], 8.4));
            assert(doubleequals(n[2], 8.8));
        }
        high_resolution_clock::duration runtime = high_resolution_clock::now() - start;
        std::cout << "tclap seconds to run: " << duration_cast<duration<double>>(runtime).count() << std::endl;
    }
    // boost::program_options
    {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (unsigned int x = 0; x < 100000; ++x)
        {
            std::vector<std::string> arguments(carguments);
            po::options_description desc("This is a test program.");
            desc.add_options()
                ("int,i", po::value<int>(), "The integer flag")
                ("char,c", po::value<std::vector<char>>(), "The character flag")
                ("numbers", po::value<std::vector<double>>(), "The numbers flag");
            po::positional_options_description p;
            p.add("numbers", -1);
            po::variables_map vm;
            po::store(po::command_line_parser(carguments).options(desc).positional(p).run(), vm);
            const int i = vm["int"].as<int>();
            const std::vector<char> c(vm["char"].as<std::vector<char>>());
            const std::vector<double> n(vm["numbers"].as<std::vector<double>>());
            assert(i == 7);
            assert(c[0] == 'a');
            assert(c[1] == 'b');
            assert(c[2] == 'c');
            assert(c[3] == 'd');
            assert(doubleequals(n[0], 2.7));
            assert(doubleequals(n[1], 8.4));
            assert(doubleequals(n[2], 8.8));
        }
        high_resolution_clock::duration runtime = high_resolution_clock::now() - start;
        std::cout << "boost::program_options seconds to run: " << duration_cast<duration<double>>(runtime).count() << std::endl;
    }
    return 0;
}
