/******************************************************************************
# file: /home/TU/tug35668/lab/cis_3207/lab_1/main.c
#
# Tarek Elseify
# January 14, 2019
# Lab 1 - Giorgio's Discrete Event Simulator
# CIS 3207 SEC 001
# tug35668@temple.edu
#
# This program is discrete events simulation that is used to model
# a simple machine
******************************************************************************/

/* standard include statements */
/*                             */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "simulation.h"

/* function prototypes are listed here */
/*                                     */
int gen_random(int min, int max);
void find_int(FILE *file);
void set_global_values(FILE *config);
int exit_from_cpu(float percent);
void process_cpu(event *task, queue *cpu_q, p_queue *q);
void process_disk(event *task, queue *disk_1, queue *disk_2, p_queue *event_q);

/* enumeration for device states */
/*                               */
enum DEVICE_STATUS {IDLE, BUSY};

/* global variables that will be used throughout the program */
/*                                                           */
int SEED;
int INIT_TIME;
int FIN_TIME;
int ARRIVE_MIN;
int ARRIVE_MAX;
float QUIT_PROB;
int CPU_MIN;
int CPU_MAX;
int DISK1_MIN;
int DISK1_MAX;
int DISK2_MIN;
int DISK2_MAX;

/* used to calculate statistics of the simulation */
/*                                                */
int MAX_SIZE_CPU = 0;
int MAX_SIZE_DISK_1 = 0;
int MAX_SIZE_DISK_2 = 0;
int TIME_BUSY_CPU = 0;
int TIME_BUSY_DISK_1 = 0;
int TIME_BUSY_DISK_2 = 0;
int MAX_RESPONSE_TIME_CPU = 0;
int MAX_RESPONSE_TIME_DISK_1 = 0;
int MAX_RESPONSE_TIME_DISK_2 = 0;
int NUM_JOB_COMPLETE_CPU = 0;
int NUM_JOB_COMPLETE_DISK_1 = 0;
int NUM_JOB_COMPLETE_DISK_2 = 0;
int RESPONSE_CPU = 0;
int RESPONSE_DISK_1 = 0;
int RESPONSE_DISK_2 = 0;

/* id used to identify jobs */
/*                          */
int ID = 0;

/* initial states of all devices are idle */
/*                                        */
int CPU_STATE = IDLE;
int DISK_1_STATE = IDLE;
int DISK_2_STATE = IDLE;

/* initialize the time */
/*                     */
int TIME_CURR = 0;

/* output file will be used throughout the program */
/*                                                 */
FILE *OUT_FILE;


