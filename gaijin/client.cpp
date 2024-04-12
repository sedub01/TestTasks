#include <iostream>
#include <boost/asio.hpp>
#include <cstdlib>
#include <ctime>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;

const string PORT_NUMBER = "1234";
const string GET_KEY = "$get";
const string SET_KEY = "$set";
const int ITERATIONS = 10000;
const string keys[] = {
    "inspection",
    "importance",
    "percentage",
    "fratricide",
    "assignment",
    "adjustment",
    "blackboard",
    "girlfriend",
    "philosophy",
    "literature"
};
const int SIZE = sizeof(keys) / sizeof(string);

class Client {
public:
    Client(io_service& service, const string& address, const string& port)
        : socket(service), resolver(service), endpoint_iterator(resolver.resolve({address, port})) {}

    void start() {
        connect();
        srand(time(NULL));
        for (int i = 0; i < ITERATIONS; ++i) {
            int operationType = rand() % 100;
            string key = keys[rand() % SIZE];
            string operation;
            // 99% вероятность для $get и 1% для $set
            if (operationType < 99) {
                operation = GET_KEY + " " + key;
            } else {
                string value = "value" + std::to_string(rand() % 100);
                operation = SET_KEY + " " + key + "=" + value;
            }
            cout << "Sending operation: " << operation << endl;
            performOperation(operation);
        }

        cout << "The end!" << endl;
    }

    void shutdown() {
        socket.shutdown(tcp::socket::shutdown_both);
        socket.close();
    }

private:
    tcp::socket socket;
    tcp::resolver resolver;
    tcp::resolver::iterator endpoint_iterator;

    void connect() {
        boost::asio::connect(socket, endpoint_iterator);
    }

    // Функция для выполнения операции $get или $set на сервере
    void performOperation(const string& operation) {
        boost::asio::write(socket, boost::asio::buffer(operation + "\n"));
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");
        string response = boost::asio::buffer_cast<const char*>(buffer.data());
        response.pop_back(); // Удаляет '\n'
        cout << "Server response: " << response << endl;
    }
};

int main() {
    try {
        boost::asio::io_service service;
        Client client(service, "localhost", PORT_NUMBER);
        client.start();
        client.shutdown();
    } catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
