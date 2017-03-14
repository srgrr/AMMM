/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 14, 2017 at 10:01:48 PM
 *********************************************/
int numGroups = ...;
range G = 1..numGroups;
float profit[g in G] = ...;
float time[g in G] = ...;
int jobs[g in G] = ...;
float g7_penalty = ...;
float profit_ratio = ...;
float g2_extra_time = ...;
float g2_extra_profit = ...;
int g2_job_limit = ...;
float time_limit=...;

dvar int+ x[g in G];
dvar int+ x2p;
dvar boolean xp;
dvar boolean t2;

maximize (sum(g in G) profit_ratio * profit[g] * x[g]) + profit_ratio*g2_extra_profit*x2p -
g7_penalty*xp;

subject to {
	sum(g in G) time[g]*x[g] + g2_extra_time*x2p <= 720;
	forall(g in G) x[g] <= jobs[g];
	x2p <= g2_job_limit;
	
	x[7] >= xp;
	x[7] <= jobs[7]*xp;
	
	x[2] >= t2*jobs[2];
	x2p  <= t2*g2_job_limit;
}