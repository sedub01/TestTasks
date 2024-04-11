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

// Функция для выполнения операции $get или $set на сервере
void performOperation(tcp::socket& socket, const string& operation) {
    boost::asio::write(socket, boost::asio::buffer(operation + "\n"));
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, "\n");
    string response = boost::asio::buffer_cast<const char*>(buffer.data());
    response.pop_back(); // Удаляет '\n'
    cout << "Server response: " << response << endl;
}

int main() {
    boost::asio::io_service service;
    tcp::resolver resolver(service);
    tcp::resolver::query query("localhost", PORT_NUMBER);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::socket socket(service);

    try {
        connect(socket, endpoint_iterator);
        srand(time(NULL));
        for (int i = 0; i < 10000; ++i) {
            int operationType = rand() % 100;
            string key = "key" + std::to_string(rand() % 10); // Случайный ключ от "key0" до "key9"
            string operation;
            // 99% вероятность для $get и 1% для $set
            if (operationType < 99) {
                operation = GET_KEY + " " + key;
            } else {
                string value = "value" + std::to_string(rand() % 100);
                operation = SET_KEY + " " + key + "=" + value;
            }
            cout << "Sending operation: " << operation << endl;
            performOperation(socket, operation);
        }

        cout << "The end!" << endl;
        socket.shutdown(tcp::socket::shutdown_both);
        socket.close();
    } catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
