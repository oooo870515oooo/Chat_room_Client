//
//  main.c
//  Chat_room_Client
//
//  Created by 許韋中 on 2019/9/24.
//  Copyright © 2019 許韋中. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct sockaddr *sockaddrp;
int sockfd;

void *recv_other(void *arg)
{
    char buf[255]= {};
    while(1)
    {
        int ret = recv(sockfd,buf,sizeof(buf),0);
        if(0 > ret)
        {
            perror("recv");
            return 0;
        }

        printf("%s\n",buf);
    }
}




int main(int argc,char **argv)
{
    if(3 != argc)
    {
        perror("參數錯誤");
        return -1;
    }

    //建立socket對象
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }

    //準備連接地址
    struct sockaddr_in addr = {AF_INET};
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    socklen_t addr_len = sizeof(addr);


    //連接
    int ret = connect(sockfd,(sockaddrp)&addr,addr_len);
    if(0 > ret)
    {
        perror("connect");
        return -1;
    }

    //發送名字
    char buf[255] = {};
    char name[255] = {};
    printf("你叫三小：");
    scanf("%s",name);
    printf("\n彩蛋請輸入以下\n\n");
    printf("What is your name?\n");
    printf("How are you?\n");
    printf("How are things?\n");
    printf("想滾請輸入：exit\n\n\n");
    ret = send(sockfd,name,strlen(name),0);
    if(0 > ret)
    {
        perror("connect");
        return -1;
    }

    //創建接收子線程
    pthread_t tid;
    ret = pthread_create(&tid,NULL,recv_other,NULL);

    if(0 > ret)
    {
        perror("pthread_create");
        return -1;
    }
    //循環發送
    while(1)
    {
        //printf("%s:",name);
        gets(buf);
        int ret = send(sockfd,buf,strlen(buf),0);
        if(0 > ret)
        {
            perror("send");
            return -1;
        }

        //輸入exit退出
        if(0 == strcmp("exit",buf))
        {
            printf("%s,你已經滾出聊天室了\n",name);
            return 0;
        }


    }

}

