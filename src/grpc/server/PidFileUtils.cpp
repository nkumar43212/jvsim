//
//  PidFileUtils.cpp
//  Telemetry Agent
//
//  Created: 6/15/16.
//
//  Authors: VIVEK M
//           ABBAS SAKARWALA
//
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include <sys/file.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>
#include "PidFileUtils.hpp"

AgentServerLog *_logger;

static int
_pid_open (std::string filename)
{
    std::string log_str;
    int fd;

    fd = open(filename.c_str(), O_CREAT|O_RDWR, 0644);
    if (fd < 0) {
        log_str = std::string(strerror(errno));
        _logger->log("<pid> Error opening " + filename + " for writing: " +
                     log_str);
        return -1;
    }

    if (flock(fd, LOCK_EX|LOCK_NB) < 0) {
        log_str = std::string(strerror(errno));
        _logger->log("<pid> Unable to lock " + filename + ": " + log_str);
        _logger->log("<pid> Is another copy of this program running ?");
        close(fd);
        return -1;
    }

    return fd;
}

static int
_pid_update (int fd)
{
    std::string log_str;
    char pid_buf[11]; /* An int32 converts to 10 bytes */

    memset(pid_buf, '\0', sizeof(pid_buf));
    snprintf(pid_buf, sizeof pid_buf, "%d\n", getpid());

    if (lseek(fd, 0, SEEK_SET) < 0) {
        log_str = std::string(strerror(errno));
        _logger->log("<pid> lseek: " + log_str);
        return -1;
    }

    if (write(fd, pid_buf, strlen(pid_buf)) < 0) {
        log_str = std::string(strerror(errno));
        _logger->log("<pid> write: " + log_str);
        return -1;
    }

    return 0;
}

int
pid_lock (std::string filename, AgentServerLog *logger)
{
    // First set the logger
    _logger = logger;

    int fd;

    // Open file
    fd = _pid_open(filename);
    if (fd < 0) {
        return fd;
    }

    // Update file with pid value
    if (_pid_update(fd) < 0) {
        close(fd);
        return -1;
    }

    return fd;
}
