#include "../headers/client.hpp"

namespace ClientLayer {

    Client::Client(const NetworkLayer::ContextPtr &context_ptr) {
        NetworkLayer::IPAddress ip_address{"api.openweathermap.org", "80"};
        m_connection = std::make_shared<NetworkLayer::Network>(context_ptr, ip_address);
    }

    void Client::process() {
        if (!m_connection->start()) {
            throw std::runtime_error("Can't reach server");
        }
        m_connection->send(city_name, token);
        std::cout << m_connection->receive();
    }

    void Client::setCity(const std::string &city_name) {
        this->city_name = city_name;
    }

    void Client::setToken(const std::string &token) {
        this->token = token;
    }

    void Client::setIpAddress(const NetworkLayer::IPAddress &ipAddress) {
        m_connection->setIpAddress(ipAddress);
    }
}