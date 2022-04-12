//
// Created by fedemgp on 3/4/22.
//

#ifndef CLASE_EXCEPCIONES_OSERROR_H
#define CLASE_EXCEPCIONES_OSERROR_H

#include <exception>

#define BUFF_LEN 256

class OSError: public std::exception {
private:
    char msg_error[BUFF_LEN];

public:
    OSError() noexcept;
    OSError(const char *fmt, ...) noexcept;
    virtual const char *what() const noexcept;
    virtual ~OSError() noexcept = default;

};


#endif //CLASE_EXCEPCIONES_OSERROR_H
