#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//1.how to create a new session?
/*	pid_t setsid(void);
 *
 *  setsid() creates a new session if the calling process is not a process group leader.  The calling process is the leader of the new session, the
 *  process group leader of the new process group, and has no controlling tty.  The process group ID and session ID of the calling process  are  set
 *  to the PID of the calling process.  The calling process will be the only process in this new process group and in this new session.
 *
 * DESCRIPTION
 *        setsid()  creates a new session if the calling process is not a process group leader.  The calling process is the leader of the new session, the
 *        process group leader of the new process group, and has no controlling tty.  The process group ID and session ID of the calling process  are  set
 *        to the PID of the calling process.  The calling process will be the only process in this new process group and in this new session.
 *
 * RETURN VALUE
 *        The session ID of the calling process.
 *
 * ERRORS
 *        On  error,  -1 is returned, and errno is set.  The only error which can happen is EPERM. It is returned when the process group ID of any process
 *        equals the PID of the calling process. Thus, in particular, setsid() fails if the calling process is already a process group leader.
 *
 * NOTES
 *        A child created via fork(2) inherits its parent’s session ID.  The session ID is preserved across an execve(2).
 *
 *        A process group leader is a process with process group ID equal to its PID. In order to be sure that setsid() will succeed, fork()  and  exit(),
 *        and have the child do setsid().
 *
 */

int main(int argc, char** argv)
{
	pid_t pid;

	if ((pid=fork()) < 0)
	{
		printf("fork error\n");
		exit(1);
	}
	else if (pid == 0)
	{
		printf("child process ID:%d\n", getpid());
		printf("child process group ID:%d\n", getpgrp());
		printf("child process session ID:%d\n", getsid(getpid()));

		printf("child process new session ID:%d\n",setsid());
		printf("child process new group ID:%d\n", getpgrp());

		//printf("child process get father session ID:%d\n", getsid(getppid()));
	
	}
	else
	{
		printf("father process...\n");
		printf("father process ID:%d\n", getpid());
		printf("father process group ID:%d\n", getpgrp());
		printf("father process session ID:%d\n", getsid(getpid()));
	}

	return 0;
}

