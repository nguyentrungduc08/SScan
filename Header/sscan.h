/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sscan.h
 * Author: hydra
 *
 * Created on November 27, 2017, 10:18 AM
 */

#ifndef SSCAN_H
#define SSCAN_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include <sys/select.h>
#include <sys/ioctl.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <stack>
#include <queue>
#include <regex>
#include <vector>
#include <chrono>
#include <thread>

//define template
#define rep(i,n) for(int i = 0; i < n; ++i)
#define FOR(i,a,b) for(int i = (int) a; i <= (int)b; ++i)
#define pb push_back
#define X first
#define Y socond
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))



//prototypes for functions
std::vector<int> getListPort();
void  getListIp(std::string namefile,std::vector<std::string> *listIP, std::vector< std::pair<std::string, std::string> > *listRange);


#endif /* SSCAN_H */

