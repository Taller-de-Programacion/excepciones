#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <vector>
#include <cstring>
#include <string>
#include <iostream>
/*
 *  Este programa se conecta a un servidor, cuyo host y service viene delimitado por parámetro, y le envia un string
 *  de 4 bytes (sin el '\0'). Luego se queda esperando un mensaje de 5 bytes, lo imprime por salida estandar y cierra
 *  de forma ordenada.
 */

/*
 * Ej0: ¿Qué cosas malas le ven a este código? Hay 4 tópicos claves que suelen ser correcciones
 *      comunes en tps individuales, ¿se animan a vestirse de correctores?
 *
 *      1.
 *      2.
 *      3.
 *      4.
 *
 * Y ahora una tarea mas dificil aun, ¿qué le ven de bueno al código en la función main?
 */

int socket_connect(const char *host_name, const char *service);
int socket_receive(int &fd, char *buffer, size_t len);
int socket_send(int &fd, const char *buffer, size_t len);

int main(int argc, const char *argv[]) {
    int fd = socket_connect(argv[1], argv[2]);

    char *buffer = (char *) calloc(5, sizeof(char));
    strncpy(buffer, "hola", 4);
    socket_send(fd, buffer, 4);

    char *received = (char *) calloc(5, sizeof(char));
    socket_receive (fd, received, 5);

    std::string msg(received, 5);
    std::cout << "Recibido: " << msg << std::endl;
    return 0;
}
/**
 * Crea un socket TCP para realizar conexiones a servidores IPv4 y se conecta al servidor
 * apuntado por <host_name> y <service>
 *
 * @param host_name
 * @param service
 * @return un file descriptor válido si la conexión fue exitosa, -1 caso contrario
 */
int socket_connect(const char *host_name, const char *service){
    int status, fd = -1;
    bool is_connected = false;

    struct addrinfo hints;
    struct addrinfo *result, *ptr;
    /*
     * El servidor al cual me quiero conectar puede tener varios nodos
     * en la lista para probar, esto se puede deber (según la documentación del manual de getaddrinfo):
     *      * el servidor es multihomed (un host se puede traducir en N direcciones IP)
     *      * el servidor puede ser accedido por distintos protocolos (IPv4, IPv6)
     *      * el servidor puede estar disponible en multiploes tipos de sockets (SOCK_STREAM, SOCK_DGRAM)
     *
     * Con la estructura hint podemos reducir algunas de estas posibilidades, pero siempre nos devolverá una lista
     * que debemos iterar
     */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = 0;
    status = getaddrinfo(host_name, service, &hints, &result);
    if (status != 0){
        return -1;
    }
    /*
     * Itero la lista, SIEMPRE ITERAR LA LISTA.
     */
    for (ptr = result; ptr != NULL && !is_connected; ptr = ptr->ai_next){
        fd = socket(ptr->ai_family, ptr->ai_socktype,
                              ptr->ai_protocol);
        /*
        *	si la creación del socket falla, no debo hacer nada mas
        *	en el ciclo (ya que no se abrio ningun fd)
        */
        if (fd == -1){
            continue;
        }
        status = connect(fd, ptr->ai_addr, ptr->ai_addrlen);
        /*
         * Si la conexión falla, el file descriptor queda en un estado inválido y debe ser destruido.
         */
        if (status == -1){
            close(fd);
            fd = -1;
        } else{
            is_connected = true;
        }
    }
    freeaddrinfo(result);
    return is_connected? fd:-1;
}
/**
 * Envía un stream de bytes delemitado por el tamaño del buffer <buff> al socket de file descriptor <skt>
 *
 * @param skt un file descriptor de un socket con la conexión realizada
 * @param buff el mensaje a enviar
 * @return la cantidad de bytes enviados, o -1 si falla
 */
int socket_send(int &skt, const char *buff, size_t length){
    int sent = 0;
    int status = 0;

    while (sent < length){
        status = send(skt, &buff[sent], length - sent, MSG_NOSIGNAL);
        /*
         * status < 0:	hubo un error, salgo del ciclo con un error.
         * status > 0:	se enviaron status bytes, continuo en el ciclo.
         */
        if (status < 0) {
            return -1;
        } else{
            sent += status;
        }
    }
    return sent;
}
/**
 * Recibe un stream de bytes delimitado por el tamaño del buffer <buff> del socket de file descriptor <skt>
 *
 * @param skt un file descriptor de un socket con la conexión realizada
 * @param buff el mensaje a recibir
 * @return la cantidad de bytes recibidps, 0 si el socket del cliente fue cerrado y no se le pudo enviar nada, -1 si falla
 */
int socket_receive(int &skt, char *buff, size_t length){
    int received = 0;
    int status = 0;

    while (received < length){
        status = recv(skt, &buff[received], length - received, 0);
        /*
         * status == 0: cerraron el socket
         * status < 0:	hubo un error
         * status > 0:	se enviaron status bytes
         */
        if (status == 0) {
            return received == length? 0:-1;
        } else if (status < 0){
            return -1;
        } else{
            received += status;
        }
    }
    return received;
}

