#include "main.h"

void DieWithError(char *errMsg, char *fileName)
{
  printf("usage: %s [-f] [pktFileName.pkt] [-t] [number of max packet]\n", fileName);
  perror(errMsg);
  exit(-1);
}

int main(int argc, char** argv)
{
	/* etc familiy */
	bool fileOption = false; // fileOption -- 1: save pkt file, otherwise save nothing.

	/* disp familiy */
	char errbuf[PCAP_ERRBUF_SIZE];
	int i;		// for general use
	int ndNum=0;	// number of network devices
	int devNum;	// device Id used for online packet capture

	pcap_dumper_t *pd;  // dump file pointer

	cpkNum = 0;  // Initialize global variable

	switch(argc)
	{
		case 1: // case of none-option
			maxPkt = MAXPKT;
			break;
		case 3: // case of unique option
			if(!strcmp(argv[1], "-f")) {
				strcpy(pktFileName, argv[2]);
				fileOption = true;
			}else if(!strcmp(argv[1], "-t")) {
				maxPkt = atoi(argv[2]);
			}else {
				DieWithError("there is no such option! check it again please.\n", *argv);
			}
			break;
		case 5: // case of two options
			if(!strcmp(argv[1], "-f") && !strcmp(argv[3], "-t")) {
				strcpy(pktFileName, argv[2]);
				fileOption = true;
				maxPkt = atoi(argv[4]);
			}else if(!strcmp(argv[1], "-t") && !strcmp(argv[3], "-f")) {
				strcpy(pktFileName, argv[4]);
				fileOption = true;
				maxPkt = atoi(argv[2]);
			}else {
				DieWithError("there is no such option! check it again please.\n", *argv);
			}
			break;
		default:
			DieWithError("there is no such option! check it again please.\n", *argv);

	}

    /* Retrieve the device list */
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    /* Print the list */
    for(d=alldevs; d; d=d->next)
    {
        printf("%d. %s", ++ndNum, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");

    }

    /* error ? */
    if(ndNum==0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return -1;
    }

    /* select device for online packet capture application */
    printf("Enter the interface number (1-%d):", ndNum);
    scanf("%d", &devNum);

    /* select error ? */
    if(devNum < 1 || devNum > ndNum)
    {
        printf("\nInterface number out of range.\n");
        /* Free the device list */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* Jump to the selected adapter */
    for(d=alldevs, i=0; i< devNum-1 ;d=d->next, i++);

    /* Open the adapter */
    if ( (adhandle= pcap_open_live(d->name,     // name of the device
                             SNAPLEN,           // captured packet size
                             1,                 // promiscous mode
                             1000,              // read timeout
                             errbuf             // error buffer
                             ) ) == NULL)
    {
        fprintf(stderr,"\nUnable to open the adapter. %s is not supported..\n",d->name);
        /* Free the device list */
        pcap_freealldevs(alldevs);
        return -1;
    }

    printf("\nselected device %s is available\n\n", d->description);

    /* At this point, we don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);
    pd = NULL;

    if(fileOption==true)
    {
	       pd = pcap_dump_open(adhandle, pktFileName);
	       pcap_loop(adhandle, -1, packet_info, (u_char *)pd) ;
    }else{
 	       pcap_loop(adhandle, -1, packet_info, NULL) ;
    }

    return 0;
}
