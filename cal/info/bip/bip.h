
#include <pthread.h>

#include "cal.h"


extern cal_i_t cal_i;


extern int bip_listening_socket;


typedef struct {
    cal_peer_t *peer;
    char *topic;
} bip_subscription_request_t;


//
// CAL-I API functions
//

int bip_init_publisher(cal_peer_t *this, void (*callback)(cal_event_t *event));
void bip_publish(char *topic, void *msg, int size);
int bip_publisher_read(void);

int bip_init_subscriber(void (*callback)(cal_event_t *event));
void bip_cancel_subscriber(void);
void bip_subscribe(cal_peer_t *peer, char *topic);
int bip_subscriber_read(void);

int bip_sendto(cal_peer_t *peer, void *msg, int size);




//
// internal helpers
//

#define Max(a, b) ((a) > (b) ? (a) : (b))

int bip_connect_to_peer(cal_peer_t *peer);


