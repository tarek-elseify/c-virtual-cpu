# C Virtual CPU
This program creates a virtualized CPU in C. To do this, there is a priority queue 
of jobs going into the CPU. Once at the CPU, the job has an x% chance of exiting 
where x is a percentage provided in the config file. If the job does not exit, it 
is inserted into a queue for either disk 1 or disk 2, whichever is shorter. It will 
return to the CPU after being processed.
<br />
<br />
The config file specifies certain thresholds (i.e min and max amount of time spent at 
the CPU, probability of exit once at the CPU, job arrival time, etc.). The modification 
of these parameters will affect the calculations of the jobs including throughput, utilization, 
average response time, and max response time.
