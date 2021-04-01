#include "socketdev.h"

/*
We would like our program to take the hostname and port number of the server it should connect to
as command-line arguments. This makes the program flexible.
*/
void socket_programming::init_socket(void){
    std::cout << "Configuring remote address...\n" << std::endl;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(ip_addr, port, &hints, &peer_address)){
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        throw;
    } 
}
/*
It is good practice to print out the address that the program is connected to
*/
void socket_programming::print_addr(void){
    std::cout << "Remote address is: " << std::endl;
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, 
                address_buffer, sizeof(address_buffer),
                service_buffer, sizeof(service_buffer), 
                NI_NUMERICHOST);
    std::cout << address_buffer <<":"<< service_buffer << std::endl;
}
/*
This function will create the socket
*/
void socket_programming::create(void){
    std::cout << "Creating socket...\n" << std::endl;
    socket_peer = socket(peer_address->ai_family, 
        peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        throw;
    }
}
/*
After the socket has been created, we call connect() to establish a connection to the remote server
*/
void socket_programming::make_connection(void){
    std::cout << "Connecting...\n" << std::endl;
    if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)){
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        throw;
    }
    freeaddrinfo(peer_address);
    std::cout << "Connected.\n" << std::endl;
}
/* Waiting for established connection */
void socket_programming::wait_for_connection(void){
    while(connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)){
        std::cout << "Connecting.. \n" <<std::endl;
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        sleep(1);
    }
    freeaddrinfo(peer_address);
    std::cout << "Connected.\n" << std::endl;
}
/*
Our programming should now loop indefinetily to check for new data coming in
*/
char * socket_programming::run(void){
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(socket_peer, &reads);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000000;
    // send a request to the server
    char *request = "request from client"; 
    send(socket_peer, request, strlen(request), 0);

    if (select(socket_peer+1, &reads, 0, 0, &timeout) < 0) {
        fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
        throw;
    }
    if (FD_ISSET(socket_peer, &reads)){
        bytes_received = recv(socket_peer, read, 8192, 0);
    if (bytes_received < 1) {
        std::cout << "Connection closed by peer.\n" << std::endl;
        throw;
    }
    //printf("Received (%d bytes): %.*s", bytes_received, bytes_received, read);
    }
    return read;
}
/*
some clean up
*/
socket_programming::~socket_programming(void){
    std::cout << "Closing socket...\n" << std::endl;
    CLOSESOCKET(socket_peer);

    std::cout << "Finished.\n" << std::endl;
}
