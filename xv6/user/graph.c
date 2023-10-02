/*The following code is added by Rohith Jaina and Apeksha Padaliya
 *RXJ220013 and AXP210162 to get stats for plotting graph*/
#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#define no_of_Process 3

/*Spinning Function*/
void change()
{
  int i = 0;
  int j = 0;
  int k = 0;
	for(i = 0; i < 100; ++i)
	{
		for(j = 0; j < 50000; ++j)
		{
			k = j % 10;
      k++ ;
     }
	}
}

/*Print function to prunt Stats*/
void print(struct pstat *statistic, int* pid)
{
    int i;
    for(i = 0; i < NPROC; i++) {
		if (statistic->pid[i] == pid[0])
			printf(1, "pid = %d tickets= %d ticks= %d\n",statistic->pid[i], statistic->tickets[i], statistic->ticks[i]);
    else 
if (statistic->pid[i] == pid[1])
			printf(1, "pid = %d tickets=  %d ticks= %d\n",statistic->pid[i], statistic->tickets[i], statistic->ticks[i]);
    else 
if (statistic->pid[i] == pid[2])
			printf(1, "pid = %d tickets= %d ticks=  %d\n",statistic->pid[i], statistic->tickets[i], statistic->ticks[i]);
	}
}

int main(int argc, char *argv[])
{
  int pid[no_of_Process];
  int child, i;

  // fork no_of_Process(3) number of children and collect data about them on iteration basis
  for (child = 0; child < no_of_Process; child++) 
{
    pid[child] = fork();
    if (pid[child] == 0) {
      settickets((child+1)*10);
      for (i = 0; i < 100; i++)
        change();
      exit();
    }
  }

  struct pstat ps;
  int t;
  // This runs the process for 100 iterations and collects data for each iteration
  for (t = 0; t < 100; t++)
{
    getpinfo(&ps);
    //printf(1,"iteration %d \n",t);
    print(&ps, pid);
  }

  // This waits for all children to complete
	while(wait() > -1);

  exit();
}
