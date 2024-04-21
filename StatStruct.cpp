// StatStruct.cpp : Программа работы с ведомостью выплат с использованием структур.

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <algorithm>

// Создаём структуру
struct line
{
    std::string firstName;
    std::string secondName;
    std::string date;
    float amount;
};

// Функция ввода даты и проверка ввода на корректность.
// Скопировал из предыдущего модуля.
std::string inputDate()
{
    int day, month, year;
    std::string result = "NULL";
    std::cout << "Enter a date in the format dd.mm.yyyy: ";
    while (result == "NULL")
    {
        std::cin >> day;           // Считываем день
        if (std::cin.get() != '.') // Проверяем наличие '.' в качестве разделителя
        {
            std::cerr << "Expected '.'. Repeat input: ";
            std::cin.clear();
            std::cin.ignore();
        }
        std::cin >> month;         // Считываем месяц
        if (std::cin.get() != '.') // Проверяем наличие '.' в качестве разделителя
        {
            std::cerr << "Expected '.'. Repeat input: ";
            std::cin.clear();
            std::cin.ignore();
        }
        std::cin >> year;          // Считываем год
        // Проверяем корректность введённой даты, включая проверку на високосный год
        if (year > 0)
        {
            if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 0 && day < 32)
                result = std::to_string(day) + '.' + std::to_string(month) + '.' + std::to_string(year);
            else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 0 && day < 31)
                result = std::to_string(day) + '.' + std::to_string(month) + '.' + std::to_string(year);
            else if (month == 2)
                if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0) && day > 0 && day < 30)
                    result = std::to_string(day) + '.' + std::to_string(month) + '.' + std::to_string(year);
                else if (day > 0 && day < 29)
                    result = std::to_string(day) + '.' + std::to_string(month) + '.' + std::to_string(year);
                else
                {
                    std::cerr << "Incorrect date. Repeat input: ";
                    std::cin.clear();
                    std::cin.ignore();
                }
            else
            {
                std::cerr << "Incorrect date. Repeat input: ";
                std::cin.clear();
                std::cin.ignore();
            }
        }
        else
        {
            std::cerr << "Incorrect date. Repeat input: ";
            std::cin.clear();
            std::cin.ignore();
        }
    }
    return result;
}

// Эта функция позволяет вводить имя и фамилию или bye для выхода из программы.
// Так же проверяется корректность ввода, например нельзя ввести M@ikl или Sega2,
// но можно ввести CharlesII
// То же скопировал из предыдущего модуля.
std::string inputString(std::string title)
{
    std::string inStr;
    bool fail;
    std::cout << "Enter a " << title << " or 'BYE' for exit: ";
    do
    {
        fail = false;
        std::cin >> inStr;
        for (int i = 0; !fail && i < inStr.length(); i++)
        {
            if (!isalpha(inStr[i]))
            {
                fail = true;
                std::cout << "Input incorrect " << title << ". Repeat input: ";
            }
        }
    } while (fail);
    return inStr;
}

// Функция ввода суммы, можно с точкой
float inputPayment()
{
    std::string inStr;
    bool fail;
    std::cout << "Enter a payment: ";
    do
    {
        fail = false;
        std::cin >> inStr;
        for (int i = 0; !fail && i < inStr.length(); i++)
        {
            if (!isdigit(inStr[i]) && inStr[i] != '.')
            {
                fail = true;
                std::cout << "Input incorrect. Repeat input: ";
            }
        }
    } while (fail);
    return std::stof(inStr);
}

// Функция записи структуры в файл.
void saveString(std::ofstream& file, line& records)
{
    file << records.firstName << ' ';
    file << records.secondName << ' ';
    file << records.date << ' ';
    file << records.amount << '\n';
}

// Функция чтения данных из файла
void loadString(std::ifstream& file, line& records)
{   // Инициализируем структуру, т.к. если достигнут конец файла, то при
    // чтении значение переменной не будет изменено, а для нас это важно.
    // Что бы не выводить некорректные данные.
    records.firstName = "N/A";
    records.secondName = "N/A";
    records.date = "N/A";
    records.amount = 0.0;
    file >> records.firstName;
    file >> records.secondName;
    file >> records.date;
    file >> records.amount;
}

// Главная функция. Здесь организовано консольное меню для работы с ведомостью.
int main()
{
	std::string cmd, temp;
    bool work = true;
    line records;
    std::ifstream src;
    std::ofstream dst;
    while(work)
    {   // Создание меню
        std::cout << "\nMENU :\nADD for add string to statement\nLIST for display statement\nBYE for exit\nEnter command: ";
        std::cin >> cmd;
        // Для корректного выполнения команды переведём все буквы в заглавные
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
        // Если была введена команда BYE выходим из программы.
        if (cmd == "BYE")
        {
            std::cout << "Thank you. Bye.\n";
            work = false;
        }
        // Если введена команда ADD, то вводим с клавиатуры ИМЯ, ФАМИЛИЮ и т.д.
        // После корректного ввода записываем в файл.
        else if (cmd == "ADD")
        {
            dst.open("statement.txt", std::ios::app);
            // Открываем файл и проверяем успешность операции.
            // В случае неуспеха выводится соответствующее сообщение.
            if (dst.is_open())
            {
                std::cout << "I add record into statement ... now.\n";
                // Вводим ИМЯ
                records.firstName = inputString("FIRST NAME");
                temp = records.firstName;
                std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
                // Если вместо имени введена команда BYE, то выходим из программы.
                if (temp == "BYE")
                {
                    std::cout << "Thank you. Bye.\n";
                    work = false;
                }
                else
                {   // Вводим ФАМИЛИЮ
                    records.secondName = inputString("SECOND NAME");
                    temp = records.secondName;
                    std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
                    // Если вместо фамилии введена команда BYE, то выходим из программы.
                    if (temp == "BYE")
                    {
                        std::cout << "Thank you. Bye.\n";
                        work = false;
                    }
                    else
                    {   // Вводим дату выплаты и сумму. Тут выйти по команде BYE не реализовано.
                        records.date = inputDate();
                        records.amount = inputPayment();
                        // Сохраняем данные в файл.
                        saveString(dst, records);
                    }
                }
                dst.close();
            }
            else
                std::cout << "File is not exist.\n";
        }
        else if (cmd == "LIST")
        {   // По команде LIST отображаем содержимое ведомости.
            std::cout << "Display records from statement:\n";
            src.open("statement.txt");
            // Открываем файл на чтение с проверкой.
            // Если файл не открыт, то считается, что он не существует в текущем каталоге.
            if (src.is_open())
            {
                while (!src.eof())
                {
                    loadString(src, records);
                    // Если поле ИМЯ неопределено, то считаем, что и другие поля тоже неопределены,
                    // Либо строка не полная, либо достигнут конец файла.
                    if(records.firstName != "N/A")
                        std::cout << records.firstName << ' ' << records.secondName << ' ' << records.date << ' ' << records.amount << '\n';
                }
                src.close();
            }
            else
                std::cout << "File is not exist in current directory.\n";
        }
        else
            std::cout << "Unknown enter. Repeat, please.\n";
    }
    return 0;
}

