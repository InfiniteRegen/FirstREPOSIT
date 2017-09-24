capture:		capture.o oncap_disp.o shell_color.o statisticsHandler.o checkIPAddress.o
	gcc -o capture capture.o oncap_disp.o shell_color.o statisticsHandler.o checkIPAddress.o -lpcap

capture.o: capture.c
		gcc -c capture.c

oncap_disp.o: oncap_disp.c
		gcc -c oncap_disp.c

shell_color.o: shell_color.c
		gcc -c shell_color.c

statisticsHandler.o: statisticsHandler.c
		gcc -c statisticsHandler.c

checkIPAddress.o: checkIPAddress.c
		gcc -c checkIPAddress.c

clean:
	rm -rf *.o capture *.pkt

clear:
	rm -rf *.o capture *.pkt
