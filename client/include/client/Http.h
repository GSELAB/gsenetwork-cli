#include <beast/http.hpp>
#include <beast/core.hpp>
#include <beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <core/JsonHelper.h>
#include <core/Transaction.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <crypto/Common.h>
#include <crypto/GKey.h>
#include <core/Ballot.h>
#include <core/Log.h>

using tcp = boost::asio::ip::tcp;
namespace http = beast::http;

namespace client{

class Client{

public:
        Client() = default;

        Client(const std::string & host, const std::string & port, const int version=11):m_host(host),m_port(port){}

        void login();

        void logout();

        void getblock(const std::string & target, const uint64_t blocknumber);

        void getversion(const std::string & target);

        void transfer(const std::string & target, const std::string & recipient, uint64_t value);

        void toBeProducer(const std::string & target);

        void vote(const std::string & target, core::Ballot & ballot);

        void sethost(const std::string & host){m_host = host;}

        const std::string gethost(){return m_host;}

        void setport(const std::string & port){m_port = port;}

        const std::string getport(){return m_port;}

        void setversion(const int version){m_version = version;}

        const int getversion(){return m_version;}
private:
        void broadcast(core::Transaction & transaction);
private:
        std::string m_host = "127.0.0.1";
        std::string m_port = "50505";
        int m_version = 11;

        crypto::GKey m_key;
        bool m_isLogin = false;

        boost::asio::io_service m_ios;
        boost::asio::ip::tcp::resolver m_resolver{m_ios};
        boost::asio::ip::tcp::socket m_socket{m_ios};
};


}// client