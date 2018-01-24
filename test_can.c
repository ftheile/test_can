/** Inspired by the examples in
 * https://www.kernel.org/doc/Documentation/networking/can.txt
 */
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int s;

	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (s != -1) {
		struct sockaddr_can addr;
		addr.can_family = AF_CAN;
		addr.can_ifindex = if_nametoindex("can0");
		if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
			ssize_t nbytes;
			struct can_frame frame;
			nbytes = read(s, &frame, sizeof(frame));
			if (nbytes == sizeof(sizeof(frame))) {
				int i;
				printf("0x%X %d", frame.can_id, frame.can_dlc);
				for (i=0; i<frame.can_dlc; i++) {
					printf("0x%X ");
				}
				printf("\n");
			} else {
				perror("read() error");
			}
		} else {
			perror("bind() error");
		}
	} else {
		perror("socket() error");
	}

	return 0;
}
