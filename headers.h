#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "proclore.h"
#include "pastevents.h"
#include "seek.h"
#include "background.h"
#include "activities.h"
#include "ping.h"
#include "networking.h"
#include "terminal_setting.h"
#include "neonate.h"
#include "fgbg.h"
#include "redirection.h"
#include "helpers.h"

#endif