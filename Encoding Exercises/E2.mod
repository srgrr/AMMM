/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 8, 2017 at 6:33:09 PM
 *********************************************/
int sourceCount=...;
range S=1..sourceCount;

int plantCount=...;
range P=1..plantCount;

int marketCount=...;
range M=1..marketCount;


float source_avail[i in S]=...;
float market_require[i in M]=...;
float price_from_source_to_plant[i in S][j in P]=...;
float price_from_plant_to_market[i in P][j in M]=...;

// amount is in KGs!
dvar float+ amount_from_source_to_plant[i in S][j in P];
dvar float+ amount_from_plant_to_market[i in P][j in M];

// obj function is just to minimize the sum of all prices that appear on our model
minimize sum(i in S, j in P) price_from_source_to_plant[i,j]*amount_from_source_to_plant[i,j] + 
         sum(i in P, j in M) price_from_plant_to_market[i,j]*amount_from_plant_to_market[i,j];

         
subject to {
	// since plants have unlimited processing capacities, let's move
	// all of our resources from sources to plants
	forall(s in S)
	  sum(p in P) amount_from_source_to_plant[s, p] == source_avail[s];
	// as happen in maxflows, the amount of resources that enter in a plant
	// must equal to the amount of resources that leave that plant
	forall(p in P)
	  sum(s in S) amount_from_source_to_plant[s, p] == sum(m in M) amount_from_plant_to_market[p, m];
	// required amount by markets are satisfied
	forall(m in M)
	  sum(p in P) amount_from_plant_to_market[p, m] >= market_require[m];
}