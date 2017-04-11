#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void forebackgrd()
{
	pid_t pgid;
	pgid = tcgetpgrp(STDIN_FILENO);
	if (pgid == -1)
	{
		printf("get foregroud process group ID failed!\n");
	}
	else if (pgid == getpgid(0))
	{
		printf("foreground process group\n");	
	}
	else
	{
		printf("background process group\n");	
	}

}

int main(int argc, char** argv)
{
	printf("STDIN_FILENO=%d\n", STDIN_FILENO);

	pid_t pgid;
	pgid = tcgetpgrp(STDIN_FILENO);
	printf("sid:%d, gid:%d, foreground gid:%d\n",getsid(0), getpgrp(), pgid);
	signal(SIGTTOU,SIG_IGN);

	forebackgrd();

	if (0 == tcsetpgrp(STDIN_FILENO, getpgrp()))
		forebackgrd();
	else
		printf("set foreground process failed\n");

	tcsetpgrp(STDIN_FILENO, pgid);//因为你修改了控制终端给你的进程组，如果当前进程组没有进程则进程组生命周期结束，控制终端释放，所以在程序结束前应该把控制终端还给原来的进程组
}
