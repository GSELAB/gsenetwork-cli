#include <string>
#include <sstream>
#include <client/Http.h>
#include <parser/commandParser.hpp>

#include <crypto/Common.h>
#include <crypto/GKey.h>

using namespace cmdparse;
using namespace core;
using namespace crypto;

namespace hdl
{
    //std::string default_host = "132.232.52.156";
    std::string default_host = "127.0.0.1";
    std::string default_port = "50505";
    int default_version = 11;

    client::Client client(default_host, default_port, default_version);

    int command_handler(po::variables_map & vm, po::options_description & desc)
    {
        if (vm.count("quit")) {
            std::cout<<"program is starting exit"<<std::endl;
            exit(0);
        }

        std::string host;
        std::string port;
        std::string recipient;
        std::string target;
        uint64_t value;
        uint64_t number;
        std::vector<std::string> candidates;
        std::vector<uint64_t> ballots;

        if (vm.count("host")) {
            host = vm["host"].as<std::string>();
            client.setHost(host);
        }

        if (vm.count("port")) {
            port = vm["port"].as<std::string>();
            client.setPort(port);
        }

        if ( vm.count("help")) {
            CINFO << "Basic Command Line Parameter App" << std::endl
                      << desc << std::endl;
            return commandParser::SUCCESS;
        } else if (vm.count("login")) {
            client.login();
            return commandParser::SUCCESS;
        } else if (vm.count("logout")) {
            client.logout();
            return commandParser::SUCCESS;
        } else if (vm.count("getblock")) {
            number = vm["getblock"].as<uint64_t>();
            client.getBlock("/get_block", number);
            return commandParser::SUCCESS;
        } else if (vm.count("getbalance")) {
            target = vm["getbalance"].as<std::string>();
            client.getBalance("/get_balance", target);
            return commandParser::SUCCESS;
        } else if (vm.count("getproducer")) {
            target = vm["getproducer"].as<std::string>();
            client.getProducer("/get_producer", target);
            return commandParser::SUCCESS;
        } else if (vm.count("getversion")) {
            client.getVersion("/get_version");
            return commandParser::SUCCESS;
        } else if (vm.count("transfer")) {
            if (vm.count("recipient")) {
                recipient = vm["recipient"].as<std::string>();
            }

            if (vm.count("value")) {
                value = vm["value"].as<uint64_t>();
            }

            client.transfer("/create_transaction", recipient, value);
            return commandParser::SUCCESS;
        } else if (vm.count("tobeproducer")) {
            client.toBeProducer("/create_producer");
            return commandParser::SUCCESS;
        } else if (vm.count("vote")) {
            if (vm.count("candidate")) {
                candidates = vm["candidate"].as<std::vector<std::string> >();
            }

            if (vm.count("ballot")) {
                ballots = vm["ballot"].as<std::vector<uint64_t> >();
            }

            if (candidates.size() != ballots.size()) {
                return commandParser::ERROR_IN_COMMAND_LINE;
            }

            int len = static_cast<int>(candidates.size());
            Ballot ballot;
            for(int i=0; i<len; i++){
                Address address(candidates[i]);
                uint64_t value(ballots[i]);
                Candidate candidate(address, value);
                ballot.put(candidate);
            }

            client.vote("/vote", ballot);
            return commandParser::SUCCESS;
        } else {
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
    try {
        desc.add_options()
            ("help,?", "Print help messages")
            ("quit,q", "quit")
            ("getproducer", po::value<std::string>(),"get producer")
            ("getbalance", po::value<std::string>(),"get account balance")
            ("getblock", po::value<uint64_t>(),"get a block by number")
            ("getversion", "get version")
            ("transfer", "transfer accounts")
            ("tobeproducer","to be a producer")
            ("vote", "vote")
            ("login","login")
            ("logout","log out")
            ("host,h", po::value<std::string>(), "host ip")
            ("port,p", po::value<std::string>(), "port number")
            ("recipient,r",po::value<std::string>(),"recipient")
            ("value,v", po::value<uint64_t>(), "transfer amount")
            ("candidate,c",po::value<std::vector<std::string> >(),"candidates")
            ("ballot,b",po::value<std::vector<uint64_t> >(),"ballot");

    } catch(std::exception& e) {
        CERROR << "Unhandled Exception reached the top of init_command_line: "
                  << e.what() << ", application will now exit";
        exit(1);
   }
}

int commandParser::parse_command_line(int argc, std::vector<const char *> & argv)
{
    int size = static_cast<int>(argv.size()-1);
    if(argc != size)
        throw std::length_error("argc != argv.size()-1");
    if(argc<1)
        return SUCCESS;

    std::string command(argv[1]);
    if(command == "?"){command = "help";}
    command = "--" + command;
    delete [] argv[1];
    argv[1] = command.c_str();
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, &(argv[0]), desc), vm);
        po::notify(vm);
    } catch(po::error& e) {
        CERROR << "ERROR: " << e.what();
        CERROR << desc;
        return ERROR_IN_COMMAND_LINE;
    }

    command_handler(vm, desc);
    return SUCCESS;
}
