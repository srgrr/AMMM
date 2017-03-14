/*********************************************
 * OPL 12.6.0.0 Model
 * Author: ramela
 * Creation Date: 14/03/2017 at 8:23:29
 *********************************************/

int nTasks=...;
int nThreads=...;
int nCPUs=...;
int nCores=...;

range T=1..nTasks;
range C=1..nCPUs;
range H=1..nThreads;
range K=1..nCores;
float rh[h in H]=...;
float rc[c in C]=...;
int TH[t in T, h in H] = ...;
int CK[c in C, k in K] = ...;
//task t in computer c
dvar boolean x_tc[t in T, c in C];
//thread h in core k
dvar boolean x_hk[h in H, k in K];
dvar int computerUsed[c in C];
dvar int amountComputerUsed;

// Objective
minimize amountComputerUsed;
subject to{
	// Constraint 1
	forall(h in H)
		sum(k in K) x_hk[h,k] == 1;
	// Constraint 2
	forall(t in T)
	  forall(c in C)
		sum(h in H: TH[t,h] == 1)
		  sum(k in K: CK[c,k] == 1)x_hk[h,k] == sum(h in H: TH[t,h] == 1) x_tc[t,c];
	// Constraint 3
	forall(c in C)
	  forall(k in K:CK[c,k]==1)
	    sum(h in H) rh[h] * x_hk[h,k] <= rc[c];
	// Constraint 4
	forall(c in C)
	  forall(t in T)
	  	computerUsed[c] >= x_tc[t,c]; 
	// Constraint 5 
	amountComputerUsed >= sum(c in C) computerUsed[c];
}
 