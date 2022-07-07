#include "../headers/network.hpp"

namespace NetworkLayer {

    Network::Network(const ContextPtr &context_ptr, IPAddress connectionIP) : contextPtr{context_ptr},
                                                                              stream{*context_ptr},
                                                                              ipAddress{connectionIP} {

    }

    void Network::setIpAddress(const NetworkLayer::IPAddress &connectionIP) {
        if (!connectionIP.host.empty()) {
            ipAddress.host = connectionIP.host;
        }
        if (!connectionIP.port.empty()) {
            ipAddress.port = connectionIP.port;
        }
    }

    bool Network::start() {
        try {
            tcp::resolver resolver(*contextPtr);
            tcp::resolver::results_type endpoints{resolver.resolve(ipAddress.host, ipAddress.port)};
            stream.connect(endpoints);
        } catch (const std::exception &e) {
            return false;
        }
        return true;
    }

    void Network::send(const std::string &city_name, const std::string &token) {
        city = city_name;
        std::string target{"/data/2.5/weather?q=" + city + "&appid=" + token + "&units=metric"};
        http::request<http::string_body> req{http::verb::get, target, 10};
        send(req);
    }

    std::string Network::receive() {
        auto resp{p_receive()};
        auto statusCode{static_cast<int>(resp.result())};
        if (statusCode != 200) {
            if (statusCode == 404) {
                throw std::runtime_error("Can't get weather in " + city);
            } else if (statusCode == 401) {
                throw std::runtime_error("Invalid api key");
            }
            throw std::runtime_error("Bad response");
        }

        std::string resp_str{boost::asio::buffers_begin(resp.body().data()),
                             boost::asio::buffers_end(resp.body().data())};
        try {
            auto temperature{std::to_string(boost::json::parse(resp_str).at("main").at("temp").to_number<double>())};
            auto wind_speed{std::to_string(boost::json::parse(resp_str).at("wind").at("speed").to_number<double>())};
            auto wind_direction{boost::json::parse(resp_str).at("wind").at("deg").to_number<int>()};

            std::string receive_str{"City: " + city + "\n" +
                                    "Temperature: " + temperature.substr(0, temperature.find('.') + 3) + "°C\n" +
                                    "Wind's speed: " + wind_speed.substr(0, wind_speed.find('.') + 3) + " meter/sec\n" +
                                    "Wind's direction: " + std::to_string(wind_direction) + "°\n"};

            return receive_str;
        } catch (const boost::exception &e) {
            throw std::runtime_error("Response parse error");
        }
    }

    void Network::send(const http::request<http::string_body> &request) {
        beast::error_code error;
        http::write(stream, request, error);
        if (error) {
            throw std::runtime_error("Bad request");
        }
    }

    http::response<http::dynamic_body> Network::p_receive() {
        http::response<http::dynamic_body> resp;
        beast::flat_buffer buffer;
        beast::error_code error;
        http::read(stream, buffer, resp, error);
        if (error) {
            throw std::runtime_error("Response error");
        }

        stream.socket().shutdown(tcp::socket::shutdown_both);
        return resp;
    }
}
