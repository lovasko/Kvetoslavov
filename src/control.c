#include "control.h"

#include "platform.h"

/**Common checking routine after calling wait().
 * @arg wait() result
 * @arg PID of the process
 * @arg head of breakpoint linked list
 * @return 0 if child exited, 1 if child stopped, -1 otherwise
 */
int after_wait(int wait_status, pid_t *pid, struct breakpoint **bp)
{
	char answer[2];
	pid_t child_pid;
	int ws;
	struct user_regs_struct regs;
	struct breakpoint *node;

	/* process-change handling routine */
	if (WIFEXITED(wait_status))
	{
		printf("Child exited.\n");
		return 0;
	}
	else if (WIFSTOPPED(wait_status))
	{
		#ifdef __linux__
			/* handle exec in traced process */
			if (wait_status >> 16 == PTRACE_EVENT_EXEC)
			{
				printf("Traced process is going to perform exec().\n");
				printf("Debugger is going to discard all breakpoints.\n");

				remove_all_breakpoints(bp);

				return -1;
			}

			/* handle fork in traced process */
			if (wait_status >> 16 == PTRACE_EVENT_FORK)
			{
				/* obtain child PID */
				ptrace(PTRACE_GETEVENTMSG, *pid, 0, &child_pid);

				printf("Traced process performed fork().\n");
				printf("PID of its child is %d.\n", child_pid);

				while (1)
				{
					printf("(s)tay in parent or follow (c)hild? ");
					fflush(stdout);

					read(0, &answer, 2);

					if (answer[0] == 's')
					{
						/* wait for the newly forked process, since it starts with SIGSTOP */
						wait(&ws);

						/* detach the child process */
						detach(child_pid, *bp);

						/* escape from another question */
						break;
					}
					else if (answer[0] == 'c')
					{
						/* since this options are not inherited by fork() */
						ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACEFORK | PTRACE_O_TRACEEXEC);

						/* wait for the newly forked process */
						wait(&ws);

						/* detach the old parent */
						detach(*pid, *bp);

						/* wait for old parent */
						wait(&ws);

						/* rewrite the current pid */
						*pid = child_pid;
						
						/* escape from another question */
						break;
					}
					else
					{
						printf("Wrong answer. Again: ");
						fflush(stdout);
					}
				}

				return -1;
			}
		#endif
		
		if (WSTOPSIG(wait_status) != SIGTRAP)
		{
			printf("Program received %s.\n", strsignal(WSTOPSIG(wait_status)));
		}
			
		PTRACE_CORRECT(PTRACE_GETREGS, *pid, 0, &regs);

		node = *bp;
		while (node != NULL)
		{
			if (INSTRUCTION_PTR(regs) - 1 == node->addr)
			{
				printf("Stopped at breakpoint in file %s at line %d. ", node->path, node->line);
				break;
			}
			node = node->next;
		}

		/* at end, but not at start either
		 * means we have breakpoints, but have not stopped
		 * on one of them */
		if (node == NULL && *bp != NULL)
		{
			printf("Stopped at unknown place. ");
			print_address(*pid, INSTRUCTION_PTR(regs));
		}

		printf("(IP = 0x%X)\n", INSTRUCTION_PTR(regs));
		
		return 1;
	}
	else
	{
		return -1;
	}
}

/**Start the selected program and insert breakpoints.
 * @arg head of linked list
 * @arg path to executable
 * @arg output parameter of started process's PID
 * @return 0 if program exited, 1 if program paused for some reason, -1 otherwise
 */
