#include <string>

#include <client/Http.h>
#include <core/Log.h>
#include <crypto/GKey.h>
#include <client/JsonRequest.h>

using namespace core;
using namespace client;
using namespace crypto;
using namespace chain;

Secret sec("BFCD34B08F053772E5D93D9D67410287697C7F93F3C49D48890EC4916B246BA7");
Secret EmptySecret;

void Client::login()
{
    m_isLogin = true;
    m_key.setSecret(sec);
}

void Client::logout()
{
    m_isLogin = false;
    m_key.setSecret(EmptySecret);
}

void Client::checkLogin()
{
    if (!m_isLogin) {
        throw ClientException("Login status is false.");
    }
}

void Client::send(std::string const& cmd, Json::Value const& value, std::function<void(std::string const&)> callback)
{
    beast::http::request<beast::http::string_body> request;
    boost::asio::connect(m_socket,
        m_resolver.resolve(boost::asio::ip::tcp::resolver::query{m_host, m_port}));
    request.method(http::verb::post);
    request.version = m_version;
    request.set(http::field::host, m_host);
    request.set(http::field::user_agent, "Beast");
    request.set(http::field::content_type, "application/json");
    request.target(cmd);
    request.body = value.toStyledString();
    request.set(http::field::content_length, std::to_string(request.body.size()));
    beast::http::write(m_socket, request);
    if (callback) {
        beast::http::response<http::string_body> response;
        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, response);
        callback(response.body);
    }

    boost::system::error_code ec;
    m_socket.shutdown(tcp::socket::shutdown_both,ec);
    if(ec && ec != boost::system::errc::not_connected)
        throw boost::system::system_error{ec};
}

void Client::broadcast(Transaction& tx)
{
    send("/push_transaction", toJson(tx), [this] (std::string const& buffer) {
        std::cout << buffer << std::endl;
    });
}

void Client::getVersion(std::string const& cmd)
{
    try {
        Json::Value requestVersion = toRequestVersion();
        send(cmd, requestVersion, [this] (std::string const& buffer) {
            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            if(reader.parse(buffer, root)) {
                std::cout << root["version"] << std::endl;
            } else {
                throw ClientException("version - parse json from rpc error.");
            }
        });
    } catch(std::exception const& e) {
        CERROR << e.what();
    }
}

void Client::getBlock(std::string const& cmd, uint64_t number)
{
    try {
        Json::Value requestBlock = toRequestBlock(number);
        CINFO << requestBlock.toStyledString();
        send(cmd, requestBlock, [this] (std::string const& buffer) {
            std::cout << buffer << std::endl;
        });
    } catch(std::exception const& e) {
        CERROR << e.what();
    }
}

void Client::getBalance(std::string const& cmd, std::string const& target)
{
    try {
        Address targetAddress(target);
        Json::Value requestBalance = toRequestBalance(targetAddress);
        send(cmd, requestBalance, [this] (std::string const& buffer) {
            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            if(reader.parse(buffer, root)) {
                std::cout << root["balance"].asUInt64() << std::endl;
            } else {
                throw ClientException("transfer - parse json from rpc error.");
            }
        });
    } catch(std::exception const& e) {
        CERROR << e.what();
    }
}

void Client::getProducer(std::string const& cmd, std::string const& target)
{
    try {
        Address targetAddress(target);
        Json::Value requestProducer = toRequestProducer(targetAddress);
        send(cmd, requestProducer, [this] (std::string const& buffer) {
            std::cout << buffer << std::endl;
        });
    } catch(std::exception const& e) {
        CERROR << e.what();
    }
}

void Client::transfer(std::string const& cmd, std::string const& recipient, uint64_t value)
{
    try {
        checkLogin();

        Address recipientAddress(recipient);
        Json::Value requestTransfer = toRequestTransfer(m_key.getAddress(), recipientAddress, value);
        send(cmd, requestTransfer, [this] (std::string const& buffer) {
            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            if(reader.parse(buffer, root)) {
                Transaction tx = toTransaction(root);
                tx.sign(m_key.getSecret());
                broadcast(tx);
            } else {
                throw ClientException("transfer - parse json from rpc error.");
            }
        });
    } catch (ClientException& e) {
        CERROR << e.what();
    } catch(std::exception& e) {
        CERROR << e.what();
    }

}

void Client::toBeProducer(std::string const& cmd)
{
    try {
        checkLogin();

        Json::Value requestBeProducer = toRequestBeProducer(m_key.getAddress());
        send(cmd, requestBeProducer, [this] (std::string const& buffer) {
            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            if(reader.parse(buffer, root)) {
                Transaction tx = toTransaction(root);
                tx.sign(m_key.getSecret());
                broadcast(tx);
            } else {
                throw ClientException("tobe producer - parse json from rpc error.");
            }
        });
    } catch (ClientException& e) {
        CERROR << e.what();
    } catch(std::exception& e) {
        CERROR << e.what();
    }
}

void Client::vote(std::string const& cmd, Ballot& ballot)
{
    try {
        checkLogin();

        Json::Value requestVote = toRequestVote(m_key.getAddress(), ballot);
        send(cmd, requestVote, [this] (std::string const& buffer) {
            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            if(reader.parse(buffer, root)) {
                Transaction tx = toTransaction(root);
                tx.sign(m_key.getSecret());
                broadcast(tx);
            } else {
                throw ClientException("vote - parse json from rpc error.");
            }
        });
    } catch (ClientException& e) {
        CERROR << e.what();
    } catch(std::exception const& e) {
        CERROR << e.what();
    }
}
