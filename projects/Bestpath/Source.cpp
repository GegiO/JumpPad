#include <stdio.h>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;
using std::cin;
using std::cout;


// distance formula
//The first function finds the shortest distance as a value
//The finder function finds a path with the same value in reverse order, and puts it onto a stack. 
int hamlettocastle(int map[14][14][2][5], int start, int limit, int startdist)
{
	limit++;							//The limit value increments every function call. It's to make sure that the function doesn't go infinitely. 
	int tempdistance;
	int distance = 100000;

	if (limit > 10)						//The limit is set to 10. If the limit variable gets above 10
	{
		return 1000000;					//return a very high number
	}

	if (start != 4)						//check if the start = 4. 4 is the value of Bob the Knight's castle
	{
		for (int x = 0; x < 14; x++)	// loop through all 14 rows in that column of the array
		{
			for (int y = 0; y < 2; y++)	// loop through the two sub-columns of the array
			{
				if (map[start][x][y][0] == 1)	//check if there are values in the row of the start column 
				{
					tempdistance = hamlettocastle(map, x, limit, map[start][x][y][2]);	//If there are, call the function again. This time, start is changed to the row. 
					if (tempdistance <= distance)		//When the function returns, save the distance that path had in a temp variable. 
					{
						distance = tempdistance;		//If the saved distance is smaller than the older distance, make the distance variable = tempdistance
					}									//This is required to make sure the shortest path is taken. Every branch that reaches Bob's castle will return a distance
				}										//The shortest branch will be saved. 
			}
		}
	}
	
	if (distance == 100000)								//If start does = 4, the loop won't work. That means we are at our destination
	{
		distance = 0;									//make distance 0. If we are at our destination, distance can't be 100000. 
	}

	return startdist + distance;						//return the calculated distance. 
}

int HTCfinder(int map[14][14][2][5], int distance, int maxdist, int start, int limit, std::stack<int>& storage)
{
	limit++;
	int result;
	int sumdist;

	if (limit > 10)
	{
		return 0;
	}

	if (distance != maxdist)  //checks if the distance passed to the function is not the same distance as the max distance found in hamlettocastle()
	{
		for (int x = 0; x < 14; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				if (map[start][x][y][0] == 1)
				{
					sumdist = distance + map[start][x][y][2];		//This finds the distance taken on this path so far. 
					result = HTCfinder(map, sumdist, maxdist, x, limit, storage);	// The distance and the next location are passed to the recursive function
					if (result == 1)
					{
						storage.push(map[start][x][y][1]);			//If results are 1, it means that the recursive function found the solution. 
						return 1;									//Start pushing the routes(stored in map[start][x][y][1]) into the storage stack
					}

				}
			}
		}
	}
	else															//If the distance = maxdist, that means we found our spot. Return 1 to start adding to the stack. 
	{
		return 1;
	}

	return 0;														//If we haven't found the spot, return 0.

}

// danger formula
//These functions find the least dangerous path to Bob's castle. 
//Danger is made into a value by accounting for time lost due to exam. 
//Assuming each exam is 90 minutes, 90 is multiplied by the chance to have an exam. Then the same formula as above is used. 
//In the end, the smallest time lost due to exam is the safest. 
//It's nearly identical to the first two functions, so it shouldn't need notes. 
int hamlettocastleprb(int map[14][14][2][5], int start, int limit, int startdist)
{
	limit++;
	int tempdistance;
	int distance = 100000;
	int putval;

	if (limit > 10)
	{
		return 1000000;
	}

	if (start == 4) // The only exception is this. It's just a different way of checking if we got to the end point or not. 
	{
		return startdist;
	}

	for (int x = 0; x < 14; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			if (map[start][x][y][0] == 1)
			{
				putval = map[start][x][y][3] * .9; //The values in the array are stored as integers, so  a 50% chance would be 50, not 0.5. We need to multiply by 90 and divide by 100, or multiply by .9 
				tempdistance = hamlettocastleprb(map, x, limit, putval);
				if (tempdistance < distance)
				{
					distance = tempdistance;
				}
			}
		}
	}


	return startdist + distance;
}

