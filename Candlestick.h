#pragma once
#include <string>
#include <iostream>

class Candlestick {
public:
    std::string date;
    double open;
    double high;
    double low;
    double close;

    Candlestick(std::string date, double open, double high, double low, double close) :
        date(date), open(open), high(high), low(low), close(close) {}

    void printData() {
        std::cout << "Date: " << date << ", Open: " << open << ", High: " << high
            << ", Low: " << low << ", Close: " << close << std::endl;
    }
};