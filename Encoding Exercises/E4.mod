/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 8, 2017 at 7:03:27 PM
 *********************************************/
/*
  This problem specification will diverge a bit from
  the original one because its statement was unclear
*/
int universe_size=...;
range N = 1..universe_size;

int subset_count=...;
range M = 1..subset_count;

/*
	This is another classical CS problem, google it!
*/

// someday: can this relation be represented in a more compact way?
int subset_contains_element[i in M][j in N]=...;

dvar boolean is_subset_chosen[i in M];

minimize sum(i in M) is_subset_chosen[i];

subject to {
	// each element of the universe appears at least in some subset
	forall(e in N)
	  sum(s in M) subset_contains_element[s,e]*is_subset_chosen[s] >= 1;
}