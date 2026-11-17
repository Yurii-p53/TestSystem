
#pragma once

#include <vector>
#include <string>
#include "User.h"
#include "md5.h"

using namespace std;

void addinituser(vector<User>& users, string name, string login)
{
    string hashlogin = md5(login);

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].login == hashlogin)
        {
            return;
        }
    }

    User user;

    user.name = name;
    user.address = "Николаев";
    user.phone = "0991234567";
    user.login = hashlogin;
    user.password = md5("12345");

    users.push_back(user);
}

void initusers(vector<User>& users)
{
    addinituser(users, "Иванов Иван Иванович", "ivanov");
    addinituser(users, "Петров Петр Петрович", "petrov");
    addinituser(users, "Сидоров Алексей Сергеевич", "sidorov");
    addinituser(users, "Коваленко Максим Андреевич", "kovalenko");
    addinituser(users, "Шевченко Дмитрий Олегович", "shevchenko");
    addinituser(users, "Бондаренко Артем Игоревич", "bondarenko");
    addinituser(users, "Мельник Андрей Викторович", "melnik");
    addinituser(users, "Ткаченко Никита Романович", "tkachenko");
    addinituser(users, "Кравченко Денис Павлович", "kravchenko");
    addinituser(users, "Олейник Владислав Сергеевич", "oleinik");
}
