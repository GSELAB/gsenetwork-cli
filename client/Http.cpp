#include <client/Http.h>
#include <core/Log.h>
#include <crypto/GKey.h>

using namespace core;
using namespace client;
using namespace crypto;
using namespace chain;

Secret sec("4077db9374f9498aff4b4ae6eb1400755655b50457930193948d2dc6cf70bf0f");
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

void Client::getblock(const std::string & target, const uint64_t blocknumber)
{
    beast::http::request<beast::http::string_body> req;
    beast::http::response<http::string_body> resp;

    std::string body;
    std::stringstream ss;
    ss  << "{"
        << "\"blockNumber\""
        << ":"
        << blocknumber
        << "}";
    ss >> body;
    try
    {
        boost::asio::connect(m_socket,
                        m_resolver.resolve(boost::asio::ip::tcp::resolver::query{m_host, m_port}));
        req.method(http::verb::post);
        req.target(target);
        req.version = m_version;
        req.set(http::field::host, m_host);
        req.set(http::field::user_agent, "Beast");

        req.set(http::field::content_type, "application/json");
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        //std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;
            std::cout<<resp_body<<std::endl;
        }

        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both,ec);

        if(ec && ec != boost::system::errc::not_connected)
            throw boost::system::system_error{ec};
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Client::getversion(const std::string & target)
{
    beast::http::request<beast::http::string_body> req;
    beast::http::response<http::string_body> resp;
    try
    {
        boost::asio::connect(m_socket,
                        m_resolver.resolve(boost::asio::ip::tcp::resolver::query{m_host, m_port}));
        req.method(http::verb::post);
        req.target(target);
        req.version = m_version;
        req.set(http::field::host, m_host);
        req.set(http::field::user_agent, "Beast");

        req.set(http::field::content_type, "application/json");

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        //std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;
            std::cout<<resp_body<<std::endl;
        }

        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both,ec);

        if(ec && ec != boost::system::errc::not_connected)
            throw boost::system::system_error{ec};
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}

