#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <array>

#include "func.h"
#include "User.h"
#include "Question.h"
#include "Test.h"
#include "Result.h"
#include "Registration.h"
#include "md5.h"

using namespace std;

class TestingSystem
{
    User currentUser;

    vector<User> users;
    vector<User> admins;
    vector<Test> tests;
    vector<Result> results;

public:

    TestingSystem()
    {
        ifstream file("admins.txt");

        if (!file.is_open())
        {
            User admin;

            admin.name = "Главный администратор";
            admin.login = md5("admin");
            admin.password = md5("admin");

            ofstream save("admins.txt");

            save << admin.name << endl;
            save << admin.login << endl;
            save << admin.password << endl;

            save.close();
        }

        file.close();

        ifstream testfile("tests.txt");

        if (!testfile.is_open() || testfile.peek() == EOF)
        {
            testfile.close();
            createdefaulttests();
        }
        else
        {
            testfile.close();
        }
    }

    void run()
    {
        while (true)
        {
            clear();

            cout << "\n====================================\n";
            cout << "       СИСТЕМА ТЕСТИРОВАНИЯ\n";
            cout << "====================================\n\n";

            cout << "1. Войти как пользователь\n";
            cout << "2. Регистрация пользователя\n";
            cout << "3. Войти как администратор\n";
            cout << "4. Регистрация администратора\n";
            cout << "0. Выход\n\n";

            int choice = readnumber("Ваш выбор: ");

            if (choice == 1)
            {
                userlogin();
            }
            else if (choice == 2)
            {
                Registration::registration();
                pause();
            }
            else if (choice == 3)
            {
                adminenter();
            }
            else if (choice == 4)
            {
                Registration::adminregistration();
                pause();
            }
            else if (choice == 0)
            {
                break;
            }
        }
    }

private:

