/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 19, 2017 at 3:56:09 PM
 *********************************************/


  main {
  
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
	
	function isSolutionValid(model) {
		return true;
	}
	
	function executeTest(data_path, known_result) {
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
			// printResult(model);
			solution = cplex.getObjValue();
		}
		write(data_path + ": ");
		if(solution == known_result && isSolutionValid(model)) {
			writeln("OK");
		}
		else {
			if(solution != -1) {
				printResult(model);			
			}
			writeln("ERR");
		}
		return solution == known_result;
	}
	
	executeTest("test_1.dat",  2);
	executeTest("test_2.dat",  2);
	executeTest("test_3.dat", -1);
	executeTest("test_4.dat",  3);
	executeTest("test_5.dat",  4);
	executeTest("test_6.dat",  2);
};