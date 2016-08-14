#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <sched.h>
#include <math.h>
#include <queue>

#include <pthread.h>
#include <semaphore.h>

using namespace std;

int schedtype = 0;             //initialize block types and sched type global variable
pthread_mutex_t scheduler_block;
pthread_cond_t cond_block;
pthread_mutex_t queue_block;           

int globaltime = 0;         

int FCFS_queue[50] = {0};    

int PBS_queue_tid[50] = {0};
int PBS_queue_tprio[50] = {0};

queue <int> MLPQ_prio1;
queue <int> MLPQ_prio2;
queue <int> MLPQ_prio3;
queue <int> MLPQ_prio4;
queue <int> MLPQ_prio5; 

int MLPQ_quantum[5] = {5, 10, 15, 20, 25};    //time quantum values for MLP queues
int MLPQ_ispresent[50] = {0};
int MLPQ_toptid = -1;


void init_scheduler(int sched_type)
{
  pthread_mutex_init(&scheduler_block, NULL);          //initializes the mutex variables and scheduling variable
  pthread_mutex_init(&queue_block, NULL);
  pthread_cond_init(&cond_block, NULL);
  schedtype = sched_type;
}

bool PBS_bubblesorter()    //bubble sort for priority queue. Returns true if the first value ever gets swapped. False if not. 
{
  int i, j;
  int temptid;
  int temptprio;
  int length = 0;
  int preempt_tid = PBS_queue_tid[0];
  while(PBS_queue_tid[length] != 0)
    {
      length++;
    }

  for(i = 1; (i <= length); i++)
    {
      for(j = 0; j < (length -1); j++)
	{
	  if(PBS_queue_tprio[j+1]<PBS_queue_tprio[j] && PBS_queue_tprio[j+1] != 0)
	    {
	      temptid = PBS_queue_tid[j];
	      temptprio = PBS_queue_tprio[j];
	      PBS_queue_tid[j] = PBS_queue_tid[j+1];
	      PBS_queue_tprio[j] = PBS_queue_tprio[j+1];
	      PBS_queue_tid[j+1] = temptid;
	      PBS_queue_tprio[j+1] = temptprio;
	    }
	}
    }

  if(PBS_queue_tid[0] == preempt_tid)
    {
      return false;
    }
  else
    {
      return true;
    }
}


int TopTidFinder()   //Finds the highest priorty first value of the multilevel queues 
{
  if(MLPQ_prio1.empty() == false)
    return MLPQ_prio1.front();
  
  if(MLPQ_prio2.empty() == false)
    return MLPQ_prio2.front();

  if(MLPQ_prio3.empty() == false)
    return MLPQ_prio3.front();

  if(MLPQ_prio4.empty() == false)
    return MLPQ_prio4.front();

  if(MLPQ_prio5.empty() == false)
    return MLPQ_prio5.front();
  else
    return -1;

}


