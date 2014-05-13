#include "runtime/util.h"

#include <kvm.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>

int
runtime_process_exists (pid_t pid)
{
	kvm_t *kvm;
	struct kinfo_proc *result;
	int count;

	kvm = kvm_open(0, 0, 0, O_RDONLY, NULL);
	result = kvm_getprocs(kvm, KERN_PROC_PID, pid, &count);
	kvm_close(kvm);

	if (result != NULL)
		return 0;
	else
		return 1;
}

