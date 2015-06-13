#ifndef KVETOSLAVOV_RUNTIME_CONTEXT_H
#define KVETOSLAVOV_RUNTIME_CONTEXT_H

#include <unistd.h>
#include <m_list.h>

struct context 
{
	struct m_list commands;
	int state;
	char** text_args;
	char* exec_path;
	pid_t pid;
	struct m_list breakpoints;
	struct m_list breakpoints_add;
	struct m_list breakpoints_remove;
};

#endif

