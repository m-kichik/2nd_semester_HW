#include <iostream>

#include <winsock2.h>
#include <windows.h>

#include <boost/asio.hpp>

class ChatClient {
public:
    explicit ChatClient(std::string  name) : userName(std::move(name)),
                                    endpoint(boost::asio::ip::address::from_string(raw_ip_address), port),
                                    socket(ioService, endpoint.protocol()) {
        connectionFailure = establishConnection();
    }

private:
    int establishConnection() {
        try {
            socket.connect(endpoint);
        } catch (boost::system::system_error & e) {
            std::cerr << "Error occured! Message: " << e.what() << std::endl;
            return e.code().value();
        }

        return 0;
    }

private:
    int writing() {
        std::string rowMessage;

        for (; std::getline(std::cin, rowMessage);) {
            auto prepMessage = userName + ": " + rowMessage + endSymbol;
            boost::asio::write(socket, boost::asio::buffer(prepMessage));

            if(rowMessage == leavingChat) return 0;
        }

        return 1;
    }

    void reading() {
        for(; ;) {
            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer, "\n");
            std::string message;
            std::istream input_stream(&buffer);
            std::getline(input_stream, message, '\n');

            std::cout << message << std::endl;
            Beep(400, 800);

            if (message.ends_with(leavingChat)) break;
        }
    }

public:
    auto runChatClient() {
        if (connectionFailure) {
            return connectionFailure;
        }

        std::cout << "To leave chat enter " << leavingChat << std::endl;
        std::cout << "Good chatting!" << std::endl;

        auto reader = std::thread(&ChatClient::reading, this);

        auto retVal = writing();

        if(retVal) {
            boost::asio::write(socket, boost::asio::buffer(userName + ": " + leavingChat + endSymbol));
        }

        reader.join();

        std::cout << "Chat left successfully" << std::endl;

        system("pause");

        return connectionFailure;
    }

private:
    const std::size_t port = 3333u;
    const std::string raw_ip_address = "127.0.0.1";
    std::string userName;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::socket socket;
    int connectionFailure = 0u;
    std::string leavingChat = "LEAVE";
    const char endSymbol = '\n';
};

int main() {
    system("chcp 1251");

    std::cout << "Enter your name to start chatting: " << std::endl;
    std::string name;
    std::getline(std::cin, name);

    auto retVal = ChatClient(name).runChatClient();

    return retVal;
}