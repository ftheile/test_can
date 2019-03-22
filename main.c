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
	if (argc == 2) {
		int s;

		s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
		if (s != -1) {
			struct sockaddr_can addr;
			addr.can_family = AF_CAN;
			addr.can_ifindex = if_nametoindex(argv[1]);
			if (addr.can_ifindex != 0) {
				if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
					int n;
					for (n=0; n<10; n++) {
						struct can_frame frame;
						if (read(s, &frame, sizeof(frame)) == sizeof(frame)) {
							int i;
							printf("0x%X %d ", frame.can_id & CAN_EFF_MASK, frame.can_dlc);
							for (i=0; i<frame.can_dlc; i++) {
								printf("0x%.2X ");
							}
							printf("\n");
						} else {
							perror("read() error");
						}
					}
				} else {
					perror("bind() error");
				}
			} else {
				perror("if_nametoindex() error");
			}
		} else {
			perror("socket() error");
		}
	} else {
		printf("Help:\n");
		printf("  %s <interface>\n", argv[0]);
	}
	return 0;
}
