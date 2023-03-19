#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define GROUP "239.0.0.1"

struct MyData {
    float smvs[16];
};

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in addr;
    struct ip_mreq mreq;
    MyData data = {0.0f};
    
    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Set up multicast address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    
    // Bind to the port
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    // Set up multicast group
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // Set up destination address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(GROUP);
    addr.sin_port = htons(PORT);
    
    // Send data
    int n = sendto(sock, &data, sizeof(data), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (n < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    
    close(sock);
    return 0;
}
