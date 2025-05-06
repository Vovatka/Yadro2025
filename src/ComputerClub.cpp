#include <ComputerClub.hpp>
#include <iostream> 
#include <fstream> 
#include <sstream>
#include <regex>
#include <cmath>
#include <iomanip>


ComputerClub::ComputerClub(const std::string& file): _fileName(file) {
    _data.open(file, _data.in);
    if (!_data.is_open()) {
        _data.close();
        throw std::runtime_error("Can not open file");
    }
}


bool ComputerClub::StartLoop() {
    std::string line;


    // узнаем кол-во столов
    std::getline(_data, line);
    int tables = _validateTables(line);
    if (!tables) {
        std::cerr << line << std::endl;
        return false;
    }
    for (int i = 1; i <= tables; i++) {
        total[i] = Tuple(0, *_getMinutes("00:00"));
    }

    // узнаем время работы
    if (!_setTime(line)) {
        std::cerr << line << std::endl;
        return false;
    }
    

    // узнаем цену стола за час
    std::getline(_data, line);
    int price = _validatePrice(line);
    if (!price) {
        std::cerr << line << std::endl;
        return false;
    }

    std::string word;
    int k = 0, id = 0, table = tables;
    auto prevTime = _getMinutes("00:00");
    std::vector<std::string> event;
    std::cout << _minutesToString(_openTime) << std::endl;
    while (std::getline(_data, line)) {
        std::istringstream iss{line};
        while (iss >> word) {
            event.push_back(word);
        }
        auto time = _getMinutes(event[0]);
        id = _validateID(event[1]);
        std::string client = event[2];

        // проверяем формат данных о клиенте
        if (id == 2) {
            table = _validateTable(event[3], tables);
        }

        if (!(time && id && table && _isValidName(client) && event.size() <= 4 && prevTime < *time)) {
            std::cerr << line << std::endl;
            return false;
        }

        std::cout << line << std::endl;

//         ID 1. Клиент пришел
// Формат: <время> 1 <имя клиента>
// Если клиент уже в компьютерном клубе, генерируется ошибка "YouShallNotPass"
// Если клиент пришел в нерабочие часы, тогда "NotOpenYet"

        if (id == 1) {
            if (_openTime > time.value()) {
                printError(event[2], "NotOpenYet");
            }
            else if (_entered_visitors.contains(client)) {
                printError(event[2], "YouShallNotPass"); 
            }
            else {
                _entered_visitors.insert(client);
            }
        }

        else if (id == 2) {
            if (_tables.contains(table)) {
                printError(event[2], "PlaceIsBusy");
            }
            else if (!_entered_visitors.contains(client)) {
                printError(event[2], "ClientUnknown");
            }
            else {
                _tables[table] = client;
                _visitors[client] =  Tuple{table, *time};
                std::cout << event[2] << " " << 12 << " " << client << " " << table << std::endl;
            }
        }


//         ID 3. Клиент ожидает
// Формат: <время> 3 <имя клиента>
// Если в клубе есть свободные столы, то генерируется ошибка "ICanWaitNoLonger!".
// Если в очереди ожидания клиентов больше, чем общее число столов, то клиент уходит и генерируется событие ID 11.

        else if (id  == 3) {
            if (_tables.size() < tables) {
                printError(event[2], "ICanWaitNoLonger!");
            }
            else if (_waiting_visitors.size() > tables) {
                std::cout << client[2] << " " << 11 <<  " " << client << std::endl;
                _entered_visitors.erase(client);
            }
            else {
                _waiting_visitors.push(Waiting{client, *time});
            }
        }

// ID 4. Клиент ушел
// Формат: <время> 4 <имя клиента>
// Если клиент не находится в компьютерном клубе, генерируется ошибка "ClientUnknown".
// Когда клиент уходит, стол, за которым он сидел освобождается и его занимает первый клиент из очереди ожидания (ID 12).

        else {
            if (!_entered_visitors.contains(client)) {
                printError(event[2], "ClientUnknown");
            }
            else {
                auto [place1, t] = _visitors[client];
                auto [all_price, all_time] = total[place1];
                all_time += t;
                double hours = std::chrono::duration<double, std::ratio<3600>>(t).count();
                all_price += static_cast<int>(std::ceil(hours)) * price;
                total[place1] = Tuple(all_price, all_time);

                _visitors.erase(client);
                auto [wclient, mins] = _waiting_visitors.front();
                _waiting_visitors.pop();
                std::cout << event[2] << " " << 11 << " " << client << std::endl;
                _visitors[wclient] = Tuple(place1, mins);
                _tables[place1] = wclient;
                std::cout << event[2] << " " << 12 << " " << wclient << " " << place1 << std::endl;
            }
        }

        event.clear();
        prevTime = *time;
    }

    std::cout << _minutesToString(_closeTime) << std::endl;
    for(auto&& [client, tuple]: _visitors) {
        auto [place1, t] = tuple;
        auto [all_price, all_time] = total[place1];
        all_time += t;
        int total_minutes = t.count();
        all_price += static_cast<int>(std::ceil(total_minutes / 60.0));
        std::cout << _minutesToString(t) << " " << 11 << " " << client << std::endl;
        total[place1] = Tuple(all_price, all_time);
    }

    while (!_waiting_visitors.empty()) {
        auto [client, time] = _waiting_visitors.front();
        std::cout << _minutesToString(time) << " " << 11 << " " << client << std::endl;
        _waiting_visitors.pop();
    }

    // for (auto&& [key, value]: total) {
    //     auto [all_price, all_time] = value;
    //     std::cout << key << all_price << _minutesToString(all_time);
    // }
    return true;
}

bool ComputerClub::_setTime(std::string& line) {
    std::getline(_data, line);
    std::istringstream tss{line};
    std::string t1, t2;
    tss >> t1; tss >> t2;
    auto openTime = _getMinutes(t1);
    auto closeTime = _getMinutes(t2);
    if (openTime && closeTime) {
        _openTime = *openTime;
        _closeTime = *closeTime;
        return true;
    };
    return false;
}

std::optional<std::chrono::minutes> ComputerClub::_getMinutes(const std::string& str) {
    std::regex time_r(R"(^\d{2}:\d{2}$)");
    if ( !std::regex_match(str, time_r) ) {
        return std::nullopt;
    }

    int h = std::stoi(str.substr(0, 2));
    int m = std::stoi(str.substr(3, 2));
    if ((h >= 0 && h <= 23) && (m >= 0 && m <= 59)) {
        return std::chrono::hours(h) + std::chrono::minutes(m);
    };
    return std::nullopt;
}

bool ComputerClub::_isValidName(const std::string& str) {
    std::regex name_r(R"(^[a-z0-9_-]+$)");
    return std::regex_match(str, name_r);
}


int ComputerClub::_validateTables(const std::string& str) {
    try {
        int tables = std::stoi(str);
        if (tables <= 0) {
            throw std::invalid_argument("");
        }
        return tables;
    }
    catch (const std::invalid_argument& e) {
        return 0;
    }
    catch (const std::out_of_range& e) {
        return 0;
    }
}


float ComputerClub::_validatePrice(const std::string& str) {
    try {
        float price = std::stof(str);
        if (price <= 0) {
            throw std::invalid_argument("");
        }
        return price;
    }
    catch (const std::invalid_argument& e) {
        return 0.0f;
    }
    catch (const std::out_of_range& e) {
        return 0.0f;
    }
}

int ComputerClub::_validateID(const std::string& str) {
    try {
        int id = std::stoi(str);
        if (id <= 0 || id > 4) {
            throw std::invalid_argument("");
        }
        return id;
    }
    catch (const std::invalid_argument& e) {
        return 0;
    }
    catch (const std::out_of_range& e) {
        return 0;
    }
}

int ComputerClub::_validateTable(const std::string& str, int tables) { 
    int t = _validateTables(str);
    return t && t <= tables ? t : 0;
}

void ComputerClub::printError(const std::string& time, const std::string message) const {
    std::cout << time << " " << 13 << " " << message << std::endl;
}

std::string ComputerClub::_minutesToString(const std::chrono::minutes& minutes) const {
    int total_minutes = minutes.count();
    
    int hours = total_minutes / 60;
    int mins = total_minutes % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours
        << ":"
        << std::setw(2) << std::setfill('0') << mins;

    return oss.str();
}

ComputerClub::~ComputerClub() {
    _data.close();
}


