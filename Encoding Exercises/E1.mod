/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 8, 2017 at 6:15:35 PM
 *********************************************/
int alloyCount=...;
range N=1..alloyCount;
float price[i in N]=...;
float typeA[i in N]=...;
dvar float+ x[i in N];

minimize sum(i in N) price[i]*x[i];

subject to {
	// Ratios should sum 1 (also, they must be nonnegative)
	sum(i in N) x[i] == 1.0;
	// Be sure that percentages hold
	sum(i in N) typeA[i]*x[i] == 0.3;
}