/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../Header/sscan.h"

bool checklineRangePort(std::string line){
    rep(i,line.length())
        if (line[i] == '-')
            return true;
    return false;
}

std::vector<int> getListPortInt(){
    std::vector<int>    listPortm;
    bool                userPort[70000];
    std::ifstream       inputFile;

    memset(userPort,0,sizeof(userPort));
    inputFile.open("files/port.txt");
    if (inputFile.is_open()){
        //std::cout <<"ok opent \n";
        std::string line;
        while (getline(inputFile, line ) ){
            //std::cout << line << std::endl;
            if (line.length() > 0){
                while (line[0] <'0' || line[0] > '9'){
                    line.erase(line.begin());
                }

                if (checklineRangePort(line)){
                    int bport, eport;
                    sscanf(line.c_str(), "%d - %d", &bport, &eport);
                    //std::cout << "range: " <<line.length() <<" " << bport << " " << eport << std::endl;
                    FOR(i,bport,eport)
                        userPort[i] = true;
                }
                else{
                    int port;
                    //std::cout << "port!! " << line << std::endl;
                    sscanf(line.c_str(), "%d",&port);
                    //std::cout << "port: "  << port <<" " << line.length() << std::endl;
                    if (port > 0 && port < 65555)
                        userPort[port] = true;
                }
            }
        }
        rep(i, 65555)
            if (userPort[i])
                listPortm.pb(i);
    } else{
        std::cerr << "can't open file port\n";
        inputFile.close();
    }
    return listPortm;
}

void  getListPort(std::string namefile,std::vector<std::string> *listPort){
    std::ifstream   inputFile;
    inputFile.open(namefile.c_str());
    
    if (inputFile.is_open()){
        std::string line;
        while ( getline(inputFile, line)){
            while (line[0] <'0' || line[0] > '9'){
                    line.erase(line.begin());
                }

                if (checklineRangePort(line)){
                    int bport, eport;
                    sscanf(line.c_str(), "%d - %d", &bport, &eport);
                    std::stringstream ss;
                    ss <<bport <<"-"<<eport;
                    std::string range = ss.str();
                    listPort->pb(range);
                }
                else{
                    int port;
                    sscanf(line.c_str(), "%d",&port);
                    std::stringstream ss;
                    ss << port;
                    std::string sport = ss.str();
                    listPort->pb(sport);
                }
        } 
    } else{
        std::cerr<< "can't open file port\n";
        inputFile.close();
    }
    
}