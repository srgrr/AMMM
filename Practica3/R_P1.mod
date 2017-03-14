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
dvar boolean x_tc[t in T, c in C];
dvar boolean x_hk[h in H, k in K];
dvar float+ z;

// Objective
minimize z;
subject to{
	// Constraint 1
	forall(h in H)
		sum(k in K) x_hk[h,k] == 1;
	// Constraint 2
	forall(t in T)
	  forall(c in C)
		sum(h in H: TH[t,h] == 1)
		  sum(k in K: CK[c,k] == 1)x_hk[h,k] == sum(h in H: TH[t,h] == 1) x_tc[h,c];
	// Constraint 3

}
 