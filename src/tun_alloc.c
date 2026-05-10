#include "tun_alloc.h"

#include <linux/if.h>
#include <linux/if_tun.h>

#include <string.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <sys/ioctl.h>

// https://backreference.org/2010/03/26/tuntap-interface-tutorial/
int tun_alloc(char *dev, int flags){
    // Open file
    struct ifreq ifr = {};

    char *clone_dev_name = "/dev/net/tun";

    int tap_fd = open(clone_dev_name, O_RDWR | O_CLOEXEC);

    if (tap_fd < 0) {
        error(-1, errno, "open(%s)", clone_dev_name);
    }

    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    ifr.ifr_flags = flags;

    int err = ioctl(tap_fd, TUNSETIFF, &ifr);

    if (err != 0) {
        error(-1, errno, "ioctl(TUNSETIFF)");
    }

    strcpy(dev, ifr.ifr_name);

    return tap_fd;
}