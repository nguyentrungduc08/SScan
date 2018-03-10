/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../Header/sscan.h"

void int_to_ip(uint32_t ip, char * addr){
    unsigned char bytes[4];
    bytes[0] = (ip) & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    sprintf(addr, "%d.%d.%d.%d", bytes[3], bytes[2],bytes[1], bytes[0]);
}


void getListIp(std::string namefile){
    std::regex      e("(\\d{1,3}(\\.\\d{1,3}){3})");
    std::ifstream   inputfile;

    inputfile.open(namefile.c_str());

    if (inputfile.is_open()){

    } else{
        std::cerr << "can't open file ip.\n";
        inputfile.close();
    }
}

std::string get_ip(std::string const &s, std::regex const &r){
    std::smatch match;

    if (std::regex_search(s,match,r)){
        //std::cout << "Ip address: " << match[1] << std::endl;
        return match[1];
    }
    else{
        std::cerr << s << "not match" << std::endl;
        return NULL;
    }
}

std::pair<std::string, std::string> get_range(std::string const &s, std::regex const &r){
    std::smatch match;
    std::pair<std::string, std::string> ss;
    if (std::regex_search(s,match,r)){
        //for(int i = 0; i < match.size(); ++i){
        //cout << i <<": " << match[i].str() << endl;
        //}
        ss = std::make_pair(std::string(match[1]),std::string(match[3]));
        return ss;
        //cout << "range: " << match[1] <<" " << match[3] << endl;
        //outfile <<"range: " << match[1] << " " << match[2] << endl;
    } else{
        std::cerr << s << "not match" << std::endl;
        //return NULL;
    }
}

void  getListIp(std::string namefile,std::vector<std::string> *listIP, std::vector< std::pair<std::string, std::string> > *listRange){
    std::regex      e("(\\d{1,3}(\\.\\d{1,3}){3})");
    std::regex      ee("(\\d{1,3}(\\.\\d{1,3}){3}) (\\d{1,3}(\\.\\d{1,3}){3})");
    std::ifstream   inputfile;

    inputfile.open(namefile.c_str());

    if (inputfile.is_open()){
        std::string line;
        std::pair<std::string,std::string> ss;
        while (getline(inputfile, line) ){
            if (line.length() > 0){
                //std::cout <<line <<" "<< line.length() << std::endl;
                if (line.length() <= 22){
                    std::string ip = get_ip(line, e);
                    //std::cout <<"ip " << ip << std::endl;
                    listIP->push_back(ip);
                }
                else{
                    ss = get_range(line, ee);
                    listRange->push_back(ss);
                    //std::cout << "range: "<<ss.first <<" " << ss.second<< std::endl;
                }
            }

        }
        inputfile.close();
    } else{
        std::cerr << "can't open file ip\n";
        inputfile.close();
    }
}

uint32_t ip_to_int (const char * ip)
{
    /* The return value. */
    unsigned v = 0;
    /* The count of the number of bytes processed. */
    int i;
    /* A pointer to the next digit to process. */
    const char * start;

    start = ip;
    for (i = 0; i < 4; i++) {
        /* The digit being processed. */
        char c;
        /* The value of this byte. */
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            /* We insist on stopping at "." if we are still parsing
               the first, second, or third numbers. If we have reached
               the end of the numbers, we will allow any character. */
            else if ((i < 3 && c == '.') || i == 3) {
                break;
            }
            else {
                return -1;
            }
        }
        if (n >= 256) {
            return -1;
        }
        v *= 256;
        v += n;
    }
    return v;
}
