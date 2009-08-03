#ifndef NEWLIB_H
#define NEWLIB_H

#ifndef _COMPILING_SUBSYS
#define _COMPILING_SUBSYS
#endif

#ifdef _EXFUN
#undef _EXFUN
#define _EXFUN(x, y)
#endif

#include <fcntl.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <sys/signal.h>
#include <sys/uio.h>

#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>

#include <stdarg.h>
#include <utime.h>

#include <sys/utsname.h>

#define SYS_SOCK_CONSTANTS_ONLY
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <poll.h>

#endif
