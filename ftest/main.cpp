/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <ev++.h>

void DefSIGPIPE(int sig)
{
}

int main(int argc, char* argv[])
{
    struct sigaction act;
    act.sa_handler = DefSIGPIPE;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGPIPE, &act, 0);


    ev::default_loop loop;
    loop.run(0);
}