int start(struct breakpoint **bp, char *path, pid_t *pid)
{
	char *args[2];
	int wait_status;
	struct breakpoint *node;
	
	*pid = fork();
	if (*pid == 0)
	{
		/* if we are in the child, allow the tracing 
		 * rewrite the process with desired executable 
		 */
		ptrace(PTRACE_TRACEME, 0, 0, 0);
				
		args[0] = basename(path);
		args[1] = NULL;
		
		execv(path, args);
	}
	else
	{
		/* stop after first instruction */
		wait(&wait_status);

		#ifdef __linux__
			/* allow tracing of fork and exec */
			ptrace(PTRACE_SETOPTIONS, *pid, 0, PTRACE_O_TRACEFORK | PTRACE_O_TRACEEXEC);
		#endif
		
		/* insert breakpoints and save the original byte */
		node = *bp;
		while (node != NULL)
		{ 
			node->orig = ptrace(PTRACE_PEEKTEXT, *pid, node->addr, 0);

			/* replace first byte for 0xCC */
			node->oxcc = (node->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | 0xCC;
			ptrace(PTRACE_POKETEXT, *pid, node->addr, node->oxcc);
			
			node = node->next;
		}
		
		/* carry on */
		ptrace(PTRACE_CONT, *pid, PTRACE_CONT_CONST, 0);
		wait(&wait_status);
		return after_wait(wait_status, pid, bp);
	}
}

/**Resume paused process.
 * @arg head of linked list to all breakpoints
 * @arg PID to resume
 * @arg head of linked list of newly added breakpoints
 * @arg head of linked list of newly removed breakpoints
 * @return 0 if program exited, -1 if program paused for some reason, -1 otherwise
 */
int resume(struct breakpoint **bp, pid_t *pid, struct breakpoint *to_add, struct breakpoint *to_remove)
{
	int wait_status;
	struct user_regs_struct regs;
	struct breakpoint *node, *node_remove, *bp_node, *node_add;
	int is_breakpoint;
	
	/* if no breakpoints present, 
	 *     just simply continue */
	if (bp == NULL)
	{
		ptrace(PTRACE_CONT, *pid, PTRACE_CONT_CONST, 0);
		
		wait(&wait_status);
		return after_wait(wait_status, pid, bp);
	}
	
	/* handle breakpoints */
	PTRACE_CORRECT(PTRACE_GETREGS, *pid, 0, &regs);

	/* fix the breakpoint address with original instruction byte */
	node = *bp;
	
	is_breakpoint = 0; 
	
	while (node != NULL)
	{
		if (node->addr == INSTRUCTION_PTR(regs) - 1)
		{
			ptrace(PTRACE_POKETEXT, *pid, node->addr, node->orig);
			is_breakpoint = 1;
			break;
		}
		node = node->next;
	}
	
	/* do this only, if we were standing on a breakpoint */
	if (is_breakpoint == 1)
	{	
		/* move one instruction back */
		INSTRUCTION_PTR(regs) -= 1;
		PTRACE_CORRECT(PTRACE_SETREGS, *pid, 0, &regs);
		
		/* perform the original instruction and react to possible changes*/
		ptrace(PTRACE_SINGLESTEP, *pid, PTRACE_CONT_CONST, 0);	
		wait(&wait_status);
		
		/* check whether the single step caused anything */
		if (WIFEXITED(wait_status))
		{
			printf("Child exited on breakpoint in file %s at line %d.\n", node->path, node->line);
			return 0;
		}
		
		/* put the breakpoint instruction 0xCC back */
		ptrace(PTRACE_POKETEXT, *pid, node->addr, node->oxcc);
		
		/* remove breakpoints ought to be removed */
		node_remove = to_remove;
		while (node_remove != NULL)
		{
			/* find the same breakpoint in bp, because it has the valid orig field */
			bp_node = *bp;
			while (bp_node != NULL)
			{
				if (bp_node->addr == node_remove->addr)
				{
					ptrace(PTRACE_POKETEXT, *pid, bp_node->addr, bp_node->orig); 
				}
				
				bp_node = bp_node->next;
			}
			
			node_remove = node_remove->next;
		}
	}
	
	/* add new breakpoints */
	node_add = to_add;
	while (node_add != NULL)
	{
		node_add->orig = ptrace(PTRACE_PEEKTEXT, *pid, node_add->addr, 0);
		node_add->oxcc = (node_add->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | 0xCC;
		
		ptrace(PTRACE_POKETEXT, *pid, node_add->addr, node_add->oxcc);
		
		node_add = node_add->next;
	}
		
	/* carry on */
	ptrace(PTRACE_CONT, *pid, PTRACE_CONT_CONST, 0);
	wait(&wait_status);
	
	return after_wait(wait_status, pid, bp);
}

/**Attach to running process, and pause it.
 * @arg PID to attach to
 * @return 0 if attach was successful, 1 otherwise
 */
int attach(pid_t pid)
{
	int wait_status;
	
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == 0)
	{
		wait(&wait_status);
		return 0;
	}
	else 
	{
		return 1;
	}
}

/**Detach from running process.
 * @arg PID to detach from
 * @return 0 if detach was successful, 1 otherwise
 */
int detach(pid_t pid, struct breakpoint *bp)
{
	/* remove all breakpoints */
	struct breakpoint *node; 
	
	node = bp;
	while (node != NULL)
	{
		ptrace(PTRACE_POKETEXT, pid, bp->addr, node->orig);
		node = node->next;
	}
	
	if (ptrace(PTRACE_DETACH, pid, 0, 0) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**Send SIGKILL to process.
 * @arg PID
 * @return 0 if successful, 1 otherwise
 */
int stop(pid_t pid)
{
	if (ptrace(PTRACE_KILL, pid, 0, 0) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

