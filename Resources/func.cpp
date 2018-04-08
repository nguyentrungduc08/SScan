#include "../Header/sscan.h"


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

    cout << (now->tm_year + 1900)   << '-' 
         << (now->tm_mon + 1)       << '-'
         <<  now->tm_mday           << endl;
}

int getdate(){
    time_t t            = time(0);   // get time now
    struct tm * now     = localtime( & t );
    return now->tm_mon +1;
}