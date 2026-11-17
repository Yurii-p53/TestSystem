#pragma once

#include <vector>
#include "Question.h"

using namespace std;

struct Test
{
    string category;
    string name;
    vector<Question> questions;
};