#include "client/Http.h"

using namespace core;
using namespace client;

void Endpoint::getblock(const std::string & target, const std::string & body)
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

void Endpoint::getversion(const std::string & target)
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

void Endpoint::transfer(const std::string & target, const std::string & body)
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
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        //std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;
            std::cout<< resp_body <<std::endl;

            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            std::string ret;
            if(reader.parse(resp_body, root))
            {
                std::string str_transaction = root["transfer"].asString();
                bytes data;
                data.insert(data.begin(), str_transaction.begin(), str_transaction.end());

                Secret sec("4077db9374f9498aff4b4ae6eb1400755655b50457930193948d2dc6cf70bf0f");
                Transaction transaction(data);
                transaction.sign(sec);

                //std::string ret = toJson(transaction).toStyledString();
                //std::cout << ret <<std::endl;

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
void Endpoint::toBeProducer(const std::string & target, const std::string & body)
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
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;
            //std::cout << resp_body << std::endl;

            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            std::string ret;
            if(reader.parse(resp_body, root))
            {
                std::string str_transaction = root["producer"].asString();
                bytes data;
                data.insert(data.begin(), str_transaction.begin(), str_transaction.end());

                Secret sec("4077db9374f9498aff4b4ae6eb1400755655b50457930193948d2dc6cf70bf0f");
                Transaction transaction(data);
                transaction.sign(sec);

                //std::string ret = toJson(transaction).toStyledString();
                //std::cout << ret <<std::endl;

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

void Endpoint::vote(const std::string & target, const std::string & body)
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
        req.set(http::field::content_length, std::to_string(body.size()));
        req.body = body;

        beast::http::write(m_socket, req);

        beast::flat_buffer buffer;
        beast::http::read(m_socket, buffer, resp);

        std::cout << resp << std::endl;
        {
            // to do
            std::string resp_body = resp.body;
            //std::cout << resp_body << std::endl;

            Json::Reader reader(Json::Features::strictMode());
            Json::Value root;
            std::string ret;
            if(reader.parse(resp_body, root))
            {
                std::string str_transaction = root["vote"].asString();
                bytes data;
                data.insert(data.begin(), str_transaction.begin(), str_transaction.end());

                Secret sec("4077db9374f9498aff4b4ae6eb1400755655b50457930193948d2dc6cf70bf0f");
                Transaction transaction(data);
                transaction.sign(sec);

                //std::string ret = toJson(transaction).toStyledString();
                //std::cout << ret <<std::endl;

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

void Endpoint::broadcast(core::Transaction & transaction)
{
    beast::http::request<beast::http::string_body> req;
    std::string tem, body;
    bytes data = transaction.getRLPData();
    tem.insert(tem.begin(), data.begin(), data.end());
    body = toJson("transaction", tem).toStyledString();
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