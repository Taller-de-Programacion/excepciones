//
// Created by fedemgp on 3/4/22.
//

#ifndef CLASE_EXCEPCIONES_SOCKET_H
#define CLASE_EXCEPCIONES_SOCKET_H

#include <vector>
#include <unistd.h>
#include <sys/socket.h>

class Socket {
private:
    int fd;
    static bool _try_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen, void *ctx);

public:
    Socket(const char *hostname, const char *service);
    void send(const char *buff, size_t len);
    void receive(char *buff, size_t len);
    void close();
    ~Socket();
    /*
     * Un socket nunca debería ser copiable. Debería ser movible pero a efectos de este ejemplo por ahora no lo dejaré
     * ser.
     */
    Socket(Socket &copy) = delete;
    Socket& operator=(Socket& copy) = delete;
    Socket(Socket &&other) = delete;
    Socket& operator=(Socket&& other) = delete;
};


#endif //CLASE_EXCEPCIONES_SOCKET_H
