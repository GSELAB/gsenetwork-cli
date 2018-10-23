#include "client/Http.h"

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

        std::cout << resp << std::endl;
        {
            // to do
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
    beast::http::response<http::dynamic_body> resp;
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
    beast::http::response<http::dynamic_body> resp;
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

        std::cout << resp << std::endl;
        {
            // to do
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