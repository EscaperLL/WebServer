#include"EventLoop.h"
#include"Server.h"

int main()
{
    EventLoop loop;
    Server server(&loop,8664);
    LOG<<"start\r\n";
    server.start();
    loop.loop();
    return 0;
}