/**
    \brief Класс компьютерного клуба.

    Данный класс является прототипом системы, которая следит за работой компьютерного клуба, обрабатывает события и подсчитывает выручку за день и время занятости каждого стола.
*/

#pragma once
// Следует использовать Pimpl
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

class ComputerClub {
public:
    /**
    \brief Конструктор с параметром
    \param file - имя файла с входными данными
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
    \return true, если цикл пройден полностью, 
    иначе — false
    */
    bool StartLoop();

private:
    std::string _fileName; //!< Имя файла
    std::fstream _data; //!< Данные из файла
    std::chrono::minutes _openTime; //!< Время открытия
    std::chrono::minutes _closeTime; //!< Время закрытия
    //! Посетители, пришедшие в клуб
    std::unordered_set<std::string> _entered_visitors;
    //! Посетители, ожидающие свободного стола
    std::queue<std::string> _waiting_visitors;
    //! Посетители за столами
    std::map<std::string, Tuple> _visitors;
    //! Занятые столы
    std::map<int, std::string> _tables;
    std::map<int, Tuple> total; //!< Выручка со столов

    /**
    \brief Устанавливает время работы клуба
    \param line - строка с временем открытия и закрытия
    \return true — если формат времени верный, иначе — false
    */
    bool _setTime(std::string& line);

    /**
    \brief Преобразует строку в формате "HH:MM" в количество минут
    \param str - строка с временем
    \return nullopt — если ошибка в формате, иначе — количество минут в chrono::minutes
    */
    std::optional<std::chrono::minutes> _getMinutes(const std::string& str);

    /**
    \brief Проверка формата имени клиента
    \param str - строка
    */
    bool _isValidName(const std::string& str);

    /**
    \brief Проверка количества столов
    Предполагается, что количество столов > 0
    \param str - строка
    */
    int _validateTables(const std::string& str);

    /**
    \brief Проверка цены за час
    Предполагается, что цена за час > 0
    \param str - строка
    */
    float _validatePrice(const std::string& str);

    /**
    \brief Проверка ID клиента
    \param str - строка
    */
    int _validateID(const std::string& str);

    /**
    \brief Проверка номера стола, за который 
    хочет сесть посетитель
    \param str - строка
    \param tables - количество столов в клубе
    */
    int _validateTable(const std::string& str, int tables);

    /**
    \brief Вывод сообщения об ошибке
    \param time - время ошибки
    \param message - текст ошибки
    */
    void printError(const std::string& time, const std::string message) const;

    /**
    \brief Перевод chrono::minutes в строку
    \param minutes - количество минут
    \return строка в формате "HH:MM"
    */
    std::string _minutesToString(const std::chrono::minutes& minutes) const;
};