int HTCfinderprb(int map[14][14][2][5], double distance, double maxdist, int start, int limit, std::stack<int>& storage)
{
	limit++;
	int result;
	int sumdist;

	if (limit > 10)
	{
		return 0;
	}

	if (distance == maxdist && start == 4)
	{
		return 1;
	}

	for (int x = 0; x < 14; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			if (map[start][x][y][0] == 1)
			{
				sumdist = distance + (map[start][x][y][3] * .9);
				result = HTCfinderprb(map, sumdist, maxdist, x, limit, storage);
				if (result == 1)
				{
					storage.push(map[start][x][y][1]);
					return 1;
				}
			}
		}
	}


	return 0;

}

// cheap formula
//The cheap formula works identically to the distance formula too. Instead of just calculating just distance however, it calculates the toll. 
//Each 10 dollars spent on tolls is worth 100 distance, so the results are distance + toll * 100
//The result is a path that costs no money and is only 5 units longer than a route that costs 10 dollars. 

int hamlettocastlecst(int map[14][14][2][5], int start, int limit, int startdist)
{
	limit++;
	int tempdistance;
	int distance = 100000;

	if (limit > 10)
	{
		return 1000000;
	}
	if (start != 12)
	{
		for (int x = 0; x < 14; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				if (map[start][x][y][0] == 1)
				{
					tempdistance = hamlettocastlecst(map, x, limit, map[start][x][y][2] + (map[start][x][y][4] * 10));
					if (tempdistance <= distance)
					{
						distance = tempdistance;
					}
				}
			}
		}
	}

	if (distance == 100000)
	{
		distance = 0;
	}

	return startdist + distance;
}

int HTCfindercst(int map[14][14][2][5], double distance, double maxdist, int start, int limit, std::stack<int>& storage)
{
	limit++;
	int result;
	int sumdist;

	if (limit > 4)
	{
		return 0;
	}

	if (distance == maxdist && start == 12)
	{
		return 1;
	}


	for (int x = 0; x < 14; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			if (map[start][x][y][0] == 1)
			{
				sumdist = distance + map[start][x][y][2] + (map[start][x][y][4] * 10);
				result = HTCfindercst(map, sumdist, maxdist, x, limit, storage);
				if (result == 1)
				{
					storage.push(map[start][x][y][1]);
					return 1;
				}
			}
		}
	}


	return 0;

}

// distance from NYC to grandma's house. 
// This is just another fancy way of asking for the shortest distance. 
// It works exactly like the first function, but its destination is different. 

int NYCtoG(int map[14][14][2][5], int start, int limit, int startdist)
{
	limit++;
	int tempdistance;
	int distance = 100000;

	if (limit > 10)
	{
		return 1000000;
	}

	if (start != 1)
	{
		for (int x = 0; x < 14; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				if (map[start][x][y][0] == 1)
				{
					tempdistance = NYCtoG(map, x, limit, map[start][x][y][2]);
					if (tempdistance <= distance)
					{
						distance = tempdistance;
					}
				}
			}
		}
	}

	if (distance == 100000)
	{
		distance = 0;
	}

	return startdist + distance;
}

int NYCTGfinder(int map[14][14][2][5], int distance, int maxdist, int start, int limit, std::stack<int>& storage)
{
	limit++;
	int result;
	int sumdist;

	if (limit > 10)
	{
		return 0;
	}

	if (distance != maxdist)
	{
		for (int x = 0; x < 14; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				if (map[start][x][y][0] == 1)
				{
					sumdist = distance + map[start][x][y][2];
					result = NYCTGfinder(map, sumdist, maxdist, x, limit, storage);
					if (result == 1)
					{
						storage.push(map[start][x][y][1]);
						return 1;
					}

				}
			}
		}
	}
	else
	{
		return 1;
	}

	return 0;

}

