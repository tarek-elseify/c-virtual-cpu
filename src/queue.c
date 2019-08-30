/******************************************************************************
# file: /home/TU/tug35668/lab/cis_3207/lab_1/queue.c
#
# Tarek Elseify
# January 14, 2019
# Lab 1 - Giorgio's Discrete Event Simulator
# CIS 3207 SEC 001
# tug35668@temple.edu
#
# This program contains the necessary functions to handle 
# the queues that will be used throughout the program
******************************************************************************/

/* standard include statements */
/*                             */
#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"


/******************************************************************************
#   function: create_queue
#
#   arguments: none
#
#   returns: queue *new_queue - pointer to initialized queue
#
#   this function returns a malloced initialized queue
******************************************************************************/
queue *create_queue()
{

  /* create a malloc queue */
  /*                       */
  queue *new_queue = (queue *) malloc(sizeof(queue));

  /* initialize values with 0 and null */
  /*                                   */
  new_queue->head = NULL;
  new_queue->tail = NULL;
  new_queue->size = 0;

  /* return the queue pointer */
  /*                          */
  return new_queue;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: enqueue
#
#   arguments: queue *q - pointer to a queue
#              event *new_event - pointer to the event to be enqueued
#
#   returns: none
#
#   this function enqueues a new event to a given queue
******************************************************************************/
void enqueue(queue *q, event *new_event)
{

  /* check to see if the queue is full */
  /*                                   */
  if(q->size == MAX_QUEUE_SIZE){

    /* we can not add any more events */
    /*                                */
    printf("%s\n", " .... WARNING: queue is full ....");
    exit(0);
  }

  /* if the queue is empty */
  /*                       */
  if(fifo_empty(q)){

    /* set both the head and tail as the new event */
    /*                                             */
    q->head = new_event;
    q->tail = new_event;
    q->size++;
  }

  /* if the queue is not empty */
  /*                           */
  else {

    /* let the tail's event's next point to this new event */
    /*                                                     */
    q->tail->next = new_event;

    /* set the tail to the next event */
    /*                                */
    q->tail = new_event;
    q->size++;
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: dequeue
#
#   arguments: queue *q - pointer to a queue
#
#   returns: event *ret - the event's address to be popped
#
#   this function returns the head of the queue
******************************************************************************/
event *dequeue(queue *q)
{

  /* if we try to dequeue an empty queue */
  /*                                     */
  if(fifo_empty(q)){

    /* return null */
    /*             */
    return NULL;
  }

  /* if the queue is not empty */
  /*                           */
  else {

    /* save the event in the head */
    /*                            */
    event *ret = q->head;

    /* if the head has a next */
    /*                        */
    if(q->head->next != NULL){

      /* set the head as the next element */
      /*                                  */
      q->head = q->head->next;
    }

    /* return the event */
    /*                  */
    q->size--;
    return ret;
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: print_queue
#
#   arguments: queue *q - pointer to a queue
#
#   returns: none
#
#   this function prints all the events in a queue
******************************************************************************/
void print_queue(queue *q)
{

  /* gets the starting position of the queue */
  /*                                         */
  event *current = q->head;

  /* while the event is not null */
  /*                             */
  while(current != NULL){

    /* print the information of the event */
    /*                                    */
    printf("job id: %d  |  time_in: %d\n", current->job_id, current->time);

    /* go to the next event */
    /*                      */
    current = current->next;
  }
}
/*                 */
/* end of function */


/******************************************************************************
#   function: destroy
#
#   arguments: queue *q - pointer to a queue
#
#   returns: none
#
#   this function destroys all events in a queue
******************************************************************************/
void destroy(queue *q)
{

  /* gets the starting position of the queue */
  /*                                         */
  event *current = q->head;

  /* ensure that head is NULL */
  /*                          */
  q->head = NULL;
  
  /* while the event is not null */
  /*                             */
  while(current != NULL){

    /* free the event */
    /*                */
    free(current);
    
    /* go to the next event */
    /*                      */
    current = current->next;
  }

  /* ensure the tail is NULL */
  /*                         */
  q->tail = NULL;
}
/*                 */
/* end of function */


/******************************************************************************
#   function: fifo_empty
#
#   arguments: queue *q - pointer to a queue
#
#   returns: int (1 or 0)
#
#   this function returns a logical int value representing if 
#   a queue is empty or not
******************************************************************************/
int fifo_empty(queue *q)
{

  /* if the size of the queue is 0 */
  /*                               */
  if(q->size == 0){

    /* the queue is empty */
    /*                    */
    return 1;
  }

  /* else, it is not empty */
  /*                       */
  return 0;
}
/*                 */
/* end of function */

/*****************************************************************************/
/*---------------------------------end of file-------------------------------*/
