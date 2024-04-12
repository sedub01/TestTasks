#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <thread>
#include <map>
#include <mutex>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::ifstream;
using std::ofstream;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::atomic;

const int PORT_NUMBER = 1234;
const string GET_KEY = "$get";
const string SET_KEY = "$set";
const char* FILENAME = "config.txt";

class Server {
public:
    Server() : total_requests(0), requests_last_5_seconds(0) {}

    void start() {
        readConfigFile();
        // Запуск потока для вывода статистики
        thread statisticsThread(&Server::printStatistics, this);

        io_service service;
        tcp::endpoint endpoint(tcp::v4(), PORT_NUMBER);
        tcp::acceptor acceptor(service, endpoint);

        cout << "Server started" << endl;

        while (true) {
            tcp::socket socket(service);
            acceptor.accept(socket);
            thread(&Server::handleClient, this, std::move(socket)).detach();
        }
    }

private:
    unordered_map<string, string> data;
    mutex dataMutex;
    atomic<int> total_requests;
    atomic<int> requests_last_5_seconds;

    // Функция для чтения конфигурационного файла
    void readConfigFile() {
        ifstream configFile(FILENAME);
        string key, value;
        while (configFile >> key >> value) {
            data[key] = value;
        }
        configFile.close();
    }

    // Функция для обновления конфигурационного файла на диске
    void updateConfigFile() {
        ofstream configFile(FILENAME);
        for (const auto& pair : data) {
            configFile << pair.first << " " << pair.second << endl;
        }
        configFile.close();
    }

    // Функция для обработки клиентских запросов
    void handleClient(tcp::socket socket) {
        try {
            while (true) {
                boost::asio::streambuf buffer;
                boost::asio::read_until(socket, buffer, "\n");
                string message = boost::asio::buffer_cast<const char*>(buffer.data());
                message.pop_back(); // Удаляет '\n'

                if (message.substr(0, 4) == GET_KEY) {
                    string key = message.substr(5);
                    string response;
                    lock_guard<mutex> guard(dataMutex);
                    const auto it = data.find(key);
                    response = it != data.end() ? it->second : "Key not found";
                    boost::asio::write(socket, boost::asio::buffer(response + "\n"));
                } else if (message.substr(0, 4) == SET_KEY) {
                    string key_value = message.substr(5);
                    string key = key_value.substr(0, key_value.find("="));
                    string value = key_value.substr(key_value.find("=") + 1);
                    lock_guard<mutex> guard(dataMutex);
                    data[key] = value;
                    updateConfigFile();
                    boost::asio::write(socket, boost::asio::buffer("Value set\n"));
                }

                //Увеличиваем счетчики
                total_requests++;
                requests_last_5_seconds++;
            }
        } catch (boost::system::system_error& e) {
            // Выводит сообщение об ошибке только если это не ошибка EOF
            if (e.code() != boost::asio::error::eof) {
                cout << "Exception in thread: " << e.what() << endl;
            }
        }
    }

    // Функция для вывода статистики
    void printStatistics() {
        while (true) {
            cout << "Total requests: " << total_requests << ", last 5 seconds: " << requests_last_5_seconds << endl;
            requests_last_5_seconds = 0; // Зануляет счетчик за последние 5 секунд
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
};


int main() {
    Server server;
    server.start();
    return 0;
}
