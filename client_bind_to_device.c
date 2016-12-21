#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#define BOUND_DEVICE "eth5"
#define DST_IP "11.4.3.31"
#define DST_PORT 4321

int main(int argc, char *argv[])
{
	int skfd;
	struct ifreq interface;
	char databuf[1024] = "Multicast test message lol!";
	int datalen = sizeof(databuf);
	struct sockaddr_in groupSock;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);

	strncpy(interface.ifr_ifrn.ifrn_name, BOUND_DEVICE, IFNAMSIZ);
	if (setsockopt(skfd, SOL_SOCKET, SO_BINDTODEVICE, (char*)&interface,
		sizeof(interface)) < 0) {
		printf("sendpacket: setting SO_BINDTODEVICE");
		exit(1);
	}

	memset((char*)&groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr(DST_IP);
	groupSock.sin_port = htons(DST_PORT);

	if (sendto(skfd, databuf, datalen, 0, (struct sockaddr*)&groupSock,
		sizeof(groupSock)) < 0) {
		printf("Sending datagram message error");
	}

	return 0;
}

