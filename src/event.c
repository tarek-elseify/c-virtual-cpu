/******************************************************************************
# file: /home/TU/tug35668/lab/cis_3207/lab_1/event.c
#
# Tarek Elseify
# January 14, 2019
# Lab 1 - Giorgio's Discrete Event Simulator
# CIS 3207 SEC 001
# tug35668@temple.edu
#
# This program contains the necessary struct and functions to handle events
******************************************************************************/

/* standard include statements */
/*                             */
#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"


/******************************************************************************
#   function: create_event
#
#   arguments: int init_time - time event was created
#              int id - job_id for the given event
#
#   returns: event *new - pointer to new event struct
#
#   this function mallocs and initializes an event and returns its 
#   address from the heap
******************************************************************************/
event *create_event(int init_time, int id)
{
  
  /* create the object on the heap */
  /*                               */
  event *new = (event *) malloc(sizeof(event));

  /* set the object attributes */
  /*                           */
  new->time = init_time;
  new->job_id = id;
  new->next = NULL;
  new->task = CPU_ARRIVAL;
  
  /* return the address from the heap */
  /*                                  */
  return new;
}
/*                 */
/* end of function */

/*****************************************************************************/
/*---------------------------------end of file-------------------------------*/
