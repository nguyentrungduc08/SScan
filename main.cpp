/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Nguyen Trung Duc
 *
 * Created on November 27, 2017, 9:18 AM
 */

#include "Header/sscan.h"

using namespace std;

/*
 * 
 */


vector<pair<string,string>>     listRange;
vector<string>                  listIP;
vector< pair<string,int> >      listHostIP;
vector<string>                  listPort;
vector<int>                     listPortInt;
string                          cmdMasscan;
ofstream                        outFile;

//prototype functions 

void init();
void buildCMDMasscan();
void getListHostIP();

void time(){
    time_t      rawtime;
    struct tm*  ptm;

    time ( &rawtime );

    ptm = gmtime ( &rawtime );

    puts   ("Current time around the World:");
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
    time_t t        = time(0);   // get time now
    struct tm * now = localtime( & t );
    return now->tm_mon +1;
}

int main(int argc, char** argv) {
    
    int month = getdate();
    
    auto start = std::chrono::system_clock::now();
    init();
    cout << listRange.size() << " " << listIP.size() <<" " << listPortInt.size() << endl;
    //buildCMDMasscan();
    
    rep(i,listRange.size()){
        uint32_t    be, en;
        string      sbe, sen;
        uint32_t    ip;
        sbe     = listRange[i].first;
        sen     = listRange[i].second;
        be      = ip_to_int(sbe.c_str());
        en      = ip_to_int(sen.c_str());
        //cout <<be <<  " " << en << endl;
        
        for(ip = be; ip <= en; ip = ip + (uint32_t) 1) {
            char ipc[20];
            int_to_ip(ip,ipc);
            //cout << ipc << endl;
            string s(ipc);
            listIP.pb(s);
            //break;
        }
    }
    
    for(;;){
        rep(i,listIP.size())
            rep(j,listPortInt.size()){
                pair<string, int> pp = make_pair(listIP[i],listPortInt[j]);
                listHostIP.pb(pp);
                if (listHostIP.size() >= 30000){
                    checkSocks(listHostIP,outFile);
                    listHostIP.clear();
                }
            }
    }
    
    /*
    
    int time  = 1;
    //while (1){
        outFile << "time scan: " << time << endl;
        system(cmdMasscan.c_str());
        getListHostIP();
        outFile <<  listHostIP.size() << endl;
    
        int sysI = system("rm -rf scan.xml");
    
        cout << "log sys: " << sysI << endl;
        checkSocks(listHostIP,outFile);
        //cout << cmdMasscan;
        //outFile << cmdMasscan << endl;
    
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "     Time: " << elapsed_seconds.count() << " (s)\n";
        //++time;
    //}
     */
    outFile.close();
    return 0;
}

void init(){
    getListIp("files/ip.txt", &listIP, &listRange);
    //getListPort("files/port.txt",&listPort);
    listPortInt = getListPortInt();
    outFile.open("socks.txt");
}

void buildCMDMasscan(){ 
    cmdMasscan  =  "sudo ./masscan/masscan ";
    string ipl  = "";
    rep(i,listRange.size()){
        string s = listRange[i].X +  "-" + listRange[i].Y;
        //cout << s << endl;
        s   +=  ",";
        ipl +=  s;
    }
    cmdMasscan += ipl;
    ipl = "";
    rep(i,listIP.size()){
        string s = listIP[i];
        //cout << s << endl;
        s+=",";
        ipl+=s;
    }
    cmdMasscan += ipl;
    cmdMasscan.erase(cmdMasscan.end()-1);
    
    cmdMasscan += " -p";
    ipl = "";
    rep(i,listPort.size()){
        string s = listPort[i];
        //cout << s << endl;
        s+=",";
        ipl+=s;
    }
    cmdMasscan += ipl;
    cmdMasscan.erase(cmdMasscan.end()-1);
    
    cmdMasscan += " --max-rate 10000 -oX scan.xml"; 
    return;
}

void getListHostIP(){
    ifstream fileHost;
    
    fileHost.open("scan.xml");
    regex eIP(".*<address addr=\"(.*?)\" addrtype");
    regex ePort(".*portid=\"(.*?)\"><state state");
    if (fileHost.is_open()){
        string line;
        std::smatch match;
        string ip;
        int port;
        string result;
         while (getline(fileHost, line) ){
             if (std::regex_search(line, match, eIP) && match.size() > 1) {
                            result = match.str(1);
                        } else {
                            result = std::string("");
                        } 
             if (result.length() > 0)
             {
                 //outFile << result << ":";
                 ip = result;
             }
             if (std::regex_search(line, match, ePort) && match.size() > 1) {
                            result = match.str(1);
                        } else {
                            result = std::string("");
                        } 
             if (result.length() > 0){
                //outFile << result << "\n";
                stringstream sst(result);
                port = 0;
                sst >> port;
                pair<string, int> pp = make_pair(ip,port);
                listHostIP.pb(pp);
             }
         }
        fileHost.close();
        
    } else{
        cerr << "can't open file scan!!!\n";
    }
    return;
}
