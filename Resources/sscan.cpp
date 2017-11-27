/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../Header/sscan.h"

int sendData_s(int s, const void *buffer, int buflen)
{
    const char *pbuf = (const char*) buffer;
    while (buflen > 0)
    {
        int numSent = send(s, pbuf, buflen, 0);
        if (numSent == -1)
            return -1;
        pbuf += numSent;
        buflen -= numSent;
    }
    return 1;
}

int recvData_s(int s, void *buffer, int buflen)
{
    char *pbuf = (char*) buffer;
    while (buflen > 0)
    {
        int numRecv = recv(s, pbuf, buflen, 0);
        if (numRecv == -1)
            return -1;
        if (numRecv == 0)
            return 0;
        pbuf += numRecv;
        buflen -= numRecv;
    }
    return 1;
}


int connnectSOCKS5packet1_s(QUERY * get,fd_set * rset, fd_set * wset){
    int initPacket1Length = 3;
    char initPacket1[initPacket1Length];
    initPacket1[0] = 5;
    initPacket1[1] = 1;
    initPacket1[2] = 0;

    if (sendData_s(get->fd, initPacket1, initPacket1Length) < 0){
        std::cerr << "Can't send first init packet to SOCKS server!" << std::endl;
        //closesocket(get->fd);
        return -1;
    }
    else {
        FD_CLR(get->fd, wset);
        FD_SET(get->fd, rset);
        get->flags = READRES1;
        return 1;
    }
}

bool checkConnnectSOCKS5packet1_s(QUERY * get, fd_set * rset, fd_set * wset){
    char reply1[2];
    //recv(get->fd, reply1, 2, 0);

    if (recvData_s(get->fd, reply1, 2) <= 0)
    {
        std::cerr<< "Error reading first init packet response!" <<"" << reply1 << std::endl;
        return false;
    }

    //std::cerr << "read 1: " << reply1 << std::endl;
    //std::cout <<"check 1" << reply1 << std::endl;
    //reply[0] = our version
    //reply[1] = out method. [X’00’ NO AUTHENTICATION REQUIRED]

    if( !(reply1[0] == 5 && reply1[1] == 0) )
    {
        std::cerr << "Bad response for init packet!" << std::endl;
        return false;
    }
    FD_CLR(get->fd, rset);
    FD_SET(get->fd, wset);
    return true;
}

int sendNon_s(QUERY *get, fd_set * rset, fd_set * wset, int * maxfd){

    int n, flags;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;                                          // host byte order
    addr.sin_port = htons(get->port);                                   // short, network byte order
    addr.sin_addr.s_addr = inet_addr(get->host.c_str());           // adding socks server IP address into structure

    int sendfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    u_long iMode = 1;

    //int iResult = ioctlsocket(sendfd, FIONBIO, &iMode);

    if (sendfd < 0){
        puts("can't create socket!!");
        close(sendfd);
        return -1;
    }
    get->fd = sendfd;
    if ( (n = connect(sendfd, (struct sockaddr*) &addr, sizeof(addr) ) ) < 0 ){
        get->flags = CONNECTING;
        FD_SET(sendfd, rset);
        FD_SET(sendfd, wset);

        if (sendfd > *maxfd ){
            //std::cout <<"max : " << *maxfd  << " " << sendfd <<std::endl;
            *maxfd = sendfd;
        }
        return sendfd;
    } else{
        //closesocket(sendfd);
        connnectSOCKS5packet1_s(get, rset,wset);
        //return -1;
    }
}


bool checkSocks(std::vector<std::pair<std::string, int> > checkList, std::ofstream& outF, int nos, int timeout){

    std::vector<QUERY> listCon;
    listCon.clear();
    std::stack<QUERY> sCon;

    fd_set rset, wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);

    int maxfd = -1;
    rep(i,checkList.size()){
        QUERY que;
        que.fd = 0;
        que.host = checkList[i].X;
        que.flags = 0;
        que.port = checkList[i].Y;
        sCon.push(que);
    }

    int nconn = 0;
    while ( !sCon.empty()  || nconn > 0){
        while (!sCon.empty() && nconn < NOS_DEFAULT) {
            int fdu = -1;
            QUERY con = sCon.top();
            sCon.pop();
            fdu = sendNon_s(&con, &rset, &wset, &maxfd);
            if (fdu > 0 ){
                //std::cout <<"fd: "  << fdu << std::endl;
                listCon.pb(con);
                ++nconn;
            }
        }
        fd_set rs,ws;
        rs = rset;
        ws = wset;

        struct timeval timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        int n = select(maxfd + 1, &rs, &ws, NULL, &timeout);
        //std::cout << "max: " << maxfd <<" " << " "<< sCon.size() << " " << listCon.size() << " " << n <<" ";// std::endl;
        //rep(i,listCon.size())
        //std::cout << listCon.size()<<" " <<listCon[i].host <<" " <<listCon[i].flags << " " <<maxfd << " ";// std::endl;
        if (n > 0){
            rep(i,listCon.size()){
                int fdu = listCon[i].fd;
                int flags = listCon[i].flags;
                //std::cout << fdu <<" " << flags << std::endl;
                if ( (flags & CONNECTING) && ( FD_ISSET(fdu,&rs) || FD_ISSET(fdu,&ws) ) ){
                        std::cout <<listCon[i].host <<":"<< listCon[i].port << std::endl;
                        if ( connnectSOCKS5packet1_s(&listCon[i] , &rset,&wset) == -1){
                            FD_CLR(fdu, &wset);
                            FD_CLR(fdu, &rset);
                            close(fdu);
                            --nconn;
                        } else{
                            //std::cout <<"ok send 1 " << listCon[i].flags <<"\n";
                        }
                } else if (flags & READRES1 && ( FD_ISSET(fdu,&rs) || FD_ISSET(fdu,&ws) ) ){
                        if ( checkConnnectSOCKS5packet1_s(&listCon[i] , &rset,&wset) ){
                            outF <<listCon[i].host <<":" << listCon[i].port <<"\n";
                            std::cout << "     socks5 "<<listCon[i].host <<":" << listCon[i].port << " ok\n";
                            FD_CLR(fdu, &wset);
                            FD_CLR(fdu, &rset);
                            listCon[i].flags = DONE;
                            close(fdu);
                            --nconn;
                        }else{
                            FD_CLR(fdu, &wset);
                            FD_CLR(fdu, &rset);
                            close(fdu);
                            --nconn;
                        }
                }
            }
        } else{
            rep(i,listCon.size()){
                if (listCon[i].flags != DONE)
                    {
                        close(listCon[i].fd);
                        --nconn;
                    }
            }
            FD_ZERO(&rset);
            FD_ZERO(&wset);
            listCon.clear();
        }
    }
}