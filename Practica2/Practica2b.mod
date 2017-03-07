/*********************************************
 * OPL 12.5.1.0 Model
 * Author: 1166978
 * Creation Date: 07/03/2017 at 08:23:26
 *********************************************/
int nTasks=...;
int nCPUs=...;
range T=1..nTasks;
range C=1..nCPUs;
float rt[t in T]=...;
float rc[c in C]=...;
float K=...;
dvar boolean x_tc[t in T, c in C];
dvar boolean used[t in T];
dvar float+ z;
dvar float notUsed;

// Objective
minimize z + notUsed;
subject to{
	// Constraint 1
	// La suma de recursos assignats per a cada tasca és igual a 1
	forall(t in T)
		sum(c in C) x_tc[t,c] == used[t];
	// Constraint 2
	// Els recursos assignats a cada PC son mes petits que la seva capacitat
	forall(c in C)
		sum(t in T) rt[t]* x_tc[t,c] <= rc[c];
	// Constraint 3
	// Definicio de la funcio objectiu cosat carrega
	forall(c in C)
		z >= (1/rc[c])*sum(t in T) rt[t]* x_tc[t,c];
	// Constraint 4
	// Definicio de la funcio objectiu costat usats
	notUsed == nTasks - sum(t in T) used[t];
	// Constraint 5
	K >= notUsed;
}