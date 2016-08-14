#include <stdio.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "mpi.h"
extern "C"
{
	#include "basic.h"  //import basic.h, the clock program
}

		

using namespace std;
using std::cin;
using std::cout;

int local_rank, num_procs;
int ShortestDistance = 100000; //this is our programs critical section. Only 1 thread should access this at a time. 
int** inputArray;		//i put the array as a global item so we don't have to mess with passing it around in functions

void WSP(int start, int numofnodes, int currsum, int level, vector<int> routearray){
	bool checkarray;
	int sum;
	int numproc, rank, buffer = 0;
	float floatnumproc, floatnumnodes, floatX;
	level++;
	
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	floatnumproc = float(numproc);
	floatnumnodes = float(numofnodes);
	
	routearray.at(start) = 1;
						//routearray keeps a register on all the points in the array. If a point in the array has been visited already, mark it as 1 in the vector. 

	if (currsum >= ShortestDistance){	//an instance of the recursive function ends if it gets bigger than ShortestDistance
		return;
	}

	for (int x = 0; x < numofnodes; x++){	//checks if routearray is full of 1's
		if (routearray.at(x) == 0){
			checkarray = false;
			break;
		}
		checkarray = true;
	}

	if (checkarray == true){		//if routearray is full of 1's, and the distance the function traveled is less than the current shortest distance, update shortestdistance
		if (currsum < ShortestDistance){
			ShortestDistance = currsum;
			buffer = ShortestDistance;
			MPI_Send(&buffer, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
	}


	if(level == 1 && numproc >= 2)  //The first time each process enters the recursive function, assign them a chunk of the work
	  {								//if numproc < 2, there's only 1 process. There's no need to divide work. 
	    for(int x = 0; x<numofnodes; x++){
		floatX = float(x);
	      if(ceil((floatX+1)*(floatnumproc-1)/floatnumnodes) == float(rank)){	//This equation will find which processor needs to work with which X value in the for loop
		if (inputArray[start][x] != -1 && routearray.at(x) == 0){   
			sum = currsum + inputArray[start][x];
			WSP(x, numofnodes, sum, level, routearray);
			}
		}
	    }

	  }
	else
	  {
	for (int x = 0; x < numofnodes; x++){								//every recursive function after the first will go on normally. This code starts when the work has been divided. 
		if (inputArray[start][x] != -1 && routearray.at(x) == 0){	
			sum = currsum + inputArray[start][x];
			WSP(x, numofnodes, sum, level, routearray);
		}
	}
	  }
}


void master(long starttime){
	int buffer = 0;
	int buffer2 = 0;
	int numproc;
	MPI_Request request, request2;
	MPI_Status status, status2;
	int flag = -1, flag2 = -1;
	int finishcounter = 1;
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);

	while(1) //infinite while loop to accept inputs
	{	
		if(flag != 0)
		{
			MPI_Irecv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request); //nonblocking receive for the shortestdistance values found by the workers
			flag = 0;
		}
		
		if(flag2 != 0)
		{
			MPI_Irecv(&buffer2, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &request2); //nonblocking receive. Every time a worker finishes, a integer of 1 is received. 
			flag2 = 0;
		}
			
		MPI_Test(&request, &flag, &status);			//test both receives for any messages
		MPI_Test(&request2, &flag2, &status2);

		if(flag != 0)
		{
			if(status.MPI_SOURCE != -1)
				{
					if(buffer < ShortestDistance){ShortestDistance = buffer;}		//If the master receives a shortestdistance value from the worker that's less than the current value
				}																	//Then update the shortest value
			flag = -1;	
		}
		if(flag2 != 0)
		{
			if(status2.MPI_SOURCE != -1)
			{
				finishcounter += buffer2;				//If the master receives a message from a worker that finished, increment the finishcounter
			}
			flag2 = -1;
		}
		if(finishcounter == numproc)				//When all workers finish, start the finishing process of the master
		{
			long endtime = Clock();
        		long TotalTime = endtime - starttime;
			
			printf("the minimum size is %d", ShortestDistance);
	    		cout << endl;
	   		cout << "that took " << TotalTime << " miliseconds" << endl;
			break;
			
		}
	}
}

int main(int argc, char *argv[]){
	int row = 0;
	int col = 0;
	int size, rank, numproc;
	int buffer2 = 0;
	bool firstLine = true;
	string line;
	ifstream myfile("input_5");
	if (myfile.is_open()){
		while (getline(myfile, line)){
			//cout << line << '\n';
			if (firstLine){
				stringstream ss(line);
				ss >> size;
				inputArray = new int*[size];
				for (int i = 0; i < size; ++i)
					inputArray[i] = new int[size];
				firstLine = false;
			}
			else{
				stringstream ss(line);

				col = 0;
				while (col < size){
					ss >> inputArray[row][col];
					col++;
				}
				row++;
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file";


	vector<int> vec(size, 0);
	long starttime = Clock(); //the clock function was given to us on sakai. It's defined in the basic.c and basic.h files, and gives us the time in milliseconds.
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	if(numproc == 1)		//if only 1 processor exists, just run the function normally
	{
		WSP(0,size,0,0,vec);

		long endtime = Clock();
        long TotalTime = endtime - starttime;
			
		printf("the minimum size is %d", ShortestDistance);
	    	cout << endl;
	   	cout << "that took " << TotalTime << " milliseconds" << endl;

	}else if(rank != 0){		//If there's more than 1 processors, any processor that isn't rank 0 will go do work in the recursive function
		WSP(0, size, 0, 0,vec);
		buffer2 = 1;
		MPI_Send(&buffer2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	}else{						//Process 0 will go to the master function to receive messages from the workers
		master(starttime);
	}
	MPI_Finalize();
}

