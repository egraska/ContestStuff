#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "socket_util.h"

int openConnection(HostPort* location)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd = 0;
    int s = 0;

    /* Obtain address(es) matching host*/
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    s = getaddrinfo(location->host, location->port, &hints, &result);

    if (s == 0)
    {
        for (rp = result; rp != NULL; rp = rp->ai_next)
        {
            sfd = socket(rp->ai_family, rp->ai_socktype,
            rp->ai_protocol);

            if (sfd == -1)
            {
                continue;
            }

            if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            {
                break;
            }
            close(sfd);
        }

        if (rp == NULL)
        {
            /* No address succeeded */
            sfd = -1;
        }
    }

    freeaddrinfo(result);
    return sfd;
}
