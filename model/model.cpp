//
// Created by user on 2022/10/11.
//

#include "model.hpp"

#include <sstream>
#include <iomanip>

void Model::addNumber()  {
    int hour,minute;
    hour = std::stoi(number_.substr(0, 2));
    minute = std::stoi(number_.substr(3, 2));
    minute += 1;
    if (minute >= 60) {
        minute = minute%60;
        hour = (hour+1) % 24;
    }

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hour << ':' << std::setw(2) << minute;

    setNumber(ss.str());
}

int Model::calSum(int a,int b) {
    return a+b;
}
