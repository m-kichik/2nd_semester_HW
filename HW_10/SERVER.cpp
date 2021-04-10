#include <iostream>
#include <thread>

#include <boost/asio.hpp>

class ChatClient {
public:
    ChatClient(std::string  name) : userName(std::move(name)), endpoint(boost::asio::ip::address_v4::any(), port),
                                    acceptor(ioService, endpoint.protocol()),
                                    socket(ioService) {
        connectionFailure = establishConnection();
    }

private:
    int establishConnection() {
        try {
            acceptor.bind(endpoint);
            acceptor.listen(size);
            acceptor.accept(socket);
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

            if (message.ends_with(leavingChat)) break;
        }
    }

public:
    auto runChatServer() {
        if (connectionFailure) {
            return connectionFailure;
        }

        std::cout << "To leave chat enter " << leavingChat << std::endl;
        std::cout << "Good chatting!" << std::endl;

        auto reader = std::thread(&ChatClient::reading, this);

        auto retVal = writing();

        reader.join();

        if(retVal) {
            boost::asio::write(socket, boost::asio::buffer(userName + ": " + leavingChat + endSymbol));
        }

        std::cout << "Chat left successfully" << std::endl;

        system("pause");

        return connectionFailure;
    }

private:
    const int size = 30;
    const std::size_t port = 3333u;
    std::string userName;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    int connectionFailure = 0;
    std::string leavingChat = "LEAVE";
    const char endSymbol = '\n';
};

int main() {
    system("chcp 1251");

    std::cout << "Enter your name to start chatting: " << std::endl;
    std::string name;
    std::getline(std::cin, name);

    auto retVal = ChatClient(name).runChatServer();

    return retVal;
}