// Nunnery Trip
// To solve this problem recursively, another function call is added. The routearray vector is a vector with 14 spaces, each space representing a part of the map
// I chose a vector because it can be passed by value, while an array can only be passed by reference

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

int GTTANfinder(int map[14][14][2][5], int distance, int maxdist, int start, int limit, int prev, vector<int> routearray, std::stack<int>& storage)
{
	limit++;
	int result;
	int sumdist;
	bool checkarray;

	routearray.at(start) = 1;

	if (limit > 17)
	{
		return 0;
	}

	for (int z = 0; z < 14; z++)		//Again we see the checkarray value. T
	{
		if (routearray.at(z) == 0)
		{
			checkarray = false;
			break;
		}
		checkarray = true;
	}


	if (distance == maxdist && start == 11 && checkarray)	//Since we already found the smallest distance, now we can find which path corresponds to that distance. 
	{
		return 1;											//If we get the right distance, we're at the nunnery, and all the places in the routearray are marked as visited, we can start returning. 
	}


	for (int x = 0; x < 14; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			if (map[start][x][y][0] == 1)
			{
				sumdist = distance + map[start][x][y][2];
				result = GTTANfinder(map, sumdist, maxdist, x, limit, start, routearray, storage);
				if (result == 1)						//We started returning. Now, each of the places we visited before we fulfilled the conditions displayed in the comment before this one
				{
					storage.push(map[start][x][y][1]);	//will be put onto a stack. Again, we need to use the stack because outputting the function normally would give directions in reverse order. 
					return 1;
				}
			}
		}
	}


	return 0;

}




