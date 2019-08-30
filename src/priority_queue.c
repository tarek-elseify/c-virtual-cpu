/******************************************************************************
# file: /home/TU/tug35668/lab/cis_3207/lab_1/priority_queue.c
#
# Tarek Elseify
# January 14, 2019
# Lab 1 - Giorgio's Discrete Event Simulator
# CIS 3207 SEC 001
# tug35668@temple.edu
#
# This program contains the necessary functions to handle 
# the priority queue which uses a min heap method
******************************************************************************/

/* standard include statements */
/*                             */
#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"


/******************************************************************************
#   function: create_priority_queue
#
#   arguments: none
#
#   returns: new_p_queue - pointer to the allocated priority queue
#
#   this function initializes and builds a min heap 
******************************************************************************/
p_queue *create_priority_queue()
{

  /* allocate the new priority queue to the heap */
  /*                                             */
  p_queue *new_p_queue = (p_queue *) malloc(sizeof(p_queue));

  /* check to see if we failed to allocate */
  /*                                       */
  if(new_p_queue == NULL){

    /* send an error message and exit */
    /*                                */
    printf("%s\n", " .... WARNING: could not allocate memory for queue ....");
    exit(0);
  }

  /* return the pointer to the priority queue */
  /*                                          */
  return new_p_queue;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: insert
#
#   arguments: p_queue *q - priority queue pointer
#              event *new_event - pointer to new event to be added
#
#   returns: none
#
#   this function inserts a new event into the priority queue
******************************************************************************/
void insert(p_queue *q, event *new_event)
{

  /* we check to see if the queue is full */
  /*                                      */
  if(q->size == MAX_QUEUE_SIZE){

    /* send an error message and exit */
    /*                                */
    printf("%s\n", " .... WARNING: queue is full ....");
    exit(0);
  }

  /* we get the index of the last position */
  /*                                       */
  int index = q->size;
  
  /* we add the event to the end of the queue */
  /*                                          */
  q->events[index] = new_event;

  /* increment the size of the queue */
  /*                                 */
  q->size++;

  /* we let the event swim if it is smaller than its parents */
  /*                                                         */
  swim(q, index);
  
  /* if not sorted */
  /*               */
  if(!is_heap(q,0)){
    puts("insert caused the problem....");
    print_p_queue(q);
    exit(0);
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: remove_min
#
#   arguments: p_queue *q - priority queue pointer
#
#   returns: event *priority_event - pointer to the event with highest priority
#
#   this function returns the event with the highest priority
#   and adjusts the priorirty queue accordingly
******************************************************************************/
event *remove_min(p_queue *q)
{

  /* we store the very first event to return */
  /*                                         */
  event *priority_event = q->events[0];

  /* check to see if there is not a last event */
  /*                                           */
  if((q->events[q->size - 1] == NULL) || (is_empty(q))){

    /* we don't adjust anything */
    /*                          */
    return NULL;
  }

  /* get the last event */
  /*                    */
  event *last_event = q->events[q->size - 1];

  /* swap the first event with the last event */
  /*                                          */
  q->events[0] = last_event;

  /* last event is now null */
  /*                        */
  q->events[q->size - 1] = NULL;

  /* decrease the size of the queue */
  /*                                */
  q->size--;

  /* sink the root */
  /*               */
  sink(q, 0);

  /* if not sorted */
  /*               */
  if(!is_heap(q,0)){
    puts("remove min caused the problem....");
    print_p_queue(q);
    exit(0);
  }

  /* we return the pointer to the min event */
  /*                                        */
  return priority_event;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: is_empty
#
#   arguments: p_queue *q - pointer to a queue
#
#   returns: int (1 or 0 representing logical values)
#
#   this function returns true if a queue is empty, and false otherwise
******************************************************************************/
int is_empty(p_queue *q)
{

  /* return true if the size is equal to 0 */
  /*                                       */
  if((q->size == 0)){
    return 1;
  }
  return 0;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: is_full
#
#   arguments: p_queue *q - pointer to a queue
#
#   returns: int (1 or 0 representing logical values)
#
#   this function returns true if a queue is full, and false otherwise
******************************************************************************/
int is_full(p_queue *q)
{

  /* return true if the size is equal to MAX_QUEUE_SIZE */
  /*                                                    */
  if(q->size == MAX_QUEUE_SIZE){
    return 1;
  }
  return 0;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: swim
#
#   arguments: p_queue *q - priority queue pointer
#              int index - the index to swim from
#
#   returns: none
#
#   this function performs an O(log n) operation to have an event
#   flip with all its parent nodes if it is smaller than them 
******************************************************************************/
void swim(p_queue *q, int index)
{

  /* set the current node */
  /*                      */
  event *current = q->events[index];
  int current_index = index;
  
  /* gets the index of the parent node */
  /*                                   */
  int parent_index = (index - 1) / 2;

  /* placeholder variables */
  /*                       */
  int time_child;
  int time_parent;
  event *temp;
  
  /* while the current node is not null and the index >= 0 */
  /*                                                       */
  while((current != NULL) && (parent_index != current_index)){

    /* gets the time in for the current index (child) */
    /*                                                */
    time_child = q->events[current_index]->time;

    /* gets the time in for the parent index */
    /*                                       */
    time_parent = q->events[parent_index]->time;

    /* checks to see if the parent has a higher time than the child */
    /*                                                              */
    if(time_parent >= time_child){

      /* store the parent event in a temp var */
      /*                                      */
      temp = q->events[parent_index];

      /* make the parent index the child event */
      /*                                       */
      q->events[parent_index] = q->events[current_index];

      /* make the child index the parent event */
      /*                                       */
      q->events[current_index] = temp;
    }else{

      /* the queue is already sorted */
      /*                             */
      break;
    }

    /* gets the new parent and current index */
    /*                                       */
    current_index = parent_index;
    parent_index = (current_index - 1) / 2;
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: sink
#
#   arguments: p_queue *q - priority queue pointer
#              int index - the index to sink from
#
#   returns: none
#
#   this function performs an O(log n) operation to have an event
#   flip with all its child nodes if it is greater than them 
******************************************************************************/
void sink(p_queue *q, int index)
{

  /* value placeholders */
  /*                    */
  int left_child_index, right_child_index;
  int left_time, right_time, parent_time;
  event *temp;
  
  /* value used as index of current parent */
  /*                                       */
  int current_parent_index = index;

  /* gets the number of children */
  /*                             */
  int number_children = num_children(q, current_parent_index);

  /* while the current parent has at least one child */
  /*                                                 */
  while(number_children > 0){

    /* gets the left child index */
    /*                           */
    left_child_index = get_left_child_index(current_parent_index);

    /* gets the left and parent times */
    /*                                */
    left_time = q->events[left_child_index]->time;
    parent_time = q->events[current_parent_index]->time;

    /* hold the parent in a temp var */
    /*                               */
    temp = q->events[current_parent_index];
    
    /* if there are two children nodes */
    /*                                 */
    if(number_children == 2){

      /* gets the right index and time */
      /*                               */
      right_child_index = get_right_child_index(current_parent_index);
      right_time = q->events[right_child_index]->time;

      /* if the right time is smaller than the left and the parent */
      /*                                                           */
      if((right_time < left_time) && (right_time < parent_time)){

	/* swap the parent and the right child */
	/*                                     */
	q->events[current_parent_index] = q->events[right_child_index];
	q->events[right_child_index] = temp;
	current_parent_index = right_child_index;
	number_children = num_children(q, current_parent_index);
	continue;
      }
    }

    /* if the left time is smaller than the parent */
    /*                                             */
    if(left_time < parent_time){

      /* swap the parent and the left child */
      /*                                    */
      q->events[current_parent_index] = q->events[left_child_index];
      q->events[left_child_index] = temp;
      current_parent_index = left_child_index;
      number_children = num_children(q, current_parent_index);
    }

    /* if both the right and left are smaller */
    /*                                        */
    else{

      /* stop traversing down the tree */
      /*                               */
      break;
    }
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: num_children
#
#   arguments: p_queue *q - pointer to the priority queue
#              int index - the index of the parent node
#
#   returns: int representing number of children
#
#   this function returns the number of children a parent node has
******************************************************************************/
int num_children(p_queue *q, int index)
{

  /* gets the left,right child index */
  /*                                 */
  int left_child_index = get_left_child_index(index);
  int right_child_index = get_right_child_index(index);

  /* return the number of children depending on the bounds of the index */
  /*                                                                    */
  if((left_child_index <= q->size - 2) && (right_child_index <= q->size - 1)){
    return 2;
  }else if(left_child_index <= (q->size - 1)){
    return 1;
  }else{
    return 0;
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: get_left_child_index
#
#   arguments: int index - the index of the parent node
#
#   returns: int - the index of the left child node
#
#   this function returns the index of the left child node of 
#   a given parent node
******************************************************************************/
int get_left_child_index(int index)
{

  /* return the index */
  /*                  */
  return (2 * index) + 1;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: get_right_child_index
#
#   arguments: int index - the index of the parent node
#
#   returns: int - the index of the right child node
#
#   this function returns the index of the right child node of 
#   a given parent node
******************************************************************************/
int get_right_child_index(int index)
{

  /* return the index */
  /*                  */
  return (2 * index) + 2;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: print_p_queue
#
#   arguments: p_queue *q - pointer to the priority queue
#
#   returns: none
#
#   this function prints all the events in a priority queue
******************************************************************************/
void print_p_queue(p_queue *q)
{

  /* used as a count */
  /*                 */
  int i = 0;
  
  /* gets the starting position of the queue */
  /*                                         */
  event *current = q->events[i];

  /* while the event is not null */
  /*                             */
  while((current != NULL) && (i<q->size)){

    /* print the information of the event */
    /*                                    */
    printf("job id: %5d  |  time_in: %5d", current->job_id, current->time);
    switch(current->task){
    case CPU_ARRIVAL:
      printf("%s\n", " | CPU_ARRIVAL");
      break;
    case CPU_FINISHED:
      printf("%s\n", " | CPU_FINISHED");
      break;
    case DISK_ARRIVAL:
      printf("%s\n", " | DISK_ARRIVAL");
      break;
    case DISK_1_FINISHED:
      printf("%s\n", " | DISK_1_FINISHED");
      break;
    case DISK_2_FINISHED:
      printf("%s\n", " | DISK_2_FINISHED");
      break;
    case SIM_COMPLETE:
      printf("%s\n", " | SIM_COMPLETE");
      break;
    }
    
    /* go to the next event */
    /*                      */
    current = q->events[++i];
  }
}
/*                 */
/* end of function */

int is_heap(p_queue *q, int i)
{
  for(int i=0; i<=(q->size - 2)/2; i++){    
    if(((2*i) + 1) > (q->size - 2)){
      continue;
    }
    if(((2*i) + 2) > (q->size - 1)){
      continue;
    }
    if(q->events[(2*i)+1]->time < q->events[i]->time){
      return 0;
    }
    if(q->events[(2*i)+2]->time < q->events[i]->time){
      return 0;
    }
  }
  return 1;
}
/*****************************************************************************/
/*---------------------------------end of file-------------------------------*/
