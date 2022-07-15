/**************************************************************************************************
    MADE BY: JEHAD KHALED HALAHLA
         ID: 1201467
        SEC: 1
 INSTRUCTOR:DR.AHMAD ABU SNENA
THIS PROJECT WAS INDIVISUAL EFFORT
**ONLY BASIC FUNCTIONS PROVIDED BY INSTRUCTOR WERE COPIED, EXCEPT FOR THE DELETE NODE FUNCTION**
                      ********** STARTED ON 10:42 AM 24/6/2022

                      ********** FINISHED ON 7:42 PM 24/6/2022
                  THANK YOU VERY MUCH FOR TAKING THE TIME TO EXAMINE THIS CODE!!
                             ****@ ALL RIGHTS RESERVED*****
**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define INFINITY 9999//infinity used
int V;//number of vertices
char uniqueVerticies[20][20];//stores all the unique vertices
int countUniqueVerticies(char uniqueVerticies[][20],int size);//fills the unique vertices array and counts V
void readFile(int graph[][V]);//builds the graph in an adjacency matrix and fills it with costs
int generateIndex(char city[20]);//gives each city an index
void dijkstra(int graph[][V], int src,int V,int dist[],int pathCost[],bool visitedList[],int parent[]);//finds the shortest path (single source)
int printSolution(int dist[], int n, int parent[],int pathCost[],char destination[]);//prints for a single source and single destination
void printPath(int parent[], int j,int pathCost[]);//prints the path fully with costs in each step
void printPathInFile(int parent[], int j,int pathCost[],FILE *outputFile);//same as above but for file
int minDistance(int dist[], bool visitedList[]);//minimum distance
char* indexToString(int n);//converts the strings into indecies
int printSolution2(int dist[], int n, int parent[],int pathCost[]);//prints without path
void menu();
void printGraph(int graph[][V]);//prints the matrix
int printSolutionInFile(int dist[], int n, int parent[],int pathCost[],char destination[],FILE *outputFile);//also prints in the file

int main()
{
    V = countUniqueVerticies(uniqueVerticies,20);
    int graph [V][V];
    int dist[V];//destination array
    int pathCost[V];//cost of each step
    bool visitedList[V];//visited or not using boolean
    int parent[V]; // Parent array to store shortest path tree in each time dijkistra is invoked
    FILE* outputFile = fopen("shortest_path.txt","w");
    for(int i = 0; i < V; i++)
    {
        //initiate Graph values at 0
        for(int j = 0 ; j < V ; j++)
        {
            graph[i][j]= 0;//initiates the array
        }
    }

    do
    {
        menu();
        int choice;
        char start[20];
        char dest[20];
        printf("please enter your choice:\n");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            readFile(graph);
            printGraph(graph);
            break;
        case 2:
            printf("please enter starting city..>\n");
            getc(stdin);
            gets(start);
            dijkstra(graph,generateIndex(start),V,dist,pathCost,visitedList,parent);
            printSolution2(dist,generateIndex(start),parent,pathCost);
            break;
        case 3:
            printf("please enter the specific destination:..>\n");
            getc(stdin);
            gets(dest);
            printSolution(dist,generateIndex(start),parent,pathCost,dest);

            break;
        case 4:
            printSolutionInFile(dist,generateIndex(start),parent,pathCost,dest,outputFile);
            exit(0);
            break;
        default:
            printf("not a valid option!\n");
        }
    }
    while(true);
    dijkstra(graph,0,V,dist,pathCost,visitedList,parent);


    return 0;
}




int countUniqueVerticies(char uniqueVerticies[][20],int size)
{
    FILE *ptr = fopen("cities.txt","r");
    char line[1000];
    int counter = 0 ;
    while(fgets(line,1000,ptr))
    {
        if(counter == 0)
        {
            strcpy(uniqueVerticies[0],strtok(line," "));
            counter++;
        }
        else
        {
            char city[20];
            int isUnique = 1;//it is unique
            strcpy(city,strtok(line," "));
            for(int i = 0 ; i < counter; i++  )
            {
                if(strcmp(city,uniqueVerticies[i])== 0)
                {
                    isUnique = 0 ;//if it occured once then it shouldn't be added
                }
            }
            if(isUnique)
            {
                strcpy(uniqueVerticies[counter],city);//add to vertices list
                counter++;
            }
        }
    }
    fclose(ptr);
    return counter;//number of unique vertices
}
void readFile(int graph [][V])
{
    FILE *ptr = fopen("cities.txt","r");
    int counter = 0;
    char line[1000];
    char from[20];
    char to[20];
    int cost;
    while(fgets(line,1000,ptr))
    {
        sscanf(line,"%[^ ] %[^ ] %d",from,to,&cost);
        graph[generateIndex(from)][generateIndex(to)] = cost;

    }
}

int generateIndex(char city[20]) //each verticii is given an index according to its order in unique array
{
    for(int i = 0 ; i < V; i++)
    {
        if(strcmp(city,uniqueVerticies[i])==0)
        {
            return i;
        }
    }
    return -1;
}


void dijkstra(int graph[][V], int src,int V,int dist[],int pathCost[],bool visitedList[],int parent[])
{
    // The output array. dist[i] will hold the shortest
    // distance from src to i

    // visitedList[i] will true if vertex i is included / in
    // shortest path tree or shortest distance from src to i
    // is finalized

    for(int i = 0; i < V ; i++)//clear it from previous run
        visitedList[i] = false ;

    for(int i = 0; i < V ; i++)//also clear parent list
        parent[i] = -1 ;

    // Initialize all distances as INFINITE
    for (int i = 0; i < V; i++)
    {
        dist[i] = INFINITY;
    }

    // Distance of source vertex to itself is 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++)
    {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in first iteration.
        int u = minDistance(dist, visitedList);
        // Mark the picked vertex as processed
        visitedList[u] = true;
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)
            // Update dist[v] only if is not in visitedList,
            // there is an edge from u to v, and total
            // weight of path from src to v through u is
            // smaller than current value of dist[v]
            if (!visitedList[v] && graph[u][v]
                    && dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
                pathCost[v] = graph[u][v];
            }
    }
    // print the constructed distance array

}
int minDistance(int dist[], bool visitedList[])
{
    // Initialize min value
    int min = INFINITY, min_index;
    for (int v = 0; v < V; v++)
        if (visitedList[v] == false && dist[v] <= min)
            //if its less and not visisted then replace it and take the index and value
            min = dist[v], min_index = v;
    return min_index;
}


int printSolution(int dist[], int n, int parent[],int pathCost[],char destination[])
{
    int src = n;
    printf("\n");
    printf("Vertex\t\t\t\tDistance\t\tPath");
    for (int i = 0; i < V; i++)
    {
        if(src != i && i ==generateIndex(destination))
        {
            printf("\n%s -> %-10s \t\t %d\t\t%s", indexToString(src), indexToString(i), dist[i], indexToString(src));
            printPath(parent, i,pathCost);
        }


    }
    printf("\n");
}

int printSolution2(int dist[], int n, int parent[],int pathCost[])
{
    int src = n;
    printf("Vertex\t\t\t\tDistance");
    for (int i = 0; i < V; i++)
    {
        if(src != i)
        {
            printf("\n%s -> %-10s \t\t %d", indexToString(src), indexToString(i), dist[i]);

        }


    }
    printf("\n");
}
void printPath(int parent[], int j,int pathCost[V])
{
    // Base Case : If j is source
    if (parent[j] == -1)
    {
        return;
    }

    printPath(parent, parent[j],pathCost);
    printf("->%d->%s", pathCost[j],indexToString(j));
}
char* indexToString(int n)
{
    return uniqueVerticies[n];

}
void menu()
{

    printf("1.load the file and construct the graph\n"
           "2.read the source city and compute the Dijkstra algorithm(single source shortest path)\n"
           "3.print the full route of the shortest path including the distance between each two cities and the total shortest cost\n"
           "4.print the information of step 3 to a file called shortest_path.txt and exit the program\n");

}
void printGraph(int graph[][V])
{
    printf("\n\n******FULLY CONSTRUCTED GRAPH*****\n\n");
    printf("\t   ");
    for(int i = 0 ; i < V ; i++)
    {
        printf("%-10s",indexToString(i));
    }
    printf("\n");

    for(int i = 0 ; i  < V  ; i ++)
    {
        printf("%-10s:",indexToString(i));
        for(int j = 0 ; j  < V  ; j ++)
        {
            printf(" %-8d ",graph[i][j]);
        }
        printf("\n\n");
    }

}

int printSolutionInFile(int dist[], int n, int parent[],int pathCost[],char destination[],FILE *outputFile)
{
    int src = n;
    fprintf(outputFile,"\n");
    fprintf(outputFile,"Vertex\t\t\t\tDistance\t\tPath");
    for (int i = 0; i < V; i++)
    {
        if(src != i && i ==generateIndex(destination))
        {
            fprintf(outputFile,"\n%s -> %-10s \t\t %d\t\t%s", indexToString(src), indexToString(i), dist[i], indexToString(src));
            printPathInFile(parent, i,pathCost,outputFile);
        }


    }
    printf("\n");
    fclose(outputFile);
}


void printPathInFile(int parent[], int j,int pathCost[],FILE *outputFile)
{
//recursevly it prints the path in wanted order from source to dest
// Base Case : If j is source
    if (parent[j] == -1)
    {
        return;
    }

    printPathInFile(parent, parent[j],pathCost,outputFile);
    fprintf(outputFile,"->%d->%s", pathCost[j],indexToString(j));
}

