/**
    \brief Класс компьютерного клуба.

    Данный класс является прототипом системы, которая следит за работой компьютерного клуба, обрабатывает события и подсчитывает выручку за день и время занятости каждого стола.
*/


#pragma once
#include <string>

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
    \brief Деструктор по умолчанию
    */
    ~ComputerClub() = default;
    /**
    \brief Основной цикл
    */
    void StartLoop();
private:
    

};
