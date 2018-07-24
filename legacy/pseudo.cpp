
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "windivert.h"

HANDLE handle;
WINDIVERT_ADDRESS addr; // Packet address
PWINDIVERT_UDPHDR udp_header;
char packet[MAXBUF];
UINT packetLen;
UINT8 *dst_addr = NULL;
UINT16 dst_port = 80;

#define MAXBUF  0xFFFF

char* readParam(char* in){
    char* dstAddrLang = "udp.DstAddr=";
    char* dstPortLang = "udp.DstPort=";
    char* dstPortMatch = "";
    //char str[INET_ADDRSTRLEN];
    if(strstr(in, "->") != NULL) {
        dstPortMatch = strtok(in, "->");
        dst_port = atoi(strtok(NULL, "->"));
        return dstPortLang + dstPortMatch;
    } else {
        //inet_pton(AF_INET, in, &dst_addr);
        return dstAddrLang + in;
    }
    return "";
}

int __cdecl main(int argc, char **argv) {

    // TODO: return filter policy here from param
    char* truncPolic = "";
    truncPolic = readParam(argv[0]) + "and" + readParam(argv[1]);

    handle = WinDivertOpen(truncPolic, 0, 0, 0);   // Open some filter
    if (handle == INVALID_HANDLE_VALUE)
    {
        // Handle error
        exit(1);
    }

    // Main capture-modify-inject loop:
    while (TRUE)
    {
        if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packetLen))
        {
            // Handle recv error
            continue;
        }

        // Modify packet.
        WinDivertHelperParsePacket(packet, &packetLen, &addr, NULL,NULL,NULL,NULL,&udp_header,NULL,NULL);

        addr->DstAddr = dst_addr;
        udp_header->DstPort = dst_port;

        WinDivertHelperCalcChecksums(packet, &packetLen, &addr, 0);
        if (!WinDivertSend(handle, packet, &packetLen, &addr, NULL))
        {
            // Handle send error
            continue;
        }
    }
}

