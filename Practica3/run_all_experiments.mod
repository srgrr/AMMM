/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Mar 20, 2017 at 8:22:37 PM
 *********************************************/
  main {
  
	/*function printResult(model){
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
	}*/
	
	/*function checkModel(model){
		var totalLoad=0;
		for (var t=1;t<=model.nTasks;t++)
			totalLoad += model.rt[t];
		writeln("Total load requested: " + totalLoad);
		var totalCapacity=0;
		for (var t=1;t<=model.nCPUs;t++)
			totalCapacity += model.rc[t];
		writeln("Total available capacity: " + totalCapacity);
		return (totalCapacity >= totalLoad);
	}*/
	
	function endInstances(){
	}
	
	function executeTest(model_name, data_name) {
		var src = new IloOplModelSource(model_name);
		var def = new IloOplModelDefinition(src);
		var cplex = new IloCplex();
		var model = new IloOplModel(def,cplex);
		var data = new IloOplDataSource(data_name);
		model.addDataSource(data);
		model.generate();
		cplex.epgap=0.01;
		cplex.solve();
		/*if (validModel && cplex.solve()) {
			printResult(model);
		}
		else {
			writeln("No solution found");
		}*/
		model.end();
		data.end();
		def.end();
		cplex.end();
		src.end();	
	}
	
	writeln("MODEL P1");
	for(var i=1; i<=2; ++i) {
		var lim_j = i == 1 ? 6 : 4;
		for(var j=1; j<=lim_j; ++j) {
			for(var k=0; k<=2; ++k) {
				var start= new Date();
				var tmp = start.getTime()/1000.0;
				executeTest("S_P1.mod", "InstanceGenerator/instances/experiment"+i+"_"+j+"_"+k+".dat");
				var end = new Date();
				var tmp2 = end.getTime()/1000.0;
				writeln(i+","+j+","+k+" = "+tmp2+" - "+tmp);			
			}		
		}	
	}

	writeln("MODEL P1a");
	for(var i=1; i<=2; ++i) {
		var lim_j = i == 1 ? 6 : 4;
		for(var j=1; j<=lim_j; ++j) {
			for(var k=0; k<=2; ++k) {
				var start= new Date();
				var tmp = start.getTime()/1000.0;
				executeTest("S_P1a.mod", "InstanceGenerator/instances/experiment"+i+"_"+j+"_"+k+".dat");
				var end = new Date();
				var tmp2 = end.getTime()/1000.0;
				writeln(i+","+j+","+k+" = "+tmp2+" - "+tmp);			
			}		
		}	
	}



};