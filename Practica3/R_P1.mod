/*********************************************
 * OPL 12.6.0.0 Model
 * Author: ramela
 * Creation Date: 14/03/2017 at 8:23:29
 *********************************************/

int nTasks=...;
int nCPUs=...;
range T=1..nTasks;
range C=1..nCPUs;
float rt[t in T]=...;
float rc[c in C]=...;
dvar float+ x_tc[t in T, c in C];
dvar float+ z;

// Objective
minimize z;
subject to{
	// Constraint 1
	forall(t in T)
		sum(c in C) x_tc[t,c] == 1;
	// Constraint 2
	forall(c in C)
		sum(t in T) rt[t]* x_tc[t,c] <= rc[c];
	// Constraint 3
	forall(c in C)
		z >= (1/rc[c])*sum(t in T) rt[t]* x_tc[t,c];
}
 