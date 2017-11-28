/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: hydra
 *
 * Created on November 27, 2017, 9:18 AM
 */

#include "Header/sscan.h"

using namespace std;

/*
 * 
 */


vector<pair<string,string>> listRange;
vector<string> listIP;
vector< pair<string,int> > listHostIP;
string cmdMasscan;
ofstream outFile;

//prototype functions 

void init();
void buildCMDMasscan();
void getListHostIP();

void time(){
    time_t rawtime;
    struct tm * ptm;

    time ( &rawtime );

    ptm = gmtime ( &rawtime );

    puts ("Current time around the World:");
    printf ("Phoenix, AZ (U.S.) :  %2d:%02d\n", (ptm->tm_hour+MST)%24, ptm->tm_min);
    printf ("Reykjavik (Iceland) : %2d:%02d\n", (ptm->tm_hour+UTC)%24, ptm->tm_min);
    printf ("Beijing (China) :     %2d:%02d\n", (ptm->tm_hour+CCT)%24, ptm->tm_min);
    
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    cout << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << endl;
}

int getdate(){
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    return now->tm_mon +1;
}

int main(int argc, char** argv) {
    
    int month = getdate();
    
    time();
    init();
    cout << listRange.size() << " " << listIP.size() << endl;
    buildCMDMasscan();
    //cout << cmdMasscan;
    //outFile << cmdMasscan << endl;
    system(cmdMasscan.c_str());
    outFile.close();
    return 0;
}

void init(){
    getListIp("files/ip.txt", &listIP, &listRange);
    outFile.open("socks.txt");
}

void buildCMDMasscan(){
    cmdMasscan =  "sudo ./masscan/bin/masscan ";
    string ipl;
    rep(i,listRange.size()){
        string s = listRange[i].X +  "-" + listRange[i].Y;
        //cout << s << endl;
        s+=",";
        ipl+=s;
    }
    cmdMasscan += ipl;
    cmdMasscan.erase(cmdMasscan.end()-1);
    cmdMasscan += " -p0-65535 --max-rate 10000 -oX scan.xml";   
}

void getListHostIP(){
    ifstream fileHost;
    
    fileHost.open("scan.xml");
    regex e(".*<address addr=" " addrtype=");
    if (fileHost.is_open()){
        string line;
         while (getline(fileHost, line) ){
             
         }
        
    } else{
        cerr << "can't open file!!!\n";
    }
    return;
}