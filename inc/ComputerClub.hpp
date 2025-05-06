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
    @return - true если цикл пройден полностью, 
    иначе - false
    */
    bool StartLoop();
private:
    std::string _fileName; //!< имя файла
    std::fstream _data; //!< данные из файла
    /**
    \brief Функция для получения кол-ва минут из
    строки в формате "HH:MM"
    \tparam str - время в формате строки
    @return nullopt - если есть ошибка в формате, 
    иначе - кол-во минут в chrono::minutes
    */
    std::optional<std::chrono::minutes> _validateTime(const std::string& str);
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
    int _validateTabels(const std::string& str);
    /**
    \brief Функция для проверки цены за час
    Подразумевается, что цена за час > 0
    \tparam str - строка
    */
    float _validatePrice(const std::string& str);
};
