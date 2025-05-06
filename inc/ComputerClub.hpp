/**
    \brief Класс компьютерного клуба.

    Данный класс является прототипом системы, которая следит за работой компьютерного клуба, обрабатывает события и подсчитывает выручку за день и время занятости каждого стола.
*/


#pragma once
// надо бы pimpl
#include <string>
#include <fstream>
#include <chrono>
#include <optional>
#include <map>
#include <unordered_set>
#include <queue>
#include <tuple>

//! Кортеж для хранения номера стола и времени, когда этот стол был занят в последний раз
using Tuple = std::tuple<int, std::chrono::minutes>; 
//! Кортеж для хранения имени посетителя, и времени, когда он встал в очередь
using Waiting = std::tuple<std::string, std::chrono::minutes>;


class ComputerClub {
public:
    /**
    \brief Конструктор с параметрами
    \tparam file - имя файла с входными данными
    */
    ComputerClub(const std::string& file);

    ComputerClub(const ComputerClub& other) = delete;
    ComputerClub& operator=(const ComputerClub& other) = delete;
    ComputerClub(ComputerClub&& other) = delete;
    ComputerClub& operator=(ComputerClub&& other) = delete;

    /**
    \brief Деструктор
    */
    ~ComputerClub();
    /**
    \brief Основной цикл
    @return true если цикл пройден полностью, 
    иначе - false
    */
    bool StartLoop();
private:
    std::string _fileName; //!< имя файла
    std::fstream _data; //!< данные из файла
    std::chrono::minutes _openTime; //!< Время открытия
    std::chrono::minutes _closeTime; //!< Время закрытия
    //! Посетители, пришедшие в клуб
    std::unordered_set<std::string> _entered_visitors;
    //! Посетители, ожидающие свободного стола
    std::queue<Waiting> _waiting_visitors;
    //! Посетители за столами
    std::map<std::string, Tuple> _visitors;
    //! Занятые столы
    std::map<int, std::string> _tables;
    std::map<int, Tuple> total; //<! Выручка со столов

    /**
    \brief Функция для установки времени работы клуба
    в минутах (сохраняет в поля _openTime и _closeTime)
    \tparam line - строка с временем открытия и закрытия
    @return true - если формат времени верный, иначе - false
    */
    bool _setTime(std::string& line);
    /**
    \brief Функция для получения кол-ва минут из
    строки в формате "HH:MM"
    \tparam str - время в формате строки
    @return nullopt - если есть ошибка в формате, 
    иначе - кол-во минут в chrono::minutes
    */
    std::optional<std::chrono::minutes> _getMinutes(const std::string& str);
    /**
    \brief Функция для проверки формата имени клиента
    \tparam str - строка
    */
    bool _isValidName(const std::string& str);
    /**
    \brief Функция для проверки кол-во столов
    Подразумевается, что кол-во столов > 0
    \tparam str - строка
    */
    int _validateTables(const std::string& str);
    /**
    \brief Функция для проверки цены за час
    Подразумевается, что цена за час > 0
    \tparam str - строка
    */
    float _validatePrice(const std::string& str);
    /**
    \brief Функция для проверки ID клиента
    \tparam str - строка
    */
    int _validateID(const std::string& str);
    /**
    \brief Функция для проверки стола, за который 
    хочет сесть посетитель
    \tparam str - строка
    \tparam tables - кол-во столов в клубе
    */
    int _validateTable(const std::string& str, int tables);
    /**
    \brief Функция для печати ошибки
    */
    void printError(const std::string& time, const std::string message) const;
    /**
    \brief Функция для перевода chrono::minutes в string
    */
    std::string _minutesToString(const std::chrono::minutes& minutes) const;
};
