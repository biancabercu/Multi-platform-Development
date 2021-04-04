#ifndef XREADWRITE_UTIL_H
#define XREADWRITE_UTIL_H

#include <unistd.h>

ssize_t xread(int fd, void *buf, size_t count);
/*<0	eroare
>=0	numarul de biti cititi */
ssize_t xwrite(int fd, const void *buf, size_t count);
/*<0	eroare
>=0	numarul de biti scrisi*/
#endif
