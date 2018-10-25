#include <boost/program_options.hpp>
#include <vector>
#include <iostream>
#include <functional>
#include "core/Ballot.h"

namespace po = boost::program_options;

using CommandParseHandler = std::function<int(po::variables_map & vm, po::options_description & desc)>;

namespace cmdparse
{
    class commandParser
    {
    public:
        enum
        {
          ERROR_IN_COMMAND_LINE = 1,
          SUCCESS = 0,
          ERROR_UNHANDLED_EXCEPTION = 2
        };

        commandParser():desc("Options"){}

        void init();

        int parse_command_line(int argc, std::vector<const char *> & argv);

    protected:
        void set_command_parse_handler(CommandParseHandler hdl){command_handler = hdl;}

        void init_command_line();

    protected:
            po::options_description desc;

            CommandParseHandler command_handler;
    };
}//namespace cmdparse