/******************************************************************************
#   function: main
#
#   arguments: int argc - argument count
#              char *argv[] - commandline arguments 
#
#   returns: none
#
#   this is the main function of the program
******************************************************************************/
void main(int argc, char* argv[])
{

  /* if the user did not provide a config file */
  /*                                           */
  if(argc != 2){

    /* print error to user and exit */
    /*                              */
    printf("%s\n", "usage: simulation <config_file>");
    exit(0);
  }

  /* pass config file through the commandline and open to read */
  /*                                                           */
  FILE *config_file;
  config_file = fopen(argv[1], "r");

  /* display informational message */
  /*                               */
  printf(" .... opening %s ....\n", argv[1]);
  
  /* check to see if we couldn't open the file */
  /*                                           */
  if (config_file == (FILE *) NULL){

    /* print an error and exit */
    /*                         */
    printf("error opening file (%s)\n", argv[1]);
    exit(0);
  }

  /* set the output file results to write */
  /*                                      */
  OUT_FILE = fopen("sim_results.txt", "w");
  
  /* display informational message */
  /*                               */
  printf("%s\n", " .... setting global variables ....");
  
  /* set all the global variables */
  /*                              */
  set_global_values(config_file);

  /* seed the random number generator */
  /*                                  */
  srand(SEED);
  
  /* close the config file */
  /*                       */
  fclose(config_file);

  /* create the initial and final events */
  /*                                     */
  event *initial = create_event(0, ID);
  initial->task = SIM_START;
  event *final = create_event(FIN_TIME, -1);
  final->task = SIM_COMPLETE;
  
  /* we create the priority queue with these events */
  /*                                                */
  p_queue *priority_queue = create_priority_queue();
  insert(priority_queue, initial);
  insert(priority_queue, final);
  
  /* create a queue for CPU, Disk 1, Disk 2 */
  /*                                        */
  queue *CPU = create_queue();
  queue *DISK_1 = create_queue();
  queue *DISK_2 = create_queue();

  /* place holder */
  /*              */
  event *current;
  event *new;
  
  /* print out informational message */
  /*                                 */
  printf("%s\n", " .... writing to file ....");

  /* while the event queue is not empty, and the time < fin_time */
  /*                                                             */
  while((!is_empty(priority_queue)) && (TIME_CURR < FIN_TIME)){

    /* pop off the event from the queue */
    /*                                  */
    current = remove_min(priority_queue);
    
    /* sets the time */
    /*               */
    TIME_CURR = current->time;

    /* switch case */
    /*             */
    switch(current->task){

      /* if this is the start of the simulation */
      /*                                        */
      case SIM_START:

	/* create a new event with the init time */
	/*                                       */
	current = create_event(INIT_TIME, ++ID);
	current->task = CPU_ARRIVAL;
	insert(priority_queue, current);
	break;
	
      /* if this job is related to CPU */
      /*                               */
      case CPU_ARRIVAL:

	/* process the current job in the CPU */
	/*                                    */
	process_cpu(current, CPU, priority_queue);
	new = create_event(TIME_CURR +					\
			   gen_random(ARRIVE_MIN, ARRIVE_MAX), ++ID);
	insert(priority_queue, new);
	break;

      /* if this job has finished processing at the CPU */
      /*                                                */
      case CPU_FINISHED:

	process_cpu(current, CPU, priority_queue);
	break;

      /* if this job is arriving to disk */
      /*                                 */
      case DISK_ARRIVAL:
      case DISK_1_FINISHED:
      case DISK_2_FINISHED:

	/* process it in the disk */
	/*                        */
	process_disk(current, DISK_1, DISK_2, priority_queue);
	break;
	
      /* if this is the sim complete event */
      /*                                   */
      case SIM_COMPLETE:

	/* write to file */
	/*               */
	fprintf(OUT_FILE, "At time %d, the simulation completed\n", TIME_CURR);
	
	/* do nothing */
	/*            */
	break;
    }
  }

  /* display an informational message */
  /*                                  */
  printf("%s\n", " .... simulation complete ....");

  /* gets the total time */
  /*                     */
  int total_time = FIN_TIME - INIT_TIME;

  /* display statistical information */
  /*                                 */
  printf("\n%s\n", " MAX QUEUE SIZE");
  printf(" CPU: %d | ", MAX_SIZE_CPU);
  printf(" DISK_1: %d | ", MAX_SIZE_DISK_1);
  printf(" DISK_2: %d\n", MAX_SIZE_DISK_2);
  printf("%s\n", " UTILIZATION");
  printf(" CPU: %f | ", (1.0 * TIME_BUSY_CPU / total_time));
  printf(" DISK_1: %f | ", ((float)TIME_BUSY_DISK_1 /total_time));
  printf(" DISK_2: %f\n", (1.0 * TIME_BUSY_DISK_2 / total_time));
  printf("%s\n", " AVG RESPONSE TIME");
  printf(" CPU: %f | ", (float) RESPONSE_CPU/NUM_JOB_COMPLETE_CPU);
  printf(" DISK_1: %f | ", (float) RESPONSE_DISK_1/NUM_JOB_COMPLETE_DISK_1);
  printf(" DISK_2: %f\n | ", (float) RESPONSE_DISK_2/NUM_JOB_COMPLETE_DISK_2);
  printf("%s\n", " MAX RESPONSE TIME");
  printf(" CPU: %d | ", MAX_RESPONSE_TIME_CPU);
  printf(" DISK_1: %d | ", MAX_RESPONSE_TIME_DISK_1);
  printf(" DISK_2: %d\n", MAX_RESPONSE_TIME_DISK_2);
  printf("%s\n", " THROUGHPUT");
  printf(" CPU: %f | ", (float) NUM_JOB_COMPLETE_CPU / total_time);
  printf(" DISK_1: %f | ", (float) NUM_JOB_COMPLETE_DISK_1 / total_time);
  printf(" DISK_2: %f\n", (float) NUM_JOB_COMPLETE_DISK_2 / total_time);
}
/*                 */
/* end of function */


