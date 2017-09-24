#include "main.h"

#include <pcap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/* function that writes a detection log */
void LogIpDetection(char *fileName, char *logData, int isExit)
{
	static FILE* fdWrite = NULL;

	if(isExit == 1)
	{
		fclose(fdWrite);
		fdWrite = NULL;
		return;
	}

	if(fdWrite == NULL)
	{
		fdWrite = fopen(fileName, "a");
		if(fdWrite == NULL)
		{
			perror("wrtie error:");
			return;
		}
	}

	fprintf(fdWrite, "%s\n", logData);

	return;
}

/* function that detects specified ip address */
void checkIpAddress(unsigned char *pktData, char *targetAddr)
{

	ip_header *iph = (ip_header *)(&pktData[14]);
	struct sockaddr_in src, dst;

	src.sin_addr.s_addr = iph->srcAddr;
  dst.sin_addr.s_addr = iph->dstAddr;

	char *srcAddr = inet_ntoa(src.sin_addr);
	char *dstAddr = inet_ntoa(dst.sin_addr);

	char logData[MAX_LOG_LENGTH];
	// test
	printf("\nsrcAddr:%s dstAddr:%s targetAddr:%s"
	, srcAddr, dstAddr, targetAddr);
	if( !strcmp(srcAddr, targetAddr) )
	{
		setcolor(RESET_BG);
		printf("\n###=====>>>> Target address founed in srcAddr [%s]\n", srcAddr);
		memset(logData, 0x00, sizeof(logData));
		sprintf(logData, "source address:%s target address:%s", srcAddr, targetAddr);
		LogIpDetection("testLog.log", logData, 0);
		// test
		//printf("\nsrcAddr:%s dstAddr:%s targetAddr:%s"
		//, srcAddr, dstAddr, targetAddr);
	}else if( !strcmp(dstAddr, targetAddr) )
	{
		setcolor(RESET_BG);
		// test
		//printf("\nsrcAddr:%s dstAddr:%s targetAddr:%s"
		//, srcAddr, dstAddr, targetAddr);
		memset(logData, 0x00, sizeof(logData));
		sprintf(logData, "destination address:%s target address:%s", srcAddr, targetAddr);
		printf("\n###=====>>>> Target address founed in dstAddr [%s]\n", dstAddr);
		LogIpDetection("testLog.log", logData, 0);
	}

	return;
}