void Client::transfer(const std::string & target, const std::string & recipient, uint64_t value)
{
    if(!m_isLogin){
        CINFO << "please login first";
        return;
    }
    beast::http::request<beast::http::string_body> req;
    beast::http::response<http::string_body> resp;
    std::string body;
    std::string sender = m_key.getAddress().hex();
    std::stringstream ss;
    ss  << "{"
        << "\"sender\""
        << ":"
        << "\""
        << sender
        << "\""
        << ","
        << "\"recipient\""
        << ":"
        << "\""
        << recipient
        << "\""
        << ","
        << "\"value\""
        << ":"
        << value
        << "}";
    ss  >> body;
    try
    {
        boost::asio::connect(m_socket,
                        m_resolver.resolve(boost::asio::ip::tcp::resolver::query{m_host, m_port}));
        req.method(http::verb::post);
        req.target(target);
        req.version = m_version;
        req.set(http::field::host, m_host);
        req.set(http::field::user_agent, "Beast");

        req.set(http::field::content_type, "application/json");
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        //std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;
            //std::cout<< resp_body <<std::endl;

            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            std::string ret;
            if(reader.parse(resp_body, root))
            {
                ChainID chainID = std::stoll(root["ChainID"].asString(),0,16);
                uint32_t type = std::stol(root["type"].asString(),0,16);
                Address sender = Address(root["sender"].asString());
                Address recipient = Address(root["recipient"].asString());
                bytes data;
                std::string dataStr = root["data"].asString();
                data.insert(data.begin(), dataStr.begin(), dataStr.end());
                uint64_t value = std::stoll(root["value"].asString(), 0, 16);
                Transaction transaction;
                transaction.setChainID(chainID);
                transaction.setType(type);
                transaction.setSender(sender);
                transaction.setRecipient(recipient);
                transaction.setData(data);
                transaction.setValue(value);
                transaction.sign(m_key.getSecret());

                broadcast(transaction);
            }
        }

        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both,ec);

        if(ec && ec != boost::system::errc::not_connected)
            throw boost::system::system_error{ec};
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}
void Client::toBeProducer(const std::string & target)
{
    if(!m_isLogin){
        CINFO << "please login first";
        return;
    }
    beast::http::request<beast::http::string_body> req;
    beast::http::response<http::string_body> resp;

    std::string body;
    std::string sender = m_key.getAddress().hex();
    std::stringstream ss;
    ss  << "{"
        << "\"sender\""
        << ":"
        << "\""
        << sender
        << "\""
        << "}";
    ss  >> body;
    try
    {
        boost::asio::connect(m_socket,
                        m_resolver.resolve(boost::asio::ip::tcp::resolver::query{m_host, m_port}));
        req.method(http::verb::post);
        req.target(target);
        req.version = m_version;
        req.set(http::field::host, m_host);
        req.set(http::field::user_agent, "Beast");

        req.set(http::field::content_type, "application/json");
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        //std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;

            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            std::string ret;
            if(reader.parse(resp_body, root))
            {
                ChainID chainID = std::stoll(root["ChainID"].asString(),0,16);
                uint32_t type = std::stol(root["type"].asString(),0,16);
                Address sender = Address(root["sender"].asString());
                Address recipient = Address(root["recipient"].asString());
                bytes data;
                std::string dataStr = root["data"].asString();
                data.insert(data.begin(), dataStr.begin(), dataStr.end());
                uint64_t value = std::stoll(root["value"].asString(),0,16);
                Transaction transaction;
                transaction.setChainID(chainID);
                transaction.setType(type);
                transaction.setSender(sender);
                transaction.setRecipient(recipient);
                transaction.setData(data);
                transaction.setValue(value);
                transaction.sign(m_key.getSecret());

                broadcast(transaction);
            }
        }

        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both,ec);

        if(ec && ec != boost::system::errc::not_connected)
            throw boost::system::system_error{ec};
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Client::vote(const std::string & target, Ballot & ballot)
{
    if(!m_isLogin){
        CINFO << "please login first";
        return;
    }
    beast::http::request<beast::http::string_body> req;
    beast::http::response<http::string_body> resp;

    std::string body;
    std::string sender = m_key.getAddress().hex();
    bytes data = ballot.getRLPData();
    std::stringstream ss;
    ss  << "{"
        << "\"sender\""
        << ":"
        << "\""
        << sender
        << "\""
        << ","
        << "\"data\""
        << ":"
        << "\""
        << data
        << "\""
        <<"}";
    ss  >> body;
    try
    {
        boost::asio::connect(m_socket,
                        m_resolver.resolve(boost::asio::ip::tcp::resolver::query{m_host, m_port}));
        req.method(http::verb::post);
        req.target(target);
        req.version = m_version;
        req.set(http::field::host, m_host);
        req.set(http::field::user_agent, "Beast");

        req.set(http::field::content_type, "application/json");
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        //std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;
            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            std::string ret;
            if(reader.parse(resp_body, root))
            {
                ChainID chainID = std::stoll(root["ChainID"].asString(),0,16);
                uint32_t type = std::stol(root["type"].asString(),0,16);
                Address sender = Address(root["sender"].asString());
                Address recipient = Address(root["recipient"].asString());
                bytes data;
                std::string dataStr = root["data"].asString();
                data.insert(data.begin(), dataStr.begin(), dataStr.end());
                uint64_t value = std::stoll(root["value"].asString(),0,16);
                Transaction transaction;
                transaction.setChainID(chainID);
                transaction.setType(type);
                transaction.setSender(sender);
                transaction.setRecipient(recipient);
                transaction.setData(data);
                transaction.setValue(value);
                transaction.sign(m_key.getSecret());

                broadcast(transaction);
            }
        }

        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both,ec);

        if(ec && ec != boost::system::errc::not_connected)
            throw boost::system::system_error{ec};
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Client::broadcast(core::Transaction & transaction)
{
    beast::http::request<beast::http::string_body> req;
    std::string body;
    body = toJson(transaction).toStyledString();
    try
    {
        boost::asio::connect(m_socket,
                        m_resolver.resolve(boost::asio::ip::tcp::resolver::query{m_host, m_port}));
        req.method(http::verb::post);
        req.target("/push_transaction");
        req.version = m_version;
        req.set(http::field::host, m_host);
        req.set(http::field::user_agent, "Beast");

        req.set(http::field::content_type, "application/json");
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        CINFO << "Client::broadcast\n";
        std::cout<< body << std::endl;

        beast::http::write(m_socket, req);

        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both,ec);

        if(ec && ec != boost::system::errc::not_connected)
            throw boost::system::system_error{ec};
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}