
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/socket.h>

#include "cal.h"


#define Max(a, b) ((a) > (b) ? (a) : (b))


extern cal_pd_t cal_pd;
extern cal_i_t cal_i;  // FIXME: good god rename that


void cal_pd_callback(cal_event_t *event) {
    switch (event->event_type) {
        case CAL_EVENT_JOIN: {
            struct sockaddr_in *sin = (struct sockaddr_in *)&event->peer.addr;
            printf(
                "Join event from '%s' (%s:%hu)\n",
                event->peer.name,
                inet_ntoa(sin->sin_addr),
                ntohs(sin->sin_port)
            );

            if (strcmp(event->peer.name, "time-publisher") != 0) {
                break;
            }

            printf("found a time-publisher, subscribing to the time from it\n");
            cal_i.sendto(&event->peer, "hey there!", 11);
            cal_i.subscribe(&event->peer, "time");

            break;
        }

        case CAL_EVENT_LEAVE: {
            printf("Leave event from '%s'\n", event->peer.name);
            break;
        }

        default: {
            printf("unknown event type %d\n", event->event_type);
            break;
        }
    }

    cal_pd_free_event(event);
}


void cal_i_callback(cal_event_t *event) {
    printf("CAL-I callback\n");
    // cal_i_free_event(event);
}


int main(int argc, char *argv[]) {
    int cal_pd_fd = -1;
    int cal_i_fd = -1;

    while (1) {
        int max_fd;
        int r;
        fd_set readers;


        while (cal_pd_fd == -1) {
            cal_pd_fd = cal_pd.subscribe_peer_list(cal_pd_callback);
            if (cal_pd_fd < 0) {
                sleep(1);
                continue;
            }
        }


        while (cal_i_fd == -1) {
            cal_i_fd = cal_i.init_subscriber(cal_i_callback);
            if (cal_i_fd < 0) {
                sleep(1);
                continue;
            }
        }


	FD_ZERO(&readers);
        max_fd = -1;

	FD_SET(cal_pd_fd, &readers);
        max_fd = Max(cal_pd_fd, max_fd);

	FD_SET(cal_i_fd, &readers);
        max_fd = Max(cal_i_fd, max_fd);

	r = select(max_fd + 1, &readers, NULL, NULL, NULL);
	if (r == -1) {
	    printf("select fails: %s\n", strerror(errno));

            cal_pd.unsubscribe_peer_list();
            cal_pd_fd = -1;

            cal_i.cancel_subscriber();
            cal_i_fd = -1;

	    continue;
	}

        if (FD_ISSET(cal_pd_fd, &readers)) {
            if (!cal_pd.read()) {
                printf("error reading CAL-PD event!\n");
                cal_pd.unsubscribe_peer_list();
                cal_pd_fd = -1;
                continue;
            }
        }

        if (FD_ISSET(cal_i_fd, &readers)) {
            if (!cal_i.subscriber_read()) {
                printf("error reading CAL-I event!\n");
                cal_i.cancel_subscriber();
                cal_i_fd = -1;
                continue;
            }
        }

    }

    // NOT REACHED

    exit(0);
}

