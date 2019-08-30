# C Virtual CPU
This program creates a virtualized CPU in C. To do this, there is a priority queue <br />
of jobs going into the CPU. Once at the CPU, the job has an x% chance of exiting <br />
where x is a percentage provided in the config file. If the job does not exit, it <br />
is inserted into a queue for either disk 1 or disk 2, whichever is shorter. It will <br />
return to the CPU after being processed.
<br />
<br />
The config file specifies certain thresholds (i.e min and max amount of time spent at <br />
the CPU, probability of exit once at the CPU, job arrival time, etc.). The modification <br />
of these parameters will affect the calculations of the jobs including throughput, utilization, <br />
average response time, and max response time.
