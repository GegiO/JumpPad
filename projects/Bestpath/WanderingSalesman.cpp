#include <stdio.h>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;
using std::cin;
using std::cout;

int GetThyselftoanunnery(int map[14][14][2][5], int start, int limit, int startdist, int prev, vector<int> routearray)
{
	limit++;
	int tempdistance;
	int distance = 100000;
	bool checkarray;

	routearray.at(start) = 1;   //Start represents the area where the function currently is at. This command marks that area as visited in the routearray vector. 

	if (limit > 17)				//Limit is much higher for this one. It needs to be. The direction is much longer. 
	{
		return 1000000;
	}

	for (int z = 0; z < 14; z++)	// This checks if the routearray is full of 1's. 
	{
		if (routearray.at(z) == 0)
		{
			checkarray = false;
			break;
		}
		checkarray = true;			//If it is, checkarray is marked true. 
	}

	if (start == 11 && checkarray)	//If checkarray is true, and the start val is 11, that means we visited all places already and we're at the nunnery. That means we found one possible path. 
	{
		return startdist;			//Now we can start returning values for this branch of the recursive function. 
	}


	for (int x = 0; x < 14; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			if (map[start][x][y][0] == 1)
			{
				tempdistance = GetThyselftoanunnery(map, x, limit, map[start][x][y][2], start, routearray); //This works the same as the previous functions. 
				if (tempdistance < distance)
				{
					distance = tempdistance;		//Again, same as the previous functions. It only picks the shortest branch. 
				}
			}
		}
	}

	return startdist + distance;
}
