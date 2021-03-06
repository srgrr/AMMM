/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 8, 2017 at 7:38:11 PM
 *********************************************/
int vertex_count=...;
range V=1..vertex_count;

float graph[i in V][j in V]=...;

dvar int auxiliary[v in V];
dvar boolean is_edge_used[i in V][j in V];

minimize sum(i in V, j in V) is_edge_used[i, j]*graph[i, j];

subject to {
	// simply put, each vertex should be visited once except maybe one
	// so, we must enter and leave each vertex exactly once
	forall(v in V)
	  sum(j in V) is_edge_used[v, j] == 1;
	forall(v in V)
	  sum(j in V) is_edge_used[j, v] == 1;
	// ...also, it is not valid to come and go from yourself :)
	forall(v in V)
	  is_edge_used[v, v] == 0;
	/*
		Consider this solution with two minor tours:
		1 -> 3 -> 2 ; 4 -> 6 -> 5
		note that 4 -> 6 -> 5
		imposes that aux[4] > aux[6] > aux[5] > aux[4] -> aux[4] > aux[4]
		which is a contradiction.
		Every tour induces a contradiction except for vertex 1. So, we can have
		at most a single tour
	*/
	forall(i in V : i > 1, j in V : j > 1)
	  auxiliary[i] - auxiliary[j] + vertex_count*is_edge_used[i, j] <= vertex_count - 1;
}