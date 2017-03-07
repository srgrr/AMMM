/*********************************************
 * OPL 12.6.0.0 Model
 * Author: ramela
 * Creation Date: 07/03/2017 at 9:11:29
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
	 	for (var t=1;t<=model.nTasks;t++) {
			if(model.used[t] == 0) {
				writeln("Task " + t + " not assigned to any CPU") 				
			} 			
		}
	}
	
	function endInstances(){
		model.end();
		data.end();
		def.end();
		cplex.end();
		src.end();
	}
	
	var src = new IloOplModelSource("Practica2b.mod");
	var def = new IloOplModelDefinition(src);
	var cplex = new IloCplex();
	var model = new IloOplModel(def,cplex);
	var data = new IloOplDataSource("Practica2b.dat");
	model.addDataSource(data);
	model.generate();
	cplex.epgap=0.01;
	cplex.solve();
	printResult(model);
	endInstances();		
	
};