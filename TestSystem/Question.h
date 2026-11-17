#pragma once

#include <array>

using namespace std;

struct Question
{
    string text;
    array<string, 4> answers;
    int correct;
};