/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 8, 2017 at 6:53:59 PM
 *********************************************/
int itemAmount=...;
range N=1..itemAmount;
float value[i in N]=...;
float weight[i in N]=...;
float maximum_weight=...;

dvar boolean is_item_in[i in N];
/*
  This is a very classical problem in computer science, google it!
*/
maximize sum(i in N) value[i]*is_item_in[i];

subject to {
	// the total weight of our chosen items cannot exceed the
	// maximum_weight threshold
	sum(i in N) weight[i]*is_item_in[i] <= maximum_weight;
}