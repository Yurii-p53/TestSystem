#pragma once

#include <iostream>
#include <fstream>
#include "User.h"
#include "func.h"
#include "md5.h"

using namespace std;

class Registration
{
public:

    static bool loginexists(string login, string filename)
    {
        ifstream file(filename);

        if (!file.is_open())
        {
            return false;
        }

        login = md5(login);

        User user;

        while (getline(file, user.name))
        {
            if (filename == "users.txt")
            {
                if (!getline(file, user.address)) break;
                if (!getline(file, user.phone)) break;
            }

            if (!getline(file, user.login)) break;
            if (!getline(file, user.password)) break;

            if (user.login == login)
            {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    static User registration()
    {
        clear();

        User user;

        cout << "\n========== РЕГИСТРАЦИЯ ==========\n";

        user.name = readtext("ФИО: ");
        user.address = readtext("Адрес: ");
        user.phone = readtext("Телефон: ");

        while (true)
        {
            string login = readtext("Логин: ");

            if (login == "")
            {
                cout << "Логин не может быть пустым!\n";
            }
            else if (loginexists(login, "users.txt"))
            {
                cout << "Такой логин уже существует!\n";
            }
            else
            {
                user.login = md5(login);
                break;
            }
        }

        string password = readtext("Пароль: ");

        user.password = md5(password);

        ofstream file("users.txt", ios::app);

        file << user.name << endl;
        file << user.address << endl;
        file << user.phone << endl;
        file << user.login << endl;
        file << user.password << endl;

        file.close();

        cout << "\nРегистрация прошла успешно!\n";

        return user;
    }

    static User adminregistration()
    {
        clear();

        User admin;

        cout << "\n========== РЕГИСТРАЦИЯ АДМИНИСТРАТОРА ==========\n";

        admin.name = readtext("ФИО: ");

        while (true)
        {
            string login = readtext("Логин: ");

            if (login == "")
            {
                cout << "Логин не может быть пустым!\n";
            }
            else if (loginexists(login, "admins.txt"))
            {
                cout << "Такой логин уже существует!\n";
            }
            else
            {
                admin.login = md5(login);
                break;
            }
        }

        string password = readtext("Пароль: ");

        admin.password = md5(password);

        ofstream file("admins.txt", ios::app);

        file << admin.name << endl;
        file << admin.login << endl;
        file << admin.password << endl;

        file.close();

        cout << "\nАдминистратор зарегистрирован!\n";

        return admin;
    }
};