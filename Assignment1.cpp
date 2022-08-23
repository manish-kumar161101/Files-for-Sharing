/*
Name: Manish Kumar
Roll Number: 119CS0864

C++ program to solve Traveling Salesman Problem using Branch and Bound
*/

#include <bits/stdc++.h>

using namespace std;

int N;

// Stores the path taken by the salesman.
int *final_path;

// Keeps track of the already visited nodes
bool *visited;

// Stores the final minimum weight of shortest tour.
int final_res = INT_MAX;

// Function to copy temporary solution to the final solution
void copyToFinal(int curr_path[])
{
	for (int i=0; i<N; i++)
	{
		final_path[i] = curr_path[i];
	}

	final_path[N] = curr_path[0];
}

// Function to find the minimum edge cost having an end at the vertex i
int MinEdge(int **adj, int i)
{
	int min = INT_MAX;
	for (int k = 0; k < N; k++)
	{
		if (adj[i][k]< min && i != k)
		{
			min = adj[i][k];
		}
	}

	return min;
}

// Function to find the second minimum edge cost having an end at the vertex i
int MinEdge2(int **adj, int i)
{
	int first = INT_MAX, second = INT_MAX;
	for (int j=0; j<N; j++)
	{
		if (i == j)
			continue;

		if (adj[i][j] <= first)
		{
			second = first;
			first = adj[i][j];
		}
		else if (adj[i][j] <= second && adj[i][j] != first)
		{
			second = adj[i][j];
		}
	}

	return second;
}

void displayFinalPath()
{
	for(int i = 0; i <= N; i++)
		printf("%d ", final_path[i]);
	
	printf("\n");
}

// function to find the lowest cost path
void FindSmallestPath(int **adj, int curr_bound, int curr_weight, int level, int curr_path[])
{
	// Base case when all nodes are covered
	if (level==N)
	{
		if (adj[curr_path[level-1]][curr_path[0]] != 0)
		{
			int curr_res = curr_weight + adj[curr_path[level-1]][curr_path[0]];

			if (curr_res < final_res)
			{
				copyToFinal(curr_path);
				final_res = curr_res;
			}
		}
		return;
	}

	// Interate for all vertices
	for (int i=0; i<N; i++)
	{
		// Consider next vertex if it is visited
		if (adj[curr_path[level-1]][i] != 0 && visited[i] == false)
		{
			int temp = curr_bound;
			curr_weight += adj[curr_path[level-1]][i];

			if (level==1)
			curr_bound -= ((MinEdge(adj, curr_path[level-1]) + MinEdge(adj, i))/2);
			else
			curr_bound -= ((MinEdge2(adj, curr_path[level-1]) + MinEdge(adj, i))/2);

			// Checking if curr lower bound is can lead to feasible solution
			if (curr_bound + curr_weight < final_res)
			{
				curr_path[level] = i;
				visited[i] = true;

				// call FindSmallestPath for the next level
				FindSmallestPath(adj, curr_bound, curr_weight, level+1, curr_path);
			}

			// Going back to previous node
			curr_weight -= adj[curr_path[level-1]][i];
			curr_bound = temp;

			memset(visited, false, sizeof(visited));
			
			for (int j=0; j<=level-1; j++)
			{
				visited[curr_path[j]] = true;
			}
		}
	}
}

// This function sets up final_path[]
void BranchAndBound(int **adj)
{
	int curr_path[N+1];

	int curr_bound = 0;

	memset(curr_path, -1, sizeof(curr_path));
	//memset(visited, 0, sizeof(curr_path));

	// Compute initial bound
	for (int i=0; i<N; i++)
	{
		curr_bound += (MinEdge(adj, i) + MinEdge2(adj, i));
	}

	curr_bound = (curr_bound & 1) ? curr_bound/2 + 1 : curr_bound/2;

	visited[0] = true;
	curr_path[0] = 0;

	// Call to FindSmallestPath for curr_weight equal to 0 and level 1
	FindSmallestPath(adj, curr_bound, 0, 1, curr_path);
}

int main()
{
    //Adjacency matrix for the given graph
    cout<<"Enter the number of cities: ";
    cin>>N;

    final_path = new int[N + 1];
    visited = new bool[N];

    int **adj = new int*[N];

    for(int i = 0; i < N; i++)
        adj[i] = new int[N];
    
    cout<<"Enter the adjacency matrix of the graph:"<<endl;
	for(int i = 0; i < N; i++)
        for(int j = 0; j <N; j++)
            cin>>adj[i][j];

	BranchAndBound(adj);

	printf("Minimum cost : %d\n", final_res);
	printf("Path Taken : ");
	displayFinalPath();

	return 0;
}
