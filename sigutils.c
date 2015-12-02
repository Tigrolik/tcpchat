#include "sigutils.h"

/* Function: sigchld_handler
 * -------------------------
 * handles signals
 */
void sigchld_handler()
{
	// waitpid() might overwrite errno => we save and restore it
	int saved_errno = errno;
	struct sigaction sa;

	while (waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;

	// reap all dead processes
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction()");
		exit(0);
	}
}
