/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "client.h"
#include "server.h"

#include <log.h>
#include <singletone.h>
#include <sslcontext.h>
#include <unistd.h>

void usage()
{
    std::cout << "usage:" << std::endl;
    std::cout << "-c - Certificate file name" << std::endl;
    std::cout << "-c - Private key file name" << std::endl;
    std::cout << "-l - logging level(TRACE,DEBUG,INFO) ERROR,PANIC are always on" << std::endl;
    std::cout << "-t - Print timestamp in log output" << std::endl;
}

int main(int argc, char* argv[])
{

    struct sigaction act;
    act.sa_handler = [](int) {};
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGPIPE, &act, 0);

    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetPrintTimeStamp(true);
    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetAppPrefix(argv[0]);
    int opt;
    std::string level;
    bool printTimestamp = false;

    while ((opt = getopt(argc, argv, "c:k:f:l:")) != -1) {
        switch (opt) {
        case 'c':
            sertificate = sparrow::trim_copy(optarg);
            break;
        case 'k':
            privateKey = sparrow::trim_copy(optarg);
            break;
        case 'l':
            level = optarg;
            break;
        case 't':
            printTimestamp = true;
            break;
        default: /* '?' */
            usage();
            exit(EXIT_FAILURE);
        }
    }

    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetPrintTimeStamp(printTimestamp);
    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetAppPrefix(argv[0]);

    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetLevel(
        sparrow::LogLevel::Always,
        sparrow::LogLevel::Error,
        sparrow::LogLevel::Panic,
        sparrow::LogLevel::Info,
        sparrow::LogLevel::Debug,
        sparrow::LogLevel::Warning);

    //    sparrow::Singletone<sparrow::SslContext>::Instance();

    ev::default_loop loop;
    SslServer sslServer;
    SslClient sslClient;

    loop.run(0);
}
