/*********************************************
 * OPL 12.6.0.0 Model
 * Author: ramela
 * Creation Date: 01/03/2017 at 10:44:41
 *********************************************/

 main {
  
	function printResult(model){
		writeln("Max load " + cplex.getObjValue() + "%");	
		for (var c=1;c<=model.nCPUs;c++) {
			var load=0;
			for (var t=1;t<=model.nTasks;t++) {
			  load+=(model.rt[t]* model.x_tc[t][c]);
			  if (model.x_tc[t][c] > 0) {
			  	writeln("Task " + t + " assigned to CPU " + c)			  
			  }
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
	
	var src = new IloOplModelSource("Practica2.mod");
	var def = new IloOplModelDefinition(src);
	var cplex = new IloCplex();
	var model = new IloOplModel(def,cplex);
	var data = new IloOplDataSource("Practica2.dat");
	model.addDataSource(data);
	model.generate();
	var validModel = checkModel(model);
	cplex.epgap=0.01;
	if (validModel && cplex.solve()) {
		printResult(model);
	}
	else {
		writeln("No solution found");
	}
	endInstances();		
	
};