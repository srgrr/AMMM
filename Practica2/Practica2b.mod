/*********************************************
 * OPL 12.5.1.0 Model
 * Author: 1166978
 * Creation Date: 07/03/2017 at 09:11:04
 *********************************************/
int nTasks=...;
int nCPUs=...;
int K=...;
range T=1..nTasks;
range C=1..nCPUs;
float rt[t in T]=...;
float rc[c in C]=...;
dvar boolean x_tc[t in T, c in C];

minimize sum(c in C) rc[c] - sum(t in T) sum(c in C) x_tc[t, c]*rt[t];
subject to {
	// each task is assigned to at most one CPU
	forall(t in T)
		sum(c in C) x_tc[t,c] <= 1;
	// we have at least T-K assigned tasks
	// (i.e: we have rejected at most K tasks)
    sum(t in T, c in C) x_tc[t,c] >= nTasks-K;
	// no CPU capacity is exceeded
	forall(c in C)
		sum(t in T) rt[t]*x_tc[t, c] <= rc[c];
}