//
// Created by fedemgp on 9/4/22.
//

#include "Date.h"
#include <stdexcept>

Date::Date(unsigned short year, unsigned short month, unsigned short day): year(year), month(month), day(day) {
    if (!this->is_date_valid()) throw std::runtime_error("Fecha inválida");

}

void Date::set_day(unsigned short day) {
    this->day = day;
    if (!this->is_date_valid()) throw std::runtime_error("Fecha inválida");
}

void Date::set_month(unsigned short month) {
    this->month = month;
    if (!this->is_date_valid()) throw std::runtime_error("Fecha inválida");
}

bool Date::operator==(const Date &d) const{
    return this->year == d.year && this->month == d.month && this->day == d.day;
}

bool Date::is_date_valid() {
    if (this->month > 12 || this->month == 0) return false;
    switch (this->month) {
        case 1:
            return this->day > 0 and this->day <= 31;
        case 2:
            return this->day > 0 and this->day <= 28;
        case 3:
            return this->day > 0 and this->day <= 31;
        case 4:
            return this->day > 0 and this->day <= 30;
        case 5:
            return this->day > 0 and this->day <= 31;
        case 6:
            return this->day > 0 and this->day <= 30;
        case 7:
            return this->day > 0 and this->day <= 31;
        case 8:
            return this->day > 0 and this->day <= 31;
        case 9:
            return this->day > 0 and this->day <= 30;
        case 10:
            return this->day > 0 and this->day <= 31;
        case 11:
            return this->day > 0 and this->day <= 30;
        case 12:
            return this->day > 0 and this->day <= 31;
    }
    return false;
}

void Date::set_date(unsigned short year, unsigned short month, unsigned short day) {
    Date temp(year, month, day);
    this->year = year;
    this->month = month;
    this->day = day;
}
