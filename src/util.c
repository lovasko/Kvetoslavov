#include "util.h"

#include <signal.h>

void ignore_sigint()
{
	struct sigaction new_sa;
	struct sigaction old_sa;

	sigfillset(&new_sa.sa_mask);
	new_sa.sa_handler = SIG_IGN;
	new_sa.sa_flags = 0;

	if (sigaction(SIGINT, &new_sa, &old_sa) == 0 && old_sa.sa_handler != SIG_IGN)
	{
		sigaction(SIGINT, &new_sa, 0);
	}
}

