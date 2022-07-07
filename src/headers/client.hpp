#pragma once

#include "network.hpp"

namespace ClientLayer
{
    using tcp = NetworkLayer::tcp;

    class Client
    {
    public:
        /**
         * @brief Ctor
         * @param context_ptr - smart pointer to io_context
         * @param host - server address
         * @param port - server port
         * */
        Client(const NetworkLayer::ContextPtr &context_ptr);

        /**
         * @brief Main function that connections to server, sends request and shows result
         * */
        void process();

        void setCity(const std::string &city_name);
        void setToken(const std::string &token);
        void setIpAddress(const NetworkLayer::IPAddress& ipAddress);
    private:
        std::shared_ptr<NetworkLayer::Network> m_connection;
        std::string city_name;
        std::string token;
    };
}