int scheduleme(float currentTime, int tid, int remainingTime, int tprio)   
{
  globaltime = ceil(currentTime);
  bool already_in_queue = false;
  int x = 0;
  int y = 1;

  
  if(schedtype == 0 && remainingTime != 0)  //FCFS schedulerv --------------------------------------------------------------------------------------------------
    {                                       //If a process is running FCFS, it goes here
      pthread_mutex_lock(&queue_block);
      if(FCFS_queue[0] == 0)
	{
	  FCFS_queue[0] = tid;
	}
      while(FCFS_queue[x] != 0)
	{
	  if(tid == FCFS_queue[x])
	    {
	      already_in_queue = true;
	    }

	  x++;
	}
      if(already_in_queue == false)
	{
	  FCFS_queue[x] = tid;
	  x = 0;
	}
      pthread_mutex_unlock(&queue_block); //The above code inserts all the input values to our queue
 


      pthread_mutex_lock(&scheduler_block); 
      while(FCFS_queue[0] != tid)
	{
	  pthread_cond_wait(&cond_block, &scheduler_block);  // Only allows the thread with the frontmost Tid in the queue to enter
	 
	}
      pthread_mutex_unlock(&scheduler_block);
      return globaltime;

    }                                     //If the current thread contains the TID at the front of the thread, return that TID's thread. 
  else if(schedtype == 0 && remainingTime == 0)  // FCFS threads that run out of remaining time come here
    { 
      if(tid == FCFS_queue[0])                
	{
	  pthread_mutex_lock(&queue_block);
	  FCFS_queue[0] = 0;
	  int y = 1;
	  while(FCFS_queue[y] != 0)
	    {
	      FCFS_queue[y-1] = FCFS_queue[y];
	      FCFS_queue[y] = 0;
	      y++;
	    }
	  pthread_cond_broadcast(&cond_block);
	  pthread_mutex_unlock(&queue_block);
	}                                     //Removes the depleted thread from the queue and broadcasts a signal to the conditional variable
    }
  else if(schedtype == 1)           //STRF ---------------------------------------------------------------------------------------------------------------------------------
    {
      printf("This is the STRF scheduler. We are not required to do this.");
      return NULL;
    }      
  else if(schedtype == 2 && remainingTime != 0)  //PBS-----------------------------------------------------------------------------------------------------------------------
    {                                            // Priority based scheduler threads with time remaining go here
      pthread_mutex_lock(&queue_block);

      if(PBS_queue_tid[0] == 0)
	{
	  PBS_queue_tid[0] = tid;
	  PBS_queue_tprio[0] = tprio;
	}
      while(PBS_queue_tid[x] != 0)
	{
	  if(tid == PBS_queue_tid[x])
	    {
	      already_in_queue = true;
	    }

	  x++;
	}
      if(already_in_queue == false)
	{
	  PBS_queue_tid[x] = tid;
	  PBS_queue_tprio[x] = tprio; //The above code adds values into the queue
	  if(PBS_bubblesorter())      //Bubblesorter sorts the queue in order of highest to lowest priority. If the first value of the queue changes, bubblesorter returns true. 
	    {
	      pthread_cond_signal(&cond_block);  //If bubblesorter returns true, that means the first value in the queue has been interrupted by another higher priority value.
	    }

	  x = 0;
	}
      pthread_mutex_unlock(&queue_block);      

      pthread_mutex_lock(&scheduler_block); 
      while(PBS_queue_tid[0] != tid)                   //Only the thread with the TID at the front of the queue can pass this conditional variable
	{
	  pthread_cond_wait(&cond_block, &scheduler_block);
	}
      pthread_mutex_unlock(&scheduler_block);        


      return globaltime;

    }
  else if(schedtype == 2 && remainingTime == 0)      //PBS scheduled threads with no remaining time go here
    { 
      pthread_mutex_lock(&queue_block);
	  
      x = 0;
      while(PBS_queue_tid[x] != tid && PBS_queue_tid[x] != 0)
	{
	  x++;
	}
      PBS_queue_tid[x] = 0;
      PBS_queue_tprio[x] = 0;                       
      y = x + 1;
      while(PBS_queue_tid[y] != 0)
	{
	  PBS_queue_tid[y-1] = PBS_queue_tid[y];
	  PBS_queue_tid[y] = 0;
	  PBS_queue_tprio[y-1] = PBS_queue_tprio[y];
	  PBS_queue_tprio[y] = 0;

	  y++;
	}                                          //above code deletes the thread from the queue

      pthread_cond_broadcast(&cond_block);         //broadcasts to allow another value past the conditional variable
      pthread_mutex_unlock(&queue_block);
    }
  else if(schedtype == 3)
    {
      //----------------------------------------------------------------------------------------------------------------------------------------multilevel priority queue
      pthread_mutex_lock(&queue_block);
      x = 0;
      while(MLPQ_ispresent[x] != 0)
	{
	  if(tid == MLPQ_ispresent[x])
	    {
	      already_in_queue = true;
	    }
	  x++;
	}
      if(already_in_queue == false)
	{	      
	  MLPQ_prio1.push(tid);
	  MLPQ_ispresent[x] = tid;
	}                                          //Above code adds values to the top queue
      if(TopTidFinder() == MLPQ_prio1.front())     //topTidfinder finds first highest priority value. If topfinder is in the front of priority 1 queue, run the code below
	{
	  if(TopTidFinder() == tid && MLPQ_prio1.front() == tid)      
	    {
	      if(MLPQ_quantum[0] == 0 && remainingTime != 0)  // If the alloted time quantum runs out, 
	      {
		y = MLPQ_prio1.front();                       //save the front value
		MLPQ_prio1.pop();                             //remove the thread
		MLPQ_prio2.push(y);                           //and add it to the back of the next lowest queue
		MLPQ_quantum[0] = 5;                          //reset the time quantum for the next thread
	      }
	      else if(remainingTime == 0)
	      {
		MLPQ_prio1.pop();                             //if the threads remaining time runs out, remove it and reset time quantum. 
		MLPQ_quantum[0] = 5;
	      }
	    }
	}

      if(TopTidFinder() == MLPQ_prio2.front())    //Same as above, but with queue 2
	{
	  if(TopTidFinder() == tid && MLPQ_prio2.front() == tid)
		{
		  if(MLPQ_quantum[1] == 0 && remainingTime != 0)
		    {
		      y = MLPQ_prio2.front();
		      MLPQ_prio2.pop();
		      MLPQ_prio3.push(y);
		      MLPQ_quantum[1] = 5;
		    }
		  else if(remainingTime == 0)
		    {
		      MLPQ_prio2.pop();
		      MLPQ_quantum[1] = 10;
		    }
		}
	}
      if(TopTidFinder() == MLPQ_prio3.front())    //same as above but with queue 3
	{
	  if(TopTidFinder() == tid && MLPQ_prio3.front() == tid)
		{
		  if(MLPQ_quantum[2] == 0 && remainingTime != 0)
		    {
		      y = MLPQ_prio3.front();
		      MLPQ_prio3.pop();
		      MLPQ_prio4.push(y);
		      MLPQ_quantum[2] = 15;
		    }
		  else if(remainingTime == 0)
		    {
		      MLPQ_prio3.pop();
		      MLPQ_quantum[2] = 15;
		    }
		}
	}
      if(TopTidFinder() == MLPQ_prio4.front())     //same as above but with queue 4
	{
	  if(TopTidFinder() == tid && MLPQ_prio4.front() == tid)
		{
		  if(MLPQ_quantum[3] == 0 && remainingTime != 0)
		    {
		      y = MLPQ_prio4.front();
		      MLPQ_prio4.pop();
		      MLPQ_prio5.push(y);
		      MLPQ_quantum[3] = 20;
		    }
		  else if(remainingTime == 0)
		    {
		      MLPQ_prio4.pop();
		      MLPQ_quantum[3] = 20;
		    }
		}
	}
      if(TopTidFinder() == MLPQ_prio5.front())   //same as above but with queue 5
	{
	  if(TopTidFinder() == tid && MLPQ_prio5.front() == tid)
		{
		  if(MLPQ_quantum[4] == 0 && remainingTime != 0)
		    {
		      y = MLPQ_prio5.front();
		      MLPQ_prio5.pop();
		      MLPQ_prio5.push(y);     //if queue 5 runs out of time quantum, it goes to the back of queue 5 for a round robin effect
		      MLPQ_quantum[4] = 25;
		    }
		  else if(remainingTime == 0)
		    {
		      MLPQ_prio5.pop();
		      MLPQ_quantum[4] = 25;
		    }
		}
	}


      if(MLPQ_toptid == -1)
	{
	  MLPQ_toptid = TopTidFinder();    // Sets MLPQ_toptid, with saves the TID of the previous running thread. 
	}

      if(MLPQ_toptid != TopTidFinder())    // If a thread gets preempted, the code in this if statement will run
	{
	  if(tid == MLPQ_toptid)
	  {
	    pthread_cond_broadcast(&cond_block);   //the thread that's running will release the conditional variable
	  }
	  MLPQ_toptid = TopTidFinder();            //the MLPQ_toptid value will update
	}
      
      pthread_mutex_unlock(&queue_block);


      pthread_mutex_lock(&scheduler_block);
      while(tid != MLPQ_toptid)
	{
	  pthread_cond_wait(&cond_block, &scheduler_block);  // conditional variable only allows the highest first priority value to run
	}
      pthread_mutex_unlock(&scheduler_block);
      
      if(MLPQ_prio1.front() == tid)        //any thread that passes through here is running. Decrement its respective quantum values. 
	{
	  MLPQ_quantum[0]--;
	}
      if(MLPQ_prio2.front() == tid)
	{
	  MLPQ_quantum[1]--;
	}
      if(MLPQ_prio3.front() == tid)
	{
	  MLPQ_quantum[2]--;
	}
      if(MLPQ_prio4.front() == tid)
	{
	  MLPQ_quantum[3]--;
	}
      if(MLPQ_prio5.front() == tid)
	{
	  MLPQ_quantum[4]--;
	}
      

      return globaltime;
    }
}



