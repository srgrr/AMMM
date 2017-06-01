/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 18, 2017 at 7:50:40 PM
 *********************************************/
int numCities=...;
range C=1..numCities;

int numTypes=...;
range T=1..numTypes;

int numLocations=...;
range L=1..numLocations;

float cityCoordinates[c in C][_ in 1..2] = ...;
float locationCoordinates[l in L][_ in 1..2] = ...;

int cityPopulation[c in C]=...;
int totalPopulation = sum(c in C) cityPopulation[c];

float d_center=...;

int typeCapacity[t in T] = ...;
float typeDistance[t in T] = ...;
float typeCost[t in T] = ...;

float qll[a in L][b in L];
int dll[a in L][b in L];
float dcl[c in C][l in L];


execute {
	function dis(p, q) {
		return Math.sqrt((p[1]-q[1])*(p[1]-q[1]) + (p[2]-q[2])*(p[2]-q[2]));	
	}
	
	for(var i=1; i<=numLocations; ++i) {
		for(var j=1; j<=numLocations; ++j) {
			qll[i][j] = dis(locationCoordinates[i], locationCoordinates[j]);
			if(qll[i][j] >= d_center) {
				dll[i][j] = 1;
			}
			else {
				dll[i][j] = 0;
			}
		}
	}
	for(var i=1; i<=numCities; ++i) {
		for(var j=1; j<=numLocations; ++j) {
			dcl[i][j] = dis(cityCoordinates[i], locationCoordinates[j]);
		}
	}
};

dvar boolean X[l in L][t in T];
dvar boolean P[c in C][l in L];
dvar boolean S[c in C][l in L];

// EQUATION 1
minimize sum(l in L, t in T) typeCost[t]*X[l,t];

subject to {
	// EQUATION 2
	forall(l in L)
		sum(t in T) X[l, t] <= 1;
	// EQUATION 3
	forall(c in C)
		sum(l in L) P[c, l] == 1;
	// EQUATION 4
	forall(c in C)
		sum(l in L) S[c, l] == 1;
	// EQUATION 5
	forall(c in C, l in L)
		P[c, l] + S[c, l] <= 1;
	// EQUATION 6
	forall(c in C, l in L)
		sum(t in T) X[l, t] >= P[c, l] + S[c, l];
	// EQUATION 7
	forall(la in L, lb in L : la < lb && dll[la, lb] == 0)
		sum(t in T) (X[la, t] + X[lb, t]) <= 1;
	// EQUATION 8
	forall(l in L, t in T)
		sum(c in C) cityPopulation[c]*(P[c, l] + 0.1*S[c, l])
		<= typeCapacity[t] + (1 - X[l, t])*(totalPopulation);
	// EQUATION 9
	forall(l in L, t in T, c in C : dcl[c][l] > typeDistance[t])
		X[l, t] + P[c, l] <= 1;
	// EQUATION 10
	forall(l in L, t in T, c in C : dcl[c][l] > 3.0*typeDistance[t])
		X[l, t] + S[c, l] <= 1;
}
