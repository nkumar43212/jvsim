#include <sys/file.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>

int pid_lock (const char *filename);

static int
_pid_open (const char *filename)
{
    int fd;

    fd = open(filename, O_CREAT|O_RDWR, 0644);
    if (fd < 0) {
        printf("error opening %s for writing: %s", filename, strerror(errno));
        return -1;
    }

    if (flock(fd, LOCK_EX|LOCK_NB) < 0) {
        printf("unable to lock %s: %s", filename, strerror(errno));
        printf("is another copy of this program running?");
        close(fd);
        return -1;
    }

    return fd;
}

static int
pid_update (int fd)
{
    char pid_buf[11]; /* An int32 converts to 10 bytes */

    memset(pid_buf, '\0', sizeof(pid_buf));
    snprintf(pid_buf, sizeof pid_buf, "%d\n", getpid());

    if (lseek(fd, 0, SEEK_SET) < 0) {
        printf("%s: lseek: %s", __FUNCTION__, strerror(errno));
        return -1;
    }

    if (write(fd, pid_buf, strlen(pid_buf)) < 0) {
        printf("%s: write: %s", __FUNCTION__, strerror(errno));
        return -1;              /* failure */
    }

    return 0;                   /* success */
}

int
pid_lock (const char *filename)
{
    int fd;

    fd = _pid_open(filename);
    if (fd < 0) {
        return fd;
    }

    if (pid_update(fd) < 0) {
        close(fd);
        return -1;
    }

    return fd;
}
