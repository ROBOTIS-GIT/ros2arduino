#include <uxr/client/profile/transport/udp/udp_transport_linux.h>
#include "udp_transport_internal.h"

#if defined(PLATFORM_NAME_LINUX)

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

bool uxr_init_udp_platform(uxrUDPPlatform* platform, const char* ip, uint16_t port)
{
    bool rv = false;

    /* Socket initialization */
    platform->poll_fd.fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 != platform->poll_fd.fd)
    {
        /* Remote IP setup. */
        struct sockaddr_in temp_addr;
        temp_addr.sin_family = AF_INET;
        temp_addr.sin_port = htons(port);
        temp_addr.sin_addr.s_addr = inet_addr(ip);
        platform->remote_addr = *((struct sockaddr *) &temp_addr);

        /* Poll setup. */
        platform->poll_fd.events = POLLIN;

        /* Remote address filter. */
        int connected = connect(platform->poll_fd.fd, &platform->remote_addr, sizeof(platform->remote_addr));
        rv = (0 == connected);
    }
    return rv;
}

bool uxr_close_udp_platform(uxrUDPPlatform* platform)
{
    return (0 == close(platform->poll_fd.fd));
}

size_t uxr_write_udp_data_platform(uxrUDPPlatform* platform, const uint8_t* buf, size_t len, uint8_t* errcode)
{
    size_t rv = 0;
    ssize_t bytes_sent = send(platform->poll_fd.fd, (void*)buf, len, 0);
    if (-1 != bytes_sent)
    {
        rv = (size_t)bytes_sent;
        *errcode = 0;
    }
    else
    {
        *errcode = 1;
    }
    return rv;
}

size_t uxr_read_udp_data_platform(uxrUDPPlatform* platform, uint8_t* buf, size_t len, int timeout, uint8_t* errcode)
{
    size_t rv = 0;
    int poll_rv = poll(&platform->poll_fd, 1, timeout);
    if (0 < poll_rv)
    {
        ssize_t bytes_received = recv(platform->poll_fd.fd, (void*)buf, len, 0);
        if (-1 != bytes_received)
        {
            rv = (size_t)bytes_received;
            *errcode = 0;
        }
        else
        {
            *errcode = 1;
        }
    }
    else
    {
        *errcode = (0 == poll_rv) ? 0 : 1;
    }
    return rv;
}


#endif