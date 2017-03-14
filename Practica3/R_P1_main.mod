/*********************************************
 * OPL 12.6.0.0 Model
 * Author: ramela
 * Creation Date: 14/03/2017 at 8:24:03
 *********************************************/

  main {
  
	function printResult(model){
		writeln("Max load " + cplex.getObjValue() + "%");	
		for (var c=1;c<=model.nCPUs;c++) {
			var load=0;
			for (var t=1;t<=model.nTasks;t++) {
			  load+=(model.rt[t]* model.x_tc[t][c]);
			  writeln("Amount of resources from task " + t + " assigned to CPU " + c + ": " + model.x_tc[t][c]);
 			}			
			load = (1/model.rc[c])*load;
			writeln("CPU " + c + " loaded at " + load + "%");
		}
	}
	
	function checkModel(model){
		var totalLoad=0;
		for (var t=1;t<=model.nTasks;t++)
			totalLoad += model.rt[t];
		writeln("Total load requested: " + totalLoad);
		var totalCapacity=0;
		for (var t=1;t<=model.nCPUs;t++)
			totalCapacity += model.rc[t];
		writeln("Total available capacity: " + totalCapacity);
		return (totalCapacity >= totalLoad);
	}
	
	function endInstances(){
		model.end();
		data.end();
		def.end();
		cplex.end();
		src.end();
	}
	
	var src = new IloOplModelSource("R_P1.mod");
	var def = new IloOplModelDefinition(src);
	var cplex = new IloCplex();
	var model = new IloOplModel(def,cplex);
	var data = new IloOplDataSource("R_P1.dat");
	model.addDataSource(data);
	model.generate();
	var validModel = checkModel(model);
	cplex.epgap=0.01;
	cplex.solve();
	/*if (validModel && cplex.solve()) {
		printResult(model);
	}
	else {
		writeln("No solution found");
	}*/
	endInstances();
};