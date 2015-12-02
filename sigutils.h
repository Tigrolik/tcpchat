#ifndef _SIGUTILS_H_
#define _SIGUTILS_H_

#include "common.h"
#include <signal.h>
#include <sys/wait.h>

/* Function: sigchld_handler
 * -------------------------
 * handles signals
 */
void sigchld_handler();

#endif