/******************************************************************************
#   function: gen_random
#
#   arguments: int min - minimum possible value to be generated
#              int max - maximum possible value to be generated
#
#   returns: int random - number x such that min <= x <= max
#
#   this function returns a random number between the min and max inclusive
******************************************************************************/
int gen_random(int min, int max)
{

  /* generate the random number between min and max */
  /*                                                */
  int random = (rand() % (max - min + 1)) + min;

  /* return the value */
  /*                  */
  return random;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: find_int
#
#   arguments: FILE *file - file pointer to config file
#
#   returns: none
#
#   this function parses through the config file to find next number
******************************************************************************/
void find_int(FILE *file)
{

  /* local variable */
  /*                */
  char c;

  /* while we have not reached the end of the file */
  /*                                               */
  while((c = fgetc(file)) != EOF){

    /* if we reached a space, we can discontinue search */
    /*                                                  */
    if(c == ' '){
      break;
    }
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: set_global_values
#
#   arguments: FILE *config - pointer to config file
#
#   returns: none
#
#   this function sets all the values to the global vars used in the program
******************************************************************************/
void set_global_values(FILE *config)
{

  /* continually find the next in and scan into proper address */
  /*                                                           */
  find_int(config);
  fscanf(config, "%d", &SEED);
  find_int(config);
  fscanf(config, "%d", &INIT_TIME);
  find_int(config);
  fscanf(config, "%d", &FIN_TIME);
  find_int(config);
  fscanf(config, "%d", &ARRIVE_MIN);
  find_int(config);
  fscanf(config, "%d", &ARRIVE_MAX);
  find_int(config);
  fscanf(config, "%f", &QUIT_PROB);
  find_int(config);
  fscanf(config, "%d", &CPU_MIN);
  find_int(config);
  fscanf(config, "%d", &CPU_MAX);
  find_int(config);
  fscanf(config, "%d", &DISK1_MIN);
  find_int(config);
  fscanf(config, "%d", &DISK1_MAX);
  find_int(config);
  fscanf(config, "%d", &DISK2_MIN);
  find_int(config);
  fscanf(config, "%d", &DISK2_MAX);
}
/*                 */
/* end of function */


/******************************************************************************
#   function: exit_from_cpu
#
#   arguments: float percent - percentage from QUIT_PROB
#
#   returns: 1 if this job will exit from the cpu
#            0 if this job will go on for I/O
#
#   this function returns a logical int representing whether or not
#   the job will exit from the cpu
******************************************************************************/
int exit_from_cpu(float percent)
{

  /* convert decimal to percent */
  /*                            */
  float actual = percent * 100.0;
  
  /* if the user gave an invalid password */
  /*                                      */
  if(actual < 0.0 || actual > 100.0){

    /* print out an error and exit */
    /*                             */
    printf("invalid quit probability %f\n", actual);
    exit(0);
  }

  /* generate a random number between 0.0 and 100.0 */
  /*                                                */
  float random = ((float)rand()/(float)(RAND_MAX)) * 100.0;
  
  /* if the random percentage generated is below the user given percent */
  /*                                                                    */
  if(random <= actual){

    /* return a true int */
    /*                   */
    return 1;
  }

  /* return false otherwise */
  /*                        */
  return 0;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: process_cpu
#
#   arguments: event *task - pointer to the current event to process
#              queue *cpu_q - pointer to the cpu queue
#              p_queue *event_q - pointer to event queue
#              
#   returns: none
#
#   this function handles an event and creates a new process to add
#   to the event queue
******************************************************************************/
void process_cpu(event *task, queue *cpu_q, p_queue *event_q)
{

  /* if this job is incoming to the CPU */
  /*                                    */
  if(task->task == CPU_ARRIVAL){

    /* write to file */
    /*               */
    fprintf(OUT_FILE, "job_%d arrives at the CPU at time %d\n",	\
	    task->job_id, task->time);

    /* insert the current task to the CPU queue */
    /*                                          */
    enqueue(cpu_q, task);    

    /* adjust max queue size if necessary */
    /*                                    */
    if(cpu_q->size > MAX_SIZE_CPU){

      /* set the new max size of the cpu */
      /*                                 */
      MAX_SIZE_CPU = cpu_q->size;
    }
  }

  /* if this is a completed CPU job */
  /*                                */
  else {

    /* flip the status of the CPU to idle */
    /*                                    */
    CPU_STATE = IDLE;

    /* process it in the CPU */
    /*                       */
    fprintf(OUT_FILE, "job_%d finshes at the CPU at time %d\n", \
	    task->job_id, task->time);

    /* if we are not exiting from the CPU */
    /*                                    */
    if(!exit_from_cpu(QUIT_PROB)){

      /* create a new event */
      /*                    */
      event *new = create_event(task->time, task->job_id);
      new->task = DISK_ARRIVAL;
            
      /* re-insert the task into the event queue */
      /*                                         */
      insert(event_q, new);
    }

    /* this job is complete */
    /*                      */
    else {

      /* write to file */
      /*               */
      fprintf(OUT_FILE, "job_%d exited from the CPU at time %d\n",	\
	    task->job_id, task->time);
    }

    /* free the task, we dont need it anymore */
    /*                                        */
    free(task);
  }

  /* if the cpu is not empty and not busy */
  /*                                      */
  if((!fifo_empty(cpu_q)) && (CPU_STATE == IDLE)){

    /* remove the event from the queue */
    /*                                 */
    event *to_process = dequeue(cpu_q);

    /* add a new event to the queue as CPU finished */
    /*                                              */
    event *finished_event = create_event(0, to_process->job_id);
    
    /* get a random int between cpu min/max */
    /*                                      */
    int random = gen_random(CPU_MIN, CPU_MAX);
    
    /* adjust the time of the finished event */
    /*                                       */
    finished_event->time = TIME_CURR + random;

    /* get the response time */
    /*                       */
    int response_time = finished_event->time - to_process->time;
    RESPONSE_CPU += response_time;

    /* if this is the maximum response time */
    /*                                      */
    if(response_time > MAX_RESPONSE_TIME_CPU){

      /* set the new max */
      /*                 */
      MAX_RESPONSE_TIME_CPU = response_time;
    }
    
    /* add the time to process to total time CPU was busy */
    /*                                                    */
    TIME_BUSY_CPU += random;
    
    /* flip the op to CPU_FINISHED */
    /*                             */
    finished_event->task = CPU_FINISHED;
    NUM_JOB_COMPLETE_CPU++;

    /* add the process to the event queue */
    /*                                    */
    insert(event_q, finished_event);
    
    /* flip the CPU status to busy */
    /*                             */
    CPU_STATE = BUSY;

    /* we no longer need the processed event */
    /*                                       */
    free(to_process);
  }  
}
/*                 */
/* end of function */


/******************************************************************************
#   function: process_disk
#
#   arguments: event *task - pointer to the current event to process
#              queue *disk_1 - pointer to the first disk
#              queue *disk_2 - pointer to the second disk
#              p_queue *event_q - pointer to event queue
#              
#   returns: none
#
#   this function handles a disk event and sends it back to the cpu
#   if complete
******************************************************************************/
void process_disk(event *task, queue *disk_1, queue *disk_2, p_queue *event_q)
{

  /* place holder variable */
  /*                       */
  int response_time;
  
  /* if this job is arriving to the disk */
  /*                                     */
  if(task->task == DISK_ARRIVAL){
    
    /* gets the size of each disk */
    /*                            */
    int size_1 = disk_1->size;
    int size_2 = disk_2->size;

    /* keep track of which disk it went to */
    /*                                     */
    int disk;
    
    /* if disk 1 is smaller than disk 2 */
    /*                                  */
    if(size_1 < size_2){

      /* add the event to disk_1 queue */
      /*                               */
      enqueue(disk_1, task);  
      disk = 1;
    }

    /* if disk 2 is smaller than disk 1 */
    /*                                  */
    else if(size_2 < size_1){

      /* add the event to disk_2 queue */
      /*                               */
      enqueue(disk_2, task);
      disk = 2;
    }

    /* if they are the same size */
    /*                           */
    else{

      /* create a random number 1 or 2 */
      /*                               */
      int random = gen_random(1,2);

      /* if 1 */
      /*      */
      if(random == 1){

	/* add to disk 1 */
	/*               */
	enqueue(disk_1, task);
	disk = 1;
      }else{

	/* add to disk 2 */
	/*               */
	enqueue(disk_2, task);
	disk = 2;
      }      
    }

    /* if the new size of disk 1 is the new max */
    /*                                          */
    if(disk_1->size > MAX_SIZE_DISK_1){

      /* adjust the new disk 1 size */
      /*                           */
      MAX_SIZE_DISK_1 = disk_1->size;
    }

    /* if the new size of disk 2 is the new max */
    /*                                          */
    if(disk_2->size > MAX_SIZE_DISK_2){

      /* adjust the new disk 2 size */
      /*                            */
      MAX_SIZE_DISK_2 = disk_2->size;
    }

    /* write to file */
    /*               */
    fprintf(OUT_FILE, "job_%d arrives at DISK_%d at time %d\n",	\
	    task->job_id, disk, task->time);
  }

  /* if this is a disk finished task */
  /*                                 */
  else {

    /* if this finished from disk 1 */
    /*                              */
    if(task->task == DISK_1_FINISHED){

      /* write to file */
      /*               */
      fprintf(OUT_FILE, "job_%d finished at DISK_1 at time %d\n",	\
	    task->job_id, task->time);

      /* disk 1 is now free */
      /*                    */
      DISK_1_STATE = IDLE;
    }
    else{

      /* write to file */
      /*               */
      fprintf(OUT_FILE, "job_%d finished at DISK_2 at time %d\n",	\
	    task->job_id, task->time);

      /* disk 2 is now free */
      /*                    */
      DISK_2_STATE = IDLE;
    }

    /* create a new event and send it to the CPU */
    /*                                           */
    event *new = create_event(task->time, task->job_id);
    new->task = CPU_ARRIVAL;
    insert(event_q, new);
    free(task);
  }

  /* we need to check if we can process something from disk 1 */
  /*                                                          */
  if((!fifo_empty(disk_1)) && (DISK_1_STATE == IDLE)){

    /* remove the event from the queue */
    /*                                 */
    event *to_process = dequeue(disk_1);
    
    /* create a new event to send back to the CPU */
    /*                                            */
    event *finished_one = create_event(0, to_process->job_id);

    /* generate a random int for between disk_1 min/max */
    /*                                                  */
    int random = gen_random(DISK1_MIN, DISK1_MAX);
    
    /* add cpu min and max to the event */
    /*                                  */
    finished_one->time = TIME_CURR + random;

    /* get the response time */
    /*                       */
    response_time = finished_one->time - to_process->time;
    RESPONSE_DISK_1 += response_time;

    /* if this is the maximum response time */
    /*                                      */
    if(response_time > MAX_RESPONSE_TIME_DISK_1){

      /* set the new max */
      /*                 */
      MAX_RESPONSE_TIME_DISK_1 = response_time;
    }

    /* add this time to time busy disk 1 */
    /*                                   */
    TIME_BUSY_DISK_1 += random;

    /* flip the op to DISK_1_FINISHED */
    /*                                */
    finished_one->task = DISK_1_FINISHED;
    NUM_JOB_COMPLETE_DISK_1++;
    
    /* add the process to the event queue */
    /*                                    */
    insert(event_q, finished_one);

    /* flip the disk 1 status to busy */
    /*                                */
    DISK_1_STATE = BUSY;
    free(to_process);
  }

  /* now we check to see if we can process something from disk 2 */
  /*                                                             */
  if((!fifo_empty(disk_2)) && (DISK_2_STATE == IDLE)){

    /* remove the event from the queue */
    /*                                 */
    event *to_process = dequeue(disk_2);

    /* create a new event to send back to the cpu */
    /*                                            */
    event *finished_two = create_event(0, to_process->job_id);

    /* generate a random int between disk_2 min/max */
    /*                                              */
    int random = gen_random(DISK2_MIN, DISK2_MAX);
    
    /* add cpu min and max to the event */
    /*                                  */
    finished_two->time = TIME_CURR + random;

    /* get the response time */
    /*                       */
    response_time = finished_two->time - to_process->time;
    RESPONSE_DISK_2 += response_time;

    /* if this is the maximum response time */
    /*                                      */
    if(response_time > MAX_RESPONSE_TIME_DISK_2){

      /* set the new max */
      /*                 */
      MAX_RESPONSE_TIME_DISK_2 = response_time;
    }

    /* add the time to process to disk 2 busy */
    /*                                        */
    TIME_BUSY_DISK_2 += random;
    
    /* flip the op to DISK_2_FINISHED */
    /*                                */
    finished_two->task = DISK_2_FINISHED;
    NUM_JOB_COMPLETE_DISK_2++;
    
    /* add the process to the event queue */
    /*                                    */
    insert(event_q, finished_two);
    
    /* flip the disk 2 status to busy */
    /*                                */
    DISK_2_STATE = BUSY;
    free(to_process);
  }
}
/*                 */
/* end of function */

/*****************************************************************************/
/*---------------------------------end of file-------------------------------*/
