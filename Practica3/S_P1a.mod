/*********************************************
 * OPL 12.6.0.0 Model
 * Author: srodrig1
 * Creation Date: Mar 14, 2017 at 10:52:07 AM
 *********************************************/
int nTasks=...;
range T=1..nTasks;
int nThreads=...;
range H=1..nThreads;
int nCPUs=...;
range C=1..nCPUs;
int nCores=...;
range K=1..nCores;
float rh[h in H]=...;
float rc[c in C]=...;
int CK[c in C][k in K]=...;
int TH[t in T][h in H]=...;

dvar boolean x_tc[t in T][c in C];
dvar boolean x_hk[h in H][k in K];


// now z means "is computer c totally empty?"
// see constraints to see how this value is enforced
dvar boolean z[c in C];

maximize sum(c in C) z[c];

subject to {
	// each thread is assigned to exactly one core
	forall(h in H)
		sum(k in K) x_hk[h, k] == 1;
	// all threads from some task are assigned to the same CPU
	forall(t in T, c in C)
		sum(h in H : TH[t, h] == 1, k in K : CK[c, k] == 1) x_hk[h, k]
		==
		(sum(h in H) TH[t, h])*x_tc[t, c];
	// no core capacity is exceeded
	forall(c in C, k in K : CK[c,k] == 1)
		sum(h in H) rh[h]*x_hk[h, k] <= rc[c];
	// let's the impose the following exclusive OR:
	// 1) z_i is one
	// 2) sum of all x_tc for some t is zero
	forall(c in C)
		sum(t in T) x_tc[t, c] >= (1-z[c]);
	forall(c in C)
	  	sum(t in T) x_tc[t, c] <= (1-z[c])*nTasks;
	// Let's suppose that z[c] is true (1):
	// sum(t in T) x_tc[t, c] >= 0
	// sum(t in T) x_tc[t, c] <= 0 -> then, zero tasks must be assigned to this CPU
	// Let's suppose that z[c] is false (0):
	// sum(t in T) x_tc[t, c] >= 1
	// sum(t in T) x_tc[t, c] <= nTasks -> at least one (with no additional upper bound)
}