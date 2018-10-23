#include <string>
#include <sstream>
#include "client/Http.h"
#include "parser/commandParser.hpp"

using namespace cmdparse;

namespace hdl
{
    std::string default_host = "127.0.0.1";
    std::string default_port = "50505";
    int default_version = 11;

    client::Endpoint client(default_host, default_port, default_version);

    int command_handler(po::variables_map & vm, po::options_description & desc)
    {
        if(vm.count("quit")){std::cout<<"program is starting exit"<<std::endl;exit(0);}

        std::string host = default_host;
        std::string port = default_port;
        int version = default_version;
        std::string target;
        std::string body;
        std::string sender;
        std::string recipient;
        uint64_t value;

        bool hashost = false;
        bool hasport = false;
        bool hasdata = false;
        bool hasversion = false;
        bool hassender = false;
        bool hasrecipient = false;
        bool hasvalue = false;

        if(vm.count("host")){hashost = true;host = vm["host"].as<std::string>();client.sethost(host);}
        if(vm.count("port")){hasport = true;port = vm["port"].as<std::string>();client.setport(port);}
        if(vm.count("data")){hasdata = true;body = vm["data"].as<std::string>();}
        if(vm.count("version")){hasversion = true;version = vm["version"].as<int>();client.setversion(version);}
        if(vm.count("sender")){hassender = true; sender = vm["sender"].as<std::string>();}
        if(vm.count("recipient")){hasrecipient = true;recipient = vm["recipient"].as<std::string>();}
        if(vm.count("value")){hasvalue = true;value = vm["value"].as<uint64_t>();}

        if ( vm.count("help"))
        {
            std::cout << "Basic Command Line Parameter App" << std::endl
                      << desc << std::endl;

            return commandParser::SUCCESS;
        }
        else if (vm.count("getblock"))
        {
            if(!hasdata){return commandParser::ERROR_IN_COMMAND_LINE;}
            target = "/get_block";

            client.getblock(target, body);

            return commandParser::SUCCESS;
        }
        else if (vm.count("getversion"))
        {
            target = "/get_version";
            client.getversion(target);

            return commandParser::SUCCESS;
        }
        else if (vm.count("transfer"))
        {
            if(!hassender||!hasrecipient||!hasvalue){return commandParser::ERROR_IN_COMMAND_LINE;}

            //{"sender":"0xfffffffffffffffffffffffffffffffffffffffe","recipient":"0xfffffffffffffffffffffffffffffffffffffffe","value":1}//
            body = "{\"sender\":\"" +sender +"\","+"\"recipient\":\""+recipient+"\"," +"\"value\":"+std::to_string(value)+"}";
            std::cout<<"body = "<<body<<std::endl;
            target = "/create_transaction";

            client.transfer(target, body);

            return commandParser::SUCCESS;
        }
        else
        {
            return commandParser::ERROR_IN_COMMAND_LINE;
        }
    }
} //namespace hdl


void commandParser::init()
{
    init_command_line();

    set_command_parse_handler(hdl::command_handler);
}

void commandParser::init_command_line()
{
    try
    {
        desc.add_options()
          ("help,?", "Print help messages")
          ("quit,q", "quit")
          ("getblock", "get a block by number")
          ("getversion", "get version")
          ("transfer,t", "transfer accounts")
          ("sender,s",po::value<std::string>(), "sender")
          ("recipient,r",po::value<std::string>(),"recipient")
          ("version,v",po::value<uint64_t>(),"http version")
          ("value,n", po::value<uint64_t>(), "value")
          ("host,h", po::value<std::string>(), "host ip")
          ("port,p", po::value<std::string>(), "port number")
          ("data,d", po::value<std::string>(), "http request body");

    }catch(std::exception& e){
        std::cerr << "Unhandled Exception reached the top of init_command_line: "
                  << e.what() << ", application will now exit" << std::endl;
    exit(1);
   }
}

int commandParser::parse_command_line(int argc, std::vector<const char *> & argv)
{
    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, &(argv[0]), desc), vm);

        po::notify(vm);
    }
    catch(po::error& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        return ERROR_IN_COMMAND_LINE;
    }
    command_handler(vm, desc);

    return SUCCESS;
}