/******************************************************************************
# file: /home/TU/tug35668/lab/cis_3207/lab_1/simulation.h
#
# Tarek Elseify
# January 14, 2019
# Lab 1 - Giorgio's Discrete Event Simulator
# CIS 3207 SEC 001
# tug35668@temple.edu
#
# This program contains the necessary struct and function protypes
# for the main program to use
******************************************************************************/

/* standard include statements */
/*                             */
#ifndef SIMULATION_H_
#define SIMULATION_H_
#include <stddef.h>

/* define the max queue size for all queues */
/*                                          */
#define MAX_QUEUE_SIZE 10000000

/* enumeration used to describe event type */
/*                                         */
typedef enum Op{
  SIM_START,
  CPU_ARRIVAL,
  CPU_FINISHED,
  DISK_ARRIVAL,
  DISK_1_FINISHED,
  DISK_2_FINISHED,
  SIM_COMPLETE,
}op;

/* struct for event containing relevent info */
/*                                           */
typedef struct Event
{
  int time;
  int job_id;
  struct Event *next;
  int task;  
}event;

/* struct for defining a queue */
/*                             */
typedef struct Queue
{
  event *head;
  event *tail;
  int size;
}queue;

/* struct for defining a priority queue */
/*                                      */
typedef struct Priority_Queue
{
  event *events[MAX_QUEUE_SIZE];
  int size;
}p_queue;

/* function prototype for the event struct */
/*                                         */
event *create_event(int init_time, int id);


/* function prototypes for the priority queue struct */
/*                                                   */
p_queue *create_priority_queue();
void insert(p_queue *q, event *new_event);
event *remove_min(p_queue *q);
int is_empty(p_queue *q);
int is_full(p_queue *q);
void swim(p_queue *q, int index);
void sink(p_queue *q, int index);
int get_left_child_index(int index);
int get_right_child_index(int index);
int num_children(p_queue *q, int index);
void print_p_queue(p_queue *q);
int is_heap(p_queue *q, int i);

/* function prototypes for the queue struct */
/*                                          */
queue *create_queue();
void enqueue(queue *q, event *new_event);
event *dequeue(queue *q);
void print_queue(queue *q);
void destroy(queue *q);
int fifo_empty(queue *q);

#endif // SIMULATION_H_

/*****************************************************************************/
/*---------------------------------end of file-------------------------------*/
