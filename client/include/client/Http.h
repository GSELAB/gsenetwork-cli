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

namespace client {

class Client {
public:
    Client() = default;

    Client(std::string const& host, std::string const& port, int version = 11): m_host(host), m_port(port) {}

    void login(std::string privateKey);

    void logout();

    void getBlock(std::string const& cmd, uint64_t number);

    void getTransaction(std::string const& cmd, std::string const& txHash);

    void getVersion(std::string const& cmd);

    void getHeight(std::string const& cmd);

    void getSolidifyHeight(std::string const& cmd);

    void getProducerList(std::string const& cmd);

    void transfer(std::string const& cmd, std::string const& recipient,bytes const& data, uint64_t value);

    void toBeProducer(std::string const& cmd);

    void vote(std::string const& cmd, core::Ballot& ballot);

    void getBalance(std::string const& cmd, std::string const& target);

    void getProducer(std::string const& cmd, std::string const& target);

    void getAccount(std::string const& cmd, std::string const& target);

public:
    void setHost(std::string const& host) { m_host = host; }

    std::string const& getHost() const { return m_host; }

    void setPort(std::string const& port) { m_port = port; }

    std::string const& getPort() const { return m_port; }

    void setVersion(int version) { m_version = version; }

    int getVersion() const { return m_version; }

protected:
    void checkLogin();

    void send(std::string const& cmd, Json::Value const& value, std::function<void(std::string const&)> callback);

    void broadcast(core::Transaction& transaction);

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

}
