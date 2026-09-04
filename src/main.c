#include "../include/net.h"
#include <stdio.h>

int main(void)
{
    int port = 0;
    printf("[denko] Enter a port [7000 - 9000]: ");
    if (scanf("%d", &port) != 1)
    {
        printf("[denko] Please enter a numeric port.\n");
        return 1;
    }

    if (port >= 7000 && port <= 9000)
    {
        return openPort(port);
    }
    else
    {
        printf("[denko] Please enter an appropriate port range.\n");
    }

    return 1;
}