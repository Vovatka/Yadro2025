#include <ComputerClub.hpp>
#include <iostream> 
#include <fstream> 
#include <sstream>
#include <regex>


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
    int tabels = _validateTabels(line);
    if (!tabels) {
        std::cerr << line << std::endl;
        return false;
    }

    // узнаем время
    std::getline(_data, line);
    std::istringstream tss{line};
    std::string t1, t2;
    tss >> t1; tss >> t2;
    auto openTime = _validateTime(t1);
    auto closeTime = _validateTime(t2);
    if (!openTime || !closeTime) {
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

    return true;
}

std::optional<std::chrono::minutes> ComputerClub::_validateTime(const std::string& str) {
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


int ComputerClub::_validateTabels(const std::string& str) {
    try {
        int tabels = std::stoi(str);
        if (tabels <= 0) {
            throw std::invalid_argument("");
        }
        return tabels;
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

ComputerClub::~ComputerClub() {
    _data.close();
}