int main()
{
	std::stack <int> routes; // This stack will hold all the routes from the finder functions. 
	//The main array. It's Huge
	//each row and column represents a place. The ordering is vertical, so for example
	//0 is cardinal city, 1 is Grandma's house, 2, is three silos, 3 is south city, 4 is bob's castle, etc. 
	//Each array value of the outer 2d array holds an inner 2d array. 
	//in the inner 2d array, each row represents one path from that place to another. 
	//In each row, value 0 represents if there is a path, 1 is the route number, 2 is the distance, 3 is the dange, and 4 is the toll.
	int map[14][14][2][5] =	 
	{
		{ { /*0 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 1*/{ 1, 20, 30, 3, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 4*/{ 1, 21, 34, 10, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*0 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*0 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*0 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, }, 
		{ { /*1 0*/{ 1, 20, 30, 3, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 2*/{ 1, 18, 35, 2, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*1 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*1 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*1 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*2 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 1*/{ 1, 18, 35, 2, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 3*/{ 1, 17, 40, 5, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*2 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 6*/{ 1, 14, 80, 20, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*2 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*2 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*3 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 2*/{ 1, 17, 40, 5, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*3 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 6*/{ 1, 15, 35, 10, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*3 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*3 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*4 0*/{ 1, 21, 34, 10, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*4 5*/{ 1, 13, 50, 95, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 7*/{ 1, 10, 63, 25, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*4 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*4 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*5 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 4*/{ 1, 13, 50, 95, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*5 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 6*/{ 1, 12, 50, 95, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*5 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*5 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*6 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 2*/{ 1, 14, 80, 20, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 3*/{ 1, 15, 35, 10, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*6 5*/{ 1, 12, 50, 95, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 9*/{ 1, 11, 35, 1, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*6 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*6 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*7 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 4*/{ 1, 10, 63, 25, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*7 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 8*/{ 1, 9, 54, 10, 10 }, { 0, 0, 0, 0, 0 } }, { /*7 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*7 10*/{ 1, 8, 59, 10, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*7 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*8 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*8 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 7*/{ 1, 9, 54, 10, 10 }, { 0, 0, 0, 0, 0 } }, { /*8 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 9*/{ 1, 5, 61, 10, 10 }, { 1, 16, 135, 0, 0 } },
		  { /*8 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 11*/{ 1, 6, 51, 10, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*8 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*9 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*9 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 6*/{ 1, 11, 35, 1, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 8*/{ 1, 5, 61, 10, 10 }, { 1, 16, 135, 0, 0 } }, { /*9 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*9 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 11*/{ 1, 4, 100, 5, 0 }, { 0, 0, 0, 0, 0 } }, { /*9 12*/{ 1, 2, 70, 20, 10 }, { 0, 0, 0, 0, 0 } }, { /*9 13*/{ 1, 3, 25, 10, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*10 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*10 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 7*/{ 1, 8, 59, 10, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 9*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*10 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 11*/{ 1, 7, 41, 5, 10 }, { 0, 0, 0, 0, 0 } }, { /*10 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*10 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*11 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*11 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 8*/{ 1, 6, 51, 10, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 9*/{ 1, 4, 100, 5, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*11 10*/{ 1, 7, 41, 5, 10 }, { 0, 0, 0, 0, 0 } }, { /*11 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*11 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*12 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*12 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 9*/{ 1, 2, 70, 20, 10 }, { 0, 0, 0, 0, 0 } },
		  { /*12 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 12*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*12 13*/{ 1, 1, 50, 5, 0 }, { 0, 0, 0, 0, 0 } }, },
		{ { /*13 0*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 1*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 2*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 3*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 4*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*13 5*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 6*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 7*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 8*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 9*/{ 1, 3, 25, 10, 0 }, { 0, 0, 0, 0, 0 } },
		  { /*13 10*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 11*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 12*/{ 1, 1, 50, 5, 0 }, { 0, 0, 0, 0, 0 } }, { /*13 13*/{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } }, },
	};

	int selecter = 0; 
	int result;
	int formality;
	int temp;
	vector<int> vec(14, 0);


	while (-1)
	{
		cout << "select which problem you want to solve, or input 6 to exit" << endl << endl;
		cin >> selecter; // switch case for selecting which problem you want to solve

		switch (selecter)
		{
		case 1:
			result = hamlettocastle(map, 6, 0, 0);
			formality = HTCfinder(map, 0, result, 6, 0, routes);
			cout << "To get from your hamlet to Bob's castle with the shortest path, take the following routes in order." << endl << endl;
			break;
		case 2:
			result = hamlettocastleprb(map, 6, 0, 0);
			cout << result << endl;
			formality = HTCfinderprb(map, 0, result, 6, 0, routes);
			cout << "Take these routes in order to get from your hamlet to Bob's castle with the least chance of getting an exam." << endl << endl;
			break;
		case 3:
			result = hamlettocastlecst(map, 6, 0, 0);
			formality = HTCfindercst(map, 0, result, 6, 0, routes);
			cout << "Take these routes in order to get from your hamlet to NYC with no money and as little distance as possible." << endl << endl;
			break;
		case 4:
			result = NYCtoG(map, 12, 0, 0);
			formality = NYCTGfinder(map, 0, result, 12, 0, routes);
			cout << "To get from NYC to grandma's house in the fastest time possible, take these routes in order" << endl << endl;
			break;
		case 5:
			cout << "This could take a long time. Please be patient" << endl << endl;
			result = GetThyselftoanunnery(map, 1, 0, 0, 1, vec);
			cout << "Almost there. Just wait a bit longer" << endl << endl;
			formality = GTTANfinder(map, 0, result, 1, 0, 1, vec, routes);
			cout << "Take the following routes to get from Grandma's house to the Nunnery" << endl << "while vising every spot in Rutgers. All with the shortest distance" << endl << endl;
			break;
		default:
			cout << "wrong input" << endl << endl;
			break;
		}

		while (routes.empty() == false) // After the problem has been selected, the output will print. 
		{
			temp = routes.top();
			cout << "route " << temp << endl;
			routes.pop();
		}
		cout << endl << endl;
	}
	int x;
	cin >> x;
}