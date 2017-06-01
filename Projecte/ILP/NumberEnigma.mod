/*********************************************
 * OPL 12.6.0.0 Model
 * Author: sergiorg
 * Creation Date: Apr 20, 2017 at 7:39:13 AM
 *********************************************/
range N = 0..10;
range P = 1..3;

dvar boolean X[p in P][n in N];

minimize 1;

subject to {
	// solo un numero por posicion
	forall(p in P)
	  sum(n in N) X[p][n] == 1;
	// one number is correct and well placed
	// exactamente uno de los
	// siguientes aparece en la solucion
	X[1][6] + X[2][8] + X[3][2] == 1;
	// one number is correct but wrong placed
	// exactamente uno de los numeros que sale
	// (pero no en las posiciones propuestas)
	// es correcto
	X[2][6] + X[3][6] + X[1][1] + X[3][1] + X[1][4] + X[2][4] == 1;
	// two numbers are correct and wrong placed
	// exactamente dos de los numeros que salen
	// (pero no en las posiciones propuestas)
	// son correctos
	X[2][2] + X[3][2] + X[1][0] + X[3][0] + X[1][6] + X[2][6] == 2;
	// nothing is correct
	// nada de lo que sale forma parte de la solucion
	// en ninguna de sus formas
	// el 7 no aparece
	sum(p in P) X[p][7] == 0;
	// el 3 no aparece
	sum(p in P) X[p][3] == 0;
	// el 8 no aparece 
	sum(p in P) X[p][8] == 0;
	// one number is correct but wrong placed
	// lo mismo que con la anterior constraint pero cambiando los numeros
	X[2][7] + X[3][7] + X[1][8] + X[3][8] + X[1][0] + X[2][0] == 1;
}