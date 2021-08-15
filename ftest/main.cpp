/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "client.h"
#include "server.h"

#include <log.h>
#include <singletone.h>
#include <sslctx.h>

int main(int argc, char* argv[])
{
    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetPrintTimeStamp(true);
    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetAppPrefix(argv[0]);

    sparrow::Singletone<sparrow::LoggerFactory>::Instance().SetLevel(
        sparrow::LogLevel::Always,
        sparrow::LogLevel::Error,
        sparrow::LogLevel::Panic,
        sparrow::LogLevel::Info,
        sparrow::LogLevel::Warning);

    sparrow::Singletone<sparrow::SslContext>::Instance();

    struct sigaction act;
    act.sa_handler = [](int) {};
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGPIPE, &act, 0);

    ev::default_loop loop;
    SslServer sslServer;
    SslClient sslClient;

    loop.run(0);
}
