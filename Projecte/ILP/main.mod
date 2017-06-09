/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 19, 2017 at 3:56:09 PM
 *********************************************/


  main {
  
  	function printPlottable(model) {
  		writeln(model.numCities);
  		for(var i=1; i<=model.numCities; ++i) {
  			writeln(model.cityCoordinates[i][1] + " " + model.cityCoordinates[i][2]);
  		}
  		writeln(model.numLocations);
  		for(var i=1; i<=model.numLocations; ++i) {
  			writeln(model.locationCoordinates[i][1] + " " + model.locationCoordinates[i][2]);
  		}
  		writeln(model.numTypes);
  		for(var i=1; i<=model.numTypes; ++i) {
  			writeln(model.typeDistance[i]);
  		}
  		for(var i=1; i<=model.numLocations; ++i) {
  			var typeIndex = 0;
  			for(var j=1; j<=model.numTypes && typeIndex == 0; ++j) {
  				if(model.X[i][j] == 1) {
  					typeIndex = j;  				
  				}
  			}
  			writeln(typeIndex);
  		}
  		for(var i=1; i<=model.numCities; ++i) {
  			var typeIndex = 0;
  			for(var j=1; j<=model.numLocations && typeIndex == 0; ++j) {
  				if(model.P[i][j] == 1) {
  					typeIndex = j;  				
  				}
  			}
  			writeln(typeIndex);
  		}
  		for(var i=1; i<=model.numCities; ++i) {
  			var typeIndex = 0;
  			for(var j=1; j<=model.numLocations && typeIndex == 0; ++j) {
  				if(model.S[i][j] == 1) {
  					typeIndex = j;  				
  				}
  			}
  			writeln(typeIndex);
  		}
  	}
  
  
	function printResult(model) {
		writeln("LOCATIONS & TYPES");
		for(var i=1; i<=model.numLocations; ++i) {
			for(var j=1; j<=model.numTypes; ++j) {
				if(model.X[i][j] == 1) {
					writeln("Location " + i + " has a city of type " + j);				
				}	
			}
		}
		writeln("\nCITIES ASSIGNATIONS");
		for(var i=1; i<=model.numCities; ++i) {
			write("City " + i + ":");
			for(var j=1; j<=model.numLocations; ++j) {
				if(model.P[i][j] == 1) {
					write(" P=" + j);
				}
			}
			for(var j=1; j<=model.numLocations; ++j) {
				if(model.S[i][j] == 1) {
					writeln(" S=" + j);				
				}			
			}
		}
	}
	
	function checkModel(model) {
		return true;
	}
	
	function endInstances() {
		model.end();
		data.end();
		def.end();
		cplex.end();
		src.end();
	}
	
	function sum(v) {
		var ret = 0;
		for(var i = 1; i <= v.size; ++i) ret += v[i];
		return ret;	
	}
	
	function isSolutionValid(solution) {
		// EQUATION 2
		for(var i = 1; i <= solution.numLocations; ++i) {
			if(sum(solution.X[i]) > 1) return false;
		}
		// EQUATIONS 3 and 4
		for(var i = 1; i <= solution.numCities; ++i) {
			if(sum(solution.P[i]) != 1) return false;			
			if(sum(solution.S[i] != 1)) return false;			
		}
		// EQUATION 5
		for(var c = 1; c <= solution.numCities; ++c) {
			for(var l = 1; l <= solution.numLocations; ++l) {
				if(solution.P[c][l] + solution.S[c][l] > 1) return false;			
			}		
		}
		// EQUATION 6
		for(var c = 1; c <= solution.numCities; ++c) {
			for(var l = 1; l <= solution.numLocations; ++l) {						
				var s = sum(solution.X[l]);
				if(s < solution.P[c][l] + solution.S[c][l]) return false;
			}
 		}
 		// EQUATION 7
 		for(var a = 1; a <= solution.numLocations; ++a) {
 			for(var b = a+1; b <= solution.numLocations; ++b) {
 			  if(solution.dll[a][b] == 0) {
 			  	var sa = sum(solution.X[a]);
 			  	var sb = sum(solution.X[b]);
 			  	if(sa + sb > 1) return false;  
 			  } 			
 			} 		
 		}
		return true;
	}
	
	function executeModel(data_path) {
		var src = new IloOplModelSource("ilp_model.mod");
		var def = new IloOplModelDefinition(src);
		var cplex = new IloCplex();
		var model = new IloOplModel(def,cplex);
		var data = new IloOplDataSource(data_path);
		model.addDataSource(data);
		model.generate();
		var validModel = checkModel(model);
		var solution = -1;
		if(validModel && cplex.solve()) {
			printPlottable(model);
			solution = cplex.getObjValue();
			writeln("OBJ VALUE = " + solution);
			if(!isSolutionValid(model)) {
				writeln("Solution is not valid!!!");			
			}
		}
		else {
			writeln("No solution found");	
		}
	}
	
	
	executeModel("data/test_1.dat");
	
};