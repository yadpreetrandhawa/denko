#include "../include/net.h"
#include <stdio.h>

int main(void)
{
    int port = 0;
    printf("[denko] Enter a port [7000 - 9000]: ");
    scanf("%d", &port);
    if (port >= 7000 && port <= 9000)
    {
        openPort(port);
    }
    else
    {
        printf("[denko] Please enter a approiate port range");
    }
    return 1;
}