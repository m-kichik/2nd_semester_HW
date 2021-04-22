#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <utility>
#include <random>

#include <boost/asio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace sf;

class TronMaker {
public:
    TronMaker(Sprite bg) : endpoint(boost::asio::ip::address_v4::any(), port),
                           acceptor(ioService, endpoint.protocol()),
                           socket(ioService),
                           window(VideoMode(width, height), gameName),
                           background(std::move(bg)),
                           delay(100'000){
        connectionFailure = establishConnection();

        window.setFramerateLimit(60);

        for (auto x : field) {
            for (auto y : x) {
                y = 0u;
            }
        }

        std::default_random_engine dre;
        std::uniform_int_distribution uid(1, 4);

        serverPl.dir = Direction(uid(dre));
    };

    ~TronMaker() = default;

private:
    enum class Direction {
        down,
        left,
        right,
        up
    };

    struct Player {
        std::pair <std::size_t, std::size_t> position;
        Direction dir;
        Color color;
    };

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
    void draw() {
        window.draw(background);
        RectangleShape rect(Vector2f(cellSize, cellSize));
        for (auto i = 0U; i < fieldWidth; ++i)
        {
            for (auto j = 0U; j < fieldHeight; ++j)
            {
                switch (field[i][j])
                {
                    case 1U:
                        rect.setPosition(static_cast <float> (i * cellSize), static_cast <float> (j * cellSize));
                        rect.setFillColor(serverPl.color);
                        window.draw(rect);
                        break;
                    case 2U:
                        rect.setPosition(static_cast <float> (i * cellSize), static_cast <float> (j * cellSize));
                        rect.setFillColor(clientPl.color);
                        window.draw(rect);
                        break;
                    default:
                        break;
                }
                window.display();
            }
        }
    }

private:
    void readKey() { // server
        if ((Keyboard::isKeyPressed(Keyboard::Left)) && (serverPl.dir != Direction::right)) {
            currentDir = Direction::left;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right)) && (serverPl.dir != Direction::left)) {
            currentDir = Direction::right;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up)) && (serverPl.dir != Direction::down)) {
            currentDir = Direction::up;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Down)) && (serverPl.dir != Direction::up)) {
            currentDir = Direction::down;
        }
    }

private:
    void acceptDirection() {
        boost::asio::streambuf buffer;

        boost::asio::read_until(socket, buffer, '\n');
        std::istream input_stream(&buffer);

        char c = '0';
        input_stream >> c;
        switch (c) {
            case 'l':
                clientPl.dir = Direction::left;
                break;
            case 'r':
                clientPl.dir = Direction::right;
                break;
            case 'u':
                clientPl.dir = Direction::up;
                break;
            default: //case 'd'
                clientPl.dir = Direction::down;
                break;
        }
    }

    void sendDirection() {
        std::string dir = "d\n";
        switch (clientPl.dir) {
            case Direction::left:
                dir = "l\n";
                break;
            case Direction::right:
                dir = "r\n";
                break;
            case Direction::up:
                dir = "u\n";
                break;
            default: //case Direction::down:
                break;
        }
        boost::asio::write(socket, boost::asio::buffer(dir));
    }

    void updatePosition(Player pl) {
        switch (pl.dir) {
            case Direction::left:
                pl.position.first == 0u ? pl.position.first = fieldWidth - 1 : pl.position.first --;
                break;
            case Direction::right:
                pl.position.first == fieldWidth - 1 ? pl.position.first = 0 : pl.position.first ++;
                break;
            case Direction::up:
                pl.position.second == 0 ? pl.position.second = fieldHeight - 1 : pl.position.second --;
                break;
            case Direction::down:
                pl.position.second == fieldHeight - 1 ? pl.position.second = 0 : pl.position.second ++;
                break;
            default:
                break;
        }
    }

    bool checkPosition() {
        if (serverPl.position == clientPl.position)
            return true;

        if (field[serverPl.position.first][serverPl.position.second] == 2u)
            return true;

        if (field[clientPl.position.first][clientPl.position.second] == 1u)
            return true;

        return false;
    }

    void updateField() {
        field[serverPl.position.first][serverPl.position.second] = 1u;
        field[clientPl.position.first][clientPl.position.second] = 2u;
    }

    void doStep() {
        updatePosition(serverPl);
        updatePosition(clientPl);

        isEnded = checkPosition();

        updateField();
    }

public:
    int startGame() {
        if (connectionFailure) {
            return connectionFailure;
        }

        background.setPosition(0.0, 0.0);

        auto timePoint = std::chrono::steady_clock::now();

        while(!isEnded && window.isOpen()) {
            Event event{};

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            window.clear();

            draw();

            readKey();

            if (std::chrono::steady_clock::now() - timePoint > delay) {
                serverPl.dir = currentDir;
                acceptDirection();
                sendDirection();
                doStep();

                timePoint = std::chrono::steady_clock::now();
            }
        }

        if (isEnded) {
            Texture rawEndBackground;
            rawEndBackground.loadFromFile("../tron/images/end.jpg");
            Sprite endBackground(rawEndBackground);
            window.clear();
            endBackground.setPosition(0.0, 0.0);
            window.draw(background);
            window.display();

            Event event{};

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }
        }

        return 0;
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

private:
    const std::string gameName = "TRON";
    inline static const std::size_t width = 600u;
    inline static const std::size_t height = 480u;
    Sprite background;
    RenderWindow window;

private:
    inline static const std::size_t cellSize = 3u;
    inline static const std::size_t fieldHeight = height / cellSize;
    inline static const std::size_t fieldWidth = width / cellSize;
    std::array<std::array<std::size_t, fieldHeight>, fieldWidth> field;

private:
    const std::chrono::microseconds delay;

private:
    bool isEnded = false;
    Player serverPl{{10., 10.}, Direction(0u), {204, 102, 255}};
    Direction currentDir;
    Player clientPl{{fieldWidth - 10., fieldHeight - 10.}, Direction(0u), {255, 102, 0}};
};

int main() {
    Texture rawBackground;
    rawBackground.loadFromFile("../tron/images/background.jpg");
    Sprite background(rawBackground);

    auto retVal = 0;

    retVal = TronMaker(background).startGame();

    system("pause");

    return retVal;
}