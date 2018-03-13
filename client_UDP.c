/* Sample UDP client */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char recvline[1000];

    char *sendline[] = {                                //This is initializing data packet
            "This is Data Packet 01",
            "This is Data Packet 02",
            "This is Data Packet 03",
            "This is Data Packet 04",
            "This is Data Packet 05",
            "This is Data Packet 06",
            "This is Data Packet 07",
            "This is Data Packet 08",
            "This is Data Packet 09",
            "This is Data Packet 10",
            "This is Data Packet 11",
            "This is Data Packet 12",
            "This is Data Packet 13",
            "This is Data Packet 14",
            "This is Data Packet 15",
            "This is Data Packet 16"
    };

    if (argc != 2) {
        printf("usage:%s <IP address>\n", argv[0]);
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct timeval tv;
    tv.tv_sec = 1;                                                              //timout by second
    tv.tv_usec = 100000;                                                        //timout by microsecond
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(32000);

    int i,seq;                                                                  //define seq
    int end = sizeof(sendline) / sizeof(sendline[0]);
    for (i = 0; i < end; i++) {
        char seq_str[1];
        seq = i / 10;
        seq = i - seq * 10;                                                 
        sprintf(seq_str, "%d", seq);                                           //convert seq to string

        char sendPacket[1002];
        sendPacket[0] = seq_str[0];

        int k = 0;
        while (sendline[i][k]) {
            sendPacket[k + 1] = sendline[i][k];
            k++;
        }

        printf("Data for Send: %s\n", sendline[i]);                         //First Attemt
        printf("Packet Send: %s\n", sendPacket);
        sendto(sockfd, sendPacket, strlen(sendPacket), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
        n = recvfrom(sockfd, recvline, 10000, 0, NULL, NULL);
        printf("Seq Received: %s\n", recvline);
        recvline[n] = 0;
        int tries = 1;

        while (seq_str[0] != recvline[0]) {                                 //Check seq resived and retry
            printf("(Timeout) \nRetry %d\n", tries++);
            printf("Packet Send: %s\n", sendPacket);
            sendto(sockfd, sendPacket, strlen(sendPacket), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
            n = recvfrom(sockfd, recvline, 10000, 0, NULL, NULL);
            printf("Seq Received: %s\n", recvline);
            recvline[n] = 0;
        }
        printf("Sent success:%s\n\n", recvline);
    }


    return 0;
}

