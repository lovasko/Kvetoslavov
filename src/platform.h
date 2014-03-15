#ifdef __FreeBSD__
	#include <machine/reg.h>
	#include <signal.h>
	
	#undef  PTRACE_TRACEME
	#define PTRACE_TRACEME PT_TRACE_ME
	
	#undef  PTRACE_SINGLESTEP	
	#define PTRACE_SINGLESTEP PT_STEP
	
	#undef  PTRACE_PEEKTEXT
	#define PTRACE_PEEKTEXT PT_READ_I
	
	#undef  PTRACE_POKETEXT
	#define PTRACE_POKETEXT PT_WRITE_I
	
	#undef  PTRACE_CONT
	#define PTRACE_CONT PT_CONTINUE
	
	#undef  PTRACE_ATTACH
	#define PTRACE_ATTACH PT_ATTACH
	
	#undef  PTRACE_DETACH
	#define PTRACE_DETACH PT_DETACH
	
	#undef  PTRACE_KILL
	#define PTRACE_KILL PT_KILL
	
	#undef  PTRACE_SETREGS
	#define PTRACE_SETREGS PT_SETREGS
	
	#undef  PTRACE_GETREGS
	#define PTRACE_GETREGS PT_GETREGS
	
	//typedef struct reg user_regs_struct;
	#define user_regs_struct reg
	#define INSTRUCTION_PTR(x) (x.r_eip)
	
	#define PTRACE_CONT_CONST ((caddr_t)1)
	
	#define PTRACE_CORRECT(x, y, z, w) (ptrace(x, y, w, z))
#elif __linux__
	#include <sys/user.h>
	#include <signal.h>
	#include <bits/sigaction.h>
	
	#define PTRACE_CONT_CONST 0
	
	#define INSTRUCTION_PTR(x) (x.rip)
	
	#define PTRACE_CORRECT(x, y, z, w) (ptrace(x, y, z, w))	
#else
	#error "unsupported build platform"
#endif