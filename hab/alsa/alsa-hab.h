
//
// Copyright (C) 2004-2008, Regents of the University of Colorado.
// This work was supported by NASA contracts NNJ05HE10G and NNC06CB40C.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of GNU General Public License version 2, as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
//

#ifndef __ALSA_HAB_H
#define __ALSA_HAB_H


#include <alsa/asoundlib.h>
#include <glib.h>

#include "hardware-abstractor.h"




// 
// the bionet audio format, expressed in ALSA terms
//

#define ALSA_CHANNELS (1)
#define ALSA_FORMAT   (SND_PCM_FORMAT_S16_LE)
#define ALSA_RATE     (9600)


// 
// other ALSA config info
//

#define ALSA_BUFFER_TIME_US (500 * 1000)
#define ALSA_PERIOD_TIME_US (100 * 1000)




// 
// local (to the alsa-hab) information, each stream gets one of these in
// its user_data pointer
//

typedef struct {
    char *device;
    GSList *clients;
    int socket;
} user_data_t;




//
// this holds info about an open alsa device
//

typedef struct {
    snd_pcm_t *pcm_handle;

    // these are for accessing the alsa stream
    // actually point into our big global list of pollfds, yuck
    struct pollfd *pollfd;
    int num_pollfds;

    uint buffer_time_us;
    snd_pcm_uframes_t buffer_size;

    uint period_time_us;
    snd_pcm_uframes_t period_size;

    int bytes_per_frame;
    int16_t *audio_buffer;
    int audio_buffer_frames;
} alsa_t;


//
// with ALSA Capture devices (aka bionet audio producers, aka microphones)
// we always wait for ALSA to produce some noise
//
// with ALSA Playback devices (aka bionet audio consumers, aka speakers) we
// wait for ALSA to consume the noise, unless there's no noise available
// from the client, in which case we pause ALSA and wait for the client to
// produce some noise for us
//

typedef enum {
    WAITING_FOR_CLIENT = 0,
    WAITING_FOR_ALSA
} who_are_we_waiting_for_t;


typedef struct {
    int socket;
    who_are_we_waiting_for_t waiting;
    alsa_t *alsa;
} client_t;




// each node (sound card) gets one of these
typedef struct {
    int hardware_is_still_there;
} node_user_data_t;




//
// The list of the Alsa-HAB's Nodes.
// There is one Node per device (soundcard).
//

extern GSList *nodes;


// 
// this hab
//

extern bionet_hab_t *this_hab;




void show_state(void);
void show_global_pollfds(void);
void show_client(client_t *client);

int discover_alsa_hardware(void);
alsa_t *open_alsa_device(char *device, snd_pcm_stream_t direction);
void close_alsa_device(alsa_t *alsa);
int xrun_handler(snd_pcm_t *handle, int err);
int check_alsa_poll(alsa_t *alsa);

void connect_client(bionet_stream_t *stream);
int handle_client(bionet_stream_t *stream, client_t *client);
void disconnect_client(bionet_stream_t *stream, client_t *client);




#endif // __ALSA_HAB_H
