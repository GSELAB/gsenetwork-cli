/*
 * Copyright (c) 2018 GSENetwork
 *
 * This file is part of GSENetwork.
 *
 * GSENetwork is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or any later version.
 *
 */

#include <beast/http.hpp>
#include <beast/core.hpp>
#include <beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <boost/program_options.hpp>

#include "parser/commandParser.hpp"

using namespace std;
using namespace cmdparse;


int main(int argc, char **argv)
{
    commandParser parser;

    parser.init();

    std::cout<< "GSENetwork Command Line!" << std::endl;

    bool done = false;
    std::string input;

    while(!done)
    {
        std::cout<<"Enter Command:\n> ";
        std::getline(std::cin, input);

        std::vector<const char *> args;
        args.push_back(argv[0]); 
        std::istringstream iss(input);

        std::string cmd;
        while(iss >> cmd){
            char * arg = new char[cmd.size()+1];
            copy(cmd.begin(), cmd.end(), arg);
            arg[cmd.size()] = '\0';
            args.push_back(arg);
        }
        args.push_back(nullptr);

        argc = static_cast<int>(args.size())-1;

        parser.parse_command_line(argc, args);
    }

    return 0;
}
