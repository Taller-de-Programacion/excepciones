//
// Created by fedemgp on 9/4/22.
//

#ifndef CLASE_EXCEPCIONES_DATE_H
#define CLASE_EXCEPCIONES_DATE_H


class Date {
private:
    unsigned short year, month, day;
    bool is_date_valid();

public:
    Date(unsigned short year, unsigned short month, unsigned short day);
    ~Date() = default;
    void set_day(unsigned short day);
    void set_month(unsigned short month);
    void set_date(unsigned short year, unsigned short month, unsigned short day);
    bool operator==(const Date &other) const;
};


#endif //CLASE_EXCEPCIONES_DATE_H
