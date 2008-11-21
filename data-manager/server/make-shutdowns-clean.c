
//
// Copyright (C) 2008, Regents of the University of Colorado.
//


#define _GNU_SOURCE


#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "bionet-data-manager.h"




static void exit_signal_handler(int signal_number) {
    g_message("caught signal %d (%s), exiting\n", signal_number, strsignal(signal_number));
    g_main_loop_quit(bdm_main_loop);
}




static void exit_handler(void) {
}




void make_shutdowns_clean(void) {
    struct sigaction sa;

    atexit(exit_handler);

    // handle exit signals
    sa.sa_handler = exit_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) < 0) {
        g_critical("sigaction(SIGINT, ...): %s", strerror(errno));
        exit(1);
    }

    if (sigaction(SIGTERM, &sa, NULL) < 0) {
        g_critical("sigaction(SIGTERM, ...): %s", strerror(errno));
        exit(1);
    }


    //
    // If a client dies or the connection is lost somehow, writes will
    // cause the BDM to receive SIGPIPE, and the default SIGPIPE handler
    // terminates the process.  Not so good.  So we change the handler to
    // ignore the signal, and we explicitly check for the error when we
    // need to.
    //

    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &sa, NULL) < 0) {
        g_critical("error setting SIGPIPE sigaction to SIG_IGN: %s", strerror(errno));
        exit(1);
    }
}