    bool finduser(string login, string password)
    {
        ifstream file("users.txt");

        if (!file.is_open())
        {
            return false;
        }

        User user;

        while (getline(file, user.name))
        {
            if (!getline(file, user.address)) break;
            if (!getline(file, user.phone)) break;
            if (!getline(file, user.login)) break;
            if (!getline(file, user.password)) break;

            if (user.login == login &&
                user.password == password)
            {
                currentUser = user;
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    void userlogin()
    {
        clear();

        cout << "\n========== ВХОД ПОЛЬЗОВАТЕЛЯ ==========\n";

        string login = md5(readtext("Логин: "));
        string password = md5(readtext("Пароль: "));

        if (finduser(login, password))
        {
            usermenu();
        }
        else
        {
            cout << "\nНеверный логин или пароль!\n";
            pause();
        }
    }

    void usermenu()
    {
        while (true)
        {
            clear();

            cout << "\n========== ЛИЧНЫЙ КАБИНЕТ ==========\n";
            cout << "Пользователь: " << currentUser.name << "\n\n";

            cout << "1. Пройти тест\n";
            cout << "2. История результатов\n";
            cout << "3. Мои данные\n";
            cout << "4. Моя статистика\n";
            cout << "0. Выход\n\n";

            int choice = readnumber("Ваш выбор: ");

            if (choice == 1)
            {
                choosetest();
            }
            else if (choice == 2)
            {
                showhistory();
                pause();
            }
            else if (choice == 3)
            {
                showmydata();
                pause();
            }
            else if (choice == 4)
            {
                showmystatistics();
                pause();
            }
            else if (choice == 0)
            {
                break;
            }
        }
    }

    void showmydata()
    {
        clear();

        cout << "\n========== МОИ ДАННЫЕ ==========\n\n";

        cout << "ФИО: " << currentUser.name << endl;
        cout << "Адрес: " << currentUser.address << endl;
        cout << "Телефон: " << currentUser.phone << endl;
        cout << "Логин: " << currentUser.login << endl;
    }

    void showhistory()
    {
        clear();

        cout << "\n========== ИСТОРИЯ РЕЗУЛЬТАТОВ ==========\n\n";

        ifstream file("results.txt");

        if (!file.is_open())
        {
            cout << "Результатов пока нет.\n";
            return;
        }

        Result result;

        bool found = false;

        string number;

        while (getline(file, result.login))
        {
            if (!getline(file, result.test)) break;
            if (!getline(file, number)) break;

            result.total = stoi(number);

            if (!getline(file, number)) break;
            result.correct = stoi(number);

            if (!getline(file, number)) break;
            result.mark = stoi(number);

            if (result.login == currentUser.login)
            {
                found = true;

                cout << "Тест: " << result.test << endl;
                cout << "Правильных ответов: "
                    << result.correct << " из "
                    << result.total << endl;

                printmark(result.mark);

                cout << endl;
            }
        }

        file.close();

        if (!found)
        {
            cout << "Результатов пока нет.\n";
        }
    }

    void printmark(int mark)
    {
        if (mark <= 5)
        {
            setcolor(LightRed, Blue);
        }
        else if (mark <= 8)
        {
            setcolor(Yellow, Blue);
        }
        else
        {
            setcolor(LightGreen, Blue);
        }

        cout << "Оценка: " << mark << " из 12";

        setcolor(White, Blue);

        cout << endl;
    }

    void showmystatistics()
    {
        clear();

        cout << "\n========== МОЯ СТАТИСТИКА ==========\n\n";

        ifstream file("results.txt");

        if (!file.is_open())
        {
            cout << "Результатов пока нет.\n";
            return;
        }

        Result result;

        string number;

        int count = 0;
        int sum = 0;
        int best = 0;
        int worst = 13;
        int correct = 0;

        while (getline(file, result.login))
        {
            if (!getline(file, result.test)) break;
            if (!getline(file, number)) break;

            result.total = stoi(number);

            if (!getline(file, number)) break;
            result.correct = stoi(number);

            if (!getline(file, number)) break;
            result.mark = stoi(number);

            if (result.login == currentUser.login)
            {
                count++;
                sum += result.mark;
                correct += result.correct;

                if (result.mark > best)
                {
                    best = result.mark;
                }

                if (result.mark < worst)
                {
                    worst = result.mark;
                }
            }
        }

        file.close();

        if (count == 0)
        {
            cout << "Вы еще не проходили тесты.\n";
            return;
        }

        cout << "Пройдено тестов: " << count << endl;
        cout << "Правильных ответов: " << correct << endl;
        cout << "Средняя оценка: " << (double)sum / count << endl;
        cout << "Лучшая оценка: " << best << endl;
        cout << "Худшая оценка: " << worst << endl;
    }

    void choosetest()
    {
        clear();

        cout << "\n========== СПИСОК ТЕСТОВ ==========\n\n";

        ifstream file("tests.txt");

        if (!file.is_open())
        {
            cout << "Тестов пока нет.\n";
            pause();
            return;
        }

        vector<string> names;
        vector<string> categories;

        string category;
        string name;
        string number;

        while (getline(file, category))
        {
            if (!getline(file, name)) break;
            if (!getline(file, number)) break;

            int count = stoi(number);

            names.push_back(name);
            categories.push_back(category);

            string line;

            for (int i = 0; i < count * 6; i++)
            {
                if (!getline(file, line))
                {
                    file.close();
                    return;
                }
            }
        }

        file.close();

        if (names.size() == 0)
        {
            cout << "Тестов пока нет.\n";
            pause();
            return;
        }

        for (int i = 0; i < names.size(); i++)
        {
            cout << i + 1 << ". "
                << categories[i] << " - "
                << names[i] << endl;
        }

        cout << "\n0. Назад\n";

        int choice = readnumber("\nВыберите тест: ");

        if (choice < 1 || choice > names.size())
        {
            return;
        }

        runtest(choice - 1);
    }

    void runtest(int testindex)
    {
        ifstream file("tests.txt");

        if (!file.is_open())
        {
            return;
        }

        string category;
        string name;
        string number;

        for (int i = 0; i <= testindex; i++)
        {
            if (!getline(file, category)) break;
            if (!getline(file, name)) break;
            if (!getline(file, number)) break;

            int count = stoi(number);

            if (i != testindex)
            {
                string line;

                for (int j = 0; j < count * 6; j++)
                {
                    getline(file, line);
                }
            }
            else
            {
                int correct = 0;
                int total = count;

                for (int j = 0; j < count; j++)
                {
                    Question question;

                    if (!getline(file, question.text)) break;

                    for (int k = 0; k < 4; k++)
                    {
                        if (!getline(file, question.answers[k])) break;
                    }

                    if (!getline(file, number)) break;

                    question.correct = stoi(number);

                    clear();

                    cout << "\nТест: " << name << endl;
                    cout << "Вопрос " << j + 1
                        << " из " << total << "\n\n";

                    cout << question.text << "\n\n";

                    for (int k = 0; k < 4; k++)
                    {
                        cout << k + 1 << ". "
                            << question.answers[k] << endl;
                    }

                    int answer = readnumber("\nВаш ответ: ");

                    if (answer == question.correct)
                    {
                        correct++;
                    }
                }

                file.close();

                int mark = 0;

                if (total > 0)
                {
                    mark = correct * 12 / total;
                }

                Result result;

                result.login = currentUser.login;
                result.test = name;
                result.total = total;
                result.correct = correct;
                result.mark = mark;

                ofstream save("results.txt", ios::app);

                save << result.login << endl;
                save << result.test << endl;
                save << result.total << endl;
                save << result.correct << endl;
                save << result.mark << endl;

                save.close();

                clear();

                cout << "\n========== РЕЗУЛЬТАТ ==========\n\n";

                cout << "Всего вопросов: " << total << endl;
                cout << "Правильных ответов: " << correct << endl;

                printmark(mark);

                showaverage();

                pause();

                return;
            }
        }

        file.close();
    }

    void showaverage()
    {
        ifstream file("results.txt");

        if (!file.is_open())
        {
            return;
        }

        Result result;
        string number;

        int sum = 0;
        int count = 0;

        while (getline(file, result.login))
        {
            if (!getline(file, result.test)) break;
            if (!getline(file, number)) break;

            result.total = stoi(number);

            if (!getline(file, number)) break;
            result.correct = stoi(number);

            if (!getline(file, number)) break;
            result.mark = stoi(number);

            if (result.login == currentUser.login)
            {
                sum += result.mark;
                count++;
            }
        }

        file.close();

        if (count > 0)
        {
            cout << "\nСредняя оценка за все тесты: "
                << (double)sum / count << endl;
        }
    }

    void createdefaulttests()
    {
        Test test;
        Question question;

        test.category = "Математика-Физика";
        test.name = "Математика-Физика";

        question.text = "Сколько будет 2 + 2?";
        question.answers = { "3", "4", "5", "6" };
        question.correct = 2;
        test.questions.push_back(question);

        question.text = "Сколько будет 5 * 5?";
        question.answers = { "10", "15", "20", "25" };
        question.correct = 4;
        test.questions.push_back(question);

        question.text = "Какое число является простым?";
        question.answers = { "4", "6", "7", "9" };
        question.correct = 3;
        test.questions.push_back(question);

        question.text = "Как обозначается скорость света?";
        question.answers = { "c", "v", "a", "F" };
        question.correct = 1;
        test.questions.push_back(question);

        question.text = "Как называется частица света?";
        question.answers = { "Электрон", "Протон", "Фотон", "Нейтрон" };
        question.correct = 3;
        test.questions.push_back(question);

        tests.push_back(test);

        savtests();
    }

    void loadusers()
    {
        users.clear();

        ifstream file("users.txt");

        User user;

        while (getline(file, user.name))
        {
            if (!getline(file, user.address)) break;
            if (!getline(file, user.phone)) break;
            if (!getline(file, user.login)) break;
            if (!getline(file, user.password)) break;

            users.push_back(user);
        }

        file.close();
    }

    void saveusers()
    {
        ofstream file("users.txt");

        for (int i = 0; i < users.size(); i++)
        {
            file << users[i].name << endl;
            file << users[i].address << endl;
            file << users[i].phone << endl;
            file << users[i].login << endl;
            file << users[i].password << endl;
        }

        file.close();
    }

    void loadadmins()
    {
        admins.clear();

        ifstream file("admins.txt");

        User admin;

        while (getline(file, admin.name))
        {
            if (!getline(file, admin.login)) break;
            if (!getline(file, admin.password)) break;

            admins.push_back(admin);
        }

        file.close();
    }

    void saveadmins()
    {
        ofstream file("admins.txt");

        for (int i = 0; i < admins.size(); i++)
        {
            file << admins[i].name << endl;
            file << admins[i].login << endl;
            file << admins[i].password << endl;
        }

        file.close();
    }

    void loadresults()
    {
        results.clear();

        ifstream file("results.txt");

        Result result;
        string number;

        while (getline(file, result.login))
        {
            if (!getline(file, result.test)) break;
            if (!getline(file, number)) break;

            result.total = stoi(number);

            if (!getline(file, number)) break;
            result.correct = stoi(number);

            if (!getline(file, number)) break;
            result.mark = stoi(number);

            results.push_back(result);
        }

        file.close();
    }

    void saveresults()
    {
        ofstream file("results.txt");

        for (int i = 0; i < results.size(); i++)
        {
            file << results[i].login << endl;
            file << results[i].test << endl;
            file << results[i].total << endl;
            file << results[i].correct << endl;
            file << results[i].mark << endl;
        }

        file.close();
    }

    void loadtests()
    {
        tests.clear();

        ifstream file("tests.txt");

        string number;

        while (true)
        {
            Test test;

            if (!getline(file, test.category)) break;
            if (!getline(file, test.name)) break;
            if (!getline(file, number)) break;

            int count = stoi(number);

            for (int i = 0; i < count; i++)
            {
                Question question;

                if (!getline(file, question.text)) break;

                for (int j = 0; j < 4; j++)
                {
                    getline(file, question.answers[j]);
                }

                getline(file, number);

                question.correct = stoi(number);

                test.questions.push_back(question);
            }

            tests.push_back(test);
        }

        file.close();
    }

    void savtests()
    {
        ofstream file("tests.txt");

        for (int i = 0; i < tests.size(); i++)
        {
            file << tests[i].category << endl;
            file << tests[i].name << endl;
            file << tests[i].questions.size() << endl;

            for (int j = 0; j < tests[i].questions.size(); j++)
            {
                file << tests[i].questions[j].text << endl;

                for (int k = 0; k < 4; k++)
                {
                    file << tests[i].questions[j].answers[k] << endl;
                }

                file << tests[i].questions[j].correct << endl;
            }
        }

        file.close();
    }

    void adminenter()
    {
        clear();

        cout << "\n========== ВХОД АДМИНИСТРАТОРА ==========\n";

        string login = md5(readtext("Логин: "));
        string password = md5(readtext("Пароль: "));

        loadadmins();

        for (int i = 0; i < admins.size(); i++)
        {
            if (admins[i].login == login &&
                admins[i].password == password)
            {
                adminmenu();
                return;
            }
        }

        cout << "\nНеверный логин или пароль!\n";
        pause();
    }

    void adminmenu()
    {
        while (true)
        {
            clear();

            cout << "\n========== ПАНЕЛЬ АДМИНИСТРАТОРА ==========\n\n";

            cout << "1. Список пользователей\n";
            cout << "2. Список тестов\n";
            cout << "3. Добавить тест\n";
            cout << "4. Добавить вопрос в тест\n";
            cout << "5. Общая статистика\n";
            cout << "6. Изменить оценку пользователя\n";
            cout << "7. Изменить пароль администратора\n";
            cout << "0. Выход\n\n";

            int choice = readnumber("Ваш выбор: ");

            if (choice == 1)
            {
                loadusers();
                loadresults();
                showusers();
                pause();
            }
            else if (choice == 2)
            {
                loadtests();
                showtests();
                pause();
            }
            else if (choice == 3)
            {
                loadtests();
                addtest();
            }
            else if (choice == 4)
            {
                loadtests();
                addquestion();
            }
            else if (choice == 5)
            {
                loadusers();
                loadadmins();
                loadtests();
                loadresults();
                showstatistics();
                pause();
            }
            else if (choice == 6)
            {
                loadusers();
                loadresults();
                setmark();
            }
            else if (choice == 7)
            {
                loadadmins();
                changeadminpassword();
            }
            else if (choice == 0)
            {
                break;
            }
        }
    }

    int getmark(string login)
    {
        int mark = -1;

        for (int i = 0; i < results.size(); i++)
        {
            if (results[i].login == login)
            {
                mark = results[i].mark;
            }
        }

        return mark;
    }

    void showusers()
    {
        clear();

        cout << "\n========== ПОЛЬЗОВАТЕЛИ ==========\n\n";

        if (users.size() == 0)
        {
            cout << "Пользователей пока нет.\n";
            return;
        }

        cout << "1. Обычный порядок\n";
        cout << "2. По фамилии А-Я\n";
        cout << "3. По оценкам от высокой к низкой\n";

        int choice = readnumber("\nСортировка: ");

        vector<User> sortedusers = users;

        if (choice == 2 || choice == 3)
        {
            for (int i = 0; i < sortedusers.size(); i++)
            {
                for (int j = i + 1; j < sortedusers.size(); j++)
                {
                    bool change = false;

                    if (choice == 2 &&
                        sortedusers[i].name > sortedusers[j].name)
                    {
                        change = true;
                    }

                    if (choice == 3 &&
                        getmark(sortedusers[i].login) <
                        getmark(sortedusers[j].login))
                    {
                        change = true;
                    }

                    if (change)
                    {
                        User temp = sortedusers[i];
                        sortedusers[i] = sortedusers[j];
                        sortedusers[j] = temp;
                    }
                }
            }
        }

        for (int i = 0; i < sortedusers.size(); i++)
        {
            cout << "\n" << i + 1 << ". "
                << sortedusers[i].name << endl;

            cout << "Адрес: " << sortedusers[i].address << endl;
            cout << "Телефон: " << sortedusers[i].phone << endl;

            int mark = getmark(sortedusers[i].login);

            if (mark == -1)
            {
                cout << "Оценка: нет результатов\n";
            }
            else
            {
                cout << "Последняя оценка: " << mark << "/12\n";
            }
        }
    }

    void showtests()
    {
        clear();

        cout << "\n========== ТЕСТЫ ==========\n\n";

        for (int i = 0; i < tests.size(); i++)
        {
            cout << i + 1 << ". "
                << tests[i].category << " - "
                << tests[i].name << endl;

            cout << "Количество вопросов: "
                << tests[i].questions.size() << "\n\n";
        }
    }

    void addtest()
    {
        clear();

        Test test;

        cout << "\n========== ДОБАВЛЕНИЕ ТЕСТА ==========\n\n";

        test.category = readtext("Категория: ");
        test.name = readtext("Название теста: ");

        tests.push_back(test);

        savtests();

        cout << "\nТест добавлен!\n";
        pause();
    }

    void addquestion()
    {
        clear();

        if (tests.size() == 0)
        {
            cout << "Сначала добавьте тест!\n";
            pause();
            return;
        }

        cout << "\n========== ВЫБОР ТЕСТА ==========\n\n";

        for (int i = 0; i < tests.size(); i++)
        {
            cout << i + 1 << ". "
                << tests[i].name << endl;
        }

        int choice = readnumber("\nВыберите тест: ");

        if (choice < 1 || choice > tests.size())
        {
            cout << "Неверный номер!\n";
            pause();
            return;
        }

        Question question;

        cout << "\n========== НОВЫЙ ВОПРОС ==========\n\n";

        question.text = readtext("Текст вопроса: ");

        for (int i = 0; i < 4; i++)
        {
            question.answers[i] = readtext(
                "Вариант " + to_string(i + 1) + ": "
            );
        }

        question.correct = readnumber("Номер правильного ответа: ");

        if (question.correct < 1 || question.correct > 4)
        {
            cout << "Неверный номер ответа!\n";
            pause();
            return;
        }

        tests[choice - 1].questions.push_back(question);

        savtests();

        cout << "\nВопрос добавлен!\n";
        pause();
    }

    void showstatistics()
    {
        clear();

        cout << "\n========== СТАТИСТИКА ==========\n\n";

        cout << "Пользователей: " << users.size() << endl;
        cout << "Администраторов: " << admins.size() << endl;
        cout << "Тестов: " << tests.size() << endl;
        cout << "Всего прохождений: " << results.size() << endl;

        if (results.size() > 0)
        {
            int sum = 0;

            for (int i = 0; i < results.size(); i++)
            {
                sum += results[i].mark;
            }

            cout << "Средняя оценка: "
                << (double)sum / results.size() << endl;
        }
    }

    void setmark()
    {
        clear();

        cout << "\n========== ИЗМЕНЕНИЕ ОЦЕНКИ ==========\n\n";

        if (users.size() == 0)
        {
            cout << "Пользователей нет!\n";
            pause();
            return;
        }

        for (int i = 0; i < users.size(); i++)
        {
            cout << i + 1 << ". " << users[i].name << endl;
        }

        int choice = readnumber("\nВыберите пользователя: ");

        if (choice < 1 || choice > users.size())
        {
            cout << "Неверный номер!\n";
            pause();
            return;
        }

        string login = users[choice - 1].login;

        vector<int> userresults;

        for (int i = 0; i < results.size(); i++)
        {
            if (results[i].login == login)
            {
                userresults.push_back(i);

                cout << userresults.size() << ". "
                    << results[i].test
                    << " | Оценка: "
                    << results[i].mark << "/12\n";
            }
        }

        if (userresults.size() == 0)
        {
            cout << "У пользователя пока нет результатов.\n";
            pause();
            return;
        }

        int resultnumber = readnumber("\nВыберите результат: ");

        if (resultnumber < 1 || resultnumber > userresults.size())
        {
            cout << "Неверный номер!\n";
            pause();
            return;
        }

        int mark = readnumber("Введите новую оценку (1-12): ");

        if (mark < 1 || mark > 12)
        {
            cout << "Оценка должна быть от 1 до 12!\n";
            pause();
            return;
        }

        results[userresults[resultnumber - 1]].mark = mark;

        saveresults();

        cout << "\nОценка изменена!\n";

        pause();
    }

    

    void changeadminpassword()
    {
        clear();

        cout << "\n========== СМЕНА ПАРОЛЯ ==========\n\n";

        string oldpassword = md5(readtext("Старый пароль: "));

        bool found = false;
        int adminindex = -1;

        for (int i = 0; i < admins.size(); i++)
        {
            if (admins[i].password == oldpassword)
            {
                found = true;
                adminindex = i;
                break;
            }
        }

        if (!found)
        {
            cout << "Неверный пароль!\n";
            pause();
            return;
        }

        string newpassword = readtext("Новый пароль: ");

        if (newpassword == "")
        {
            cout << "Пароль не может быть пустым!\n";
            pause();
            return;
        }

        admins[adminindex].password = md5(newpassword);

        saveadmins();

        cout << "\nПароль изменен!\n";
        pause();
    }
};