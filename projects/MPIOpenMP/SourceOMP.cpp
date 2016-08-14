#include <stdio.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "omp.h"

extern "C"
{
	#include "basic.h"  //import basic.h, the clock program
}

using namespace std;
using std::cin;
using std::cout;

int ShortestDistance = 100000; //this is our programs critical section. Only 1 thread should access this at a time. 
int** inputArray;		//i put the array as a global item so we don't have to mess with passing it around in functions

void WSP(int start, int numofnodes, int currsum, vector<int> routearray){
	bool checkarray;
	int sum;
	bool Exit = false;

	routearray.at(start) = 1;		//routearray keeps a register on all the points in the array. If a point in the array has been visited already, mark it as 1 in the vector. 

	#pragma omp critical(distlock) 
	{
		if (currsum >= ShortestDistance){	//an instance of the recursive function ends if it gets bigger than ShortestDistance
			Exit = true;
		}
	}
	
	if (Exit)
	{
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
			#pragma omp critical(distlock)
			{
				if (currsum < ShortestDistance){
					ShortestDistance = currsum;
				}
			}
		}

		#pragma omp parallel for private(sum)  
		for (int x = 0; x < numofnodes; x++){								//This is where we'll need to do the parallelization.This part recursively searches through the whole matrix for 
			if (inputArray[start][x] != -1 && routearray.at(x) == 0){		//the shortest path. 
				sum = currsum + inputArray[start][x];
				WSP(x, numofnodes, sum, routearray);
			}
		}
}


int main(){
	int row = 0;
	int col = 0;
	int size;
	bool firstLine = true;
	string line;
	ifstream myfile("input_4");
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
	long starttime = Clock(); //the clock function was given to us on sakai. It's defined in the basic.c and basic.h files, and gives us the time in miliseconds. 
	WSP(0, size, 0, vec);
	long endtime = Clock();
	long TotalTime = endtime - starttime;
	printf("the minimum size is %d", ShortestDistance);
	cout << endl;
	cout << "that took " << TotalTime << " miliseconds" << endl;


	int x;					//i added this because i compiled in visual studio. VS closes the command prompt when the program ends normally. 
	cin >> x;
	return 0;

}

