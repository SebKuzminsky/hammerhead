
#include <stdio.h>

void usage(void) {
    printf("usage: speedway [OPTIONS] TARGET\n\
\n\
TARGET is the hostname or IP of a Speedway device.\n\
\n\
OPTIONS is zero or more of these:\n\
\n\
    --help            Show this help.\n\
\n\
    --id ID           Set the HAB-ID to ID.  Defaults to the local hostname\n\
                      if omitted.\n\
\n\
    --gpi-delay N     After a GPI1 event, wait N milliseconds before\n\
                      starting to scan for tags.  Defaults to 0.\n\
\n\
    --gpi-polarity N  Trigger a tag scan when the value of GPI1 becomes N.\n\
                      Valid values are 0 and 1.  Defaults to 1.\n\
\n\
    --num-scans N     After a GPI trigger, do a sequence of N scans.\n\
                      Defaults to 1.\n\
\n\
    --scan-idle N     When scanning for tags, terminate the scan early if\n\
                      N milliseconds pass without seeing a new tag.\n\
                      Defaults to 3000.\n\
\n\
    --scan-timeout N  A scan for tags may not go longer than N\n\
                      milliseconds, even if new tags keep being found.\n\
                      Defaults to 5000.\n\
\n\
    --show-messages   Enable printing of all messages between the HAB and\n\
                      the Speedway device.  Defaults to off.\n\
");
}
