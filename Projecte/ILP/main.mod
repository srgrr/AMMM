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
	
	function isSolutionValid(solution) {
		
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
		}
		else {
			writeln("No solution found");	
		}
	}
	
	
	executeModel("hull.dat");
	
};