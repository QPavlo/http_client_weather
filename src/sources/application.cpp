#include "../headers/application.hpp"

static std::string city;
static std::string token;
static NetworkLayer::IPAddress ip_address;

Application::Application(int argc, char *argv[]) {
    try {
        pr_option::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Show help")
                ("city,c", pr_option::value<std::string>(),
                 "Enter city (If the city name is divided into several \n"
                 "words - use _ instead of space)")
                ("token,t", pr_option::value<std::string>(), "Enter token")
                ("address,a", pr_option::value<std::string>(), "Enter host address")
                ("port,p", pr_option::value<std::string>(), "Enter port");

        pr_option::variables_map vm;
        pr_option::store(parse_command_line(argc, argv, desc), vm);
        pr_option::notify(vm);

        if (vm.count("help") || vm.empty()) {
            std::cout << desc << '\n';
            exit(0);
        } else {
            if (vm.count("city")) {
                city = vm["city"].as<std::string>();
            }

            if (vm.count("token")) {
                token = vm["token"].as<std::string>();
            } else {
                std::ifstream file;
                file.exceptions(std::ifstream::badbit);
                try {
                    file.open("token.txt");
                    std::string line;
                    while (std::getline(file, line)) {
                        token.append(line);
                    }
                } catch (const std::ifstream::failure &e) {
                    std::cerr << e.what() << '\n';
                }
                file.close();
            }

            if (vm.count("address")) {
                ip_address.host = vm["address"].as<std::string>();
            }

            if (vm.count("port")) {
                ip_address.port = vm["port"].as<std::string>();
            }
        }
    }
    catch (const pr_option::error &ex) {
        std::cerr << ex.what() << '\n';
        exit(0);
    }
}

int Application::exec() {
    try {
        auto client_ptr{std::make_shared<boost::asio::io_context>()};
        ClientLayer::Client client(client_ptr);
        if (!ip_address.host.empty() or !ip_address.port.empty()) {
            client.setIpAddress(ip_address);
        }
        client.setCity(city);
        client.setToken(token);
        client.process();
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception" << '\n';
    }
    return 0;
}