/* 
 * File:   main.cpp
 * Author: Jaime Dantas, Alexandre Luz, Ramon Fava
 *
 * Created on April 11, 2016, 7:08 PM
 */

//#include <cstdlib>
#include <iostream>
//#include <fstream>
#include "time.h"
#include <vector>
#include "genetico.h"
#include "annealing.h"
using namespace std;

int main(int argc, char* argv[]){
	
	if(argc != 6 && argc != 4){
		cout << "Comandos para rodar:"<<endl<<"./a.out tamCromossomo tamPop probMutacao probCrossover qtMaxGeracoes"<<endl;
		cout << "./a.out tamCromossomo temperturaInicial a"<<endl;
		exit(0);
	}	

        else if(argc == 6){//algoritmo genetico
		int tamCromossomo = atoi(argv[1]);
		int tamPop = atoi(argv[2]);
		float probMutacao = atof(argv[3]);
		float probCrossover = atof(argv[4]);
		int qtMaxGeracoes = atoi(argv[5]);
		
		Genetico * g = new Genetico(tamCromossomo, tamPop, probMutacao, probCrossover, qtMaxGeracoes);
		
		g->run();
		
		delete g;		
	}
	else if(argc == 4){//simulate annealing
		int tamCromossomo = atoi(argv[1]);
		float temperturaInicial = atof(argv[2]);
		float resfriamento = atof(argv[3]);
		
		Annealing * a = new Annealing(tamCromossomo, temperturaInicial, resfriamento);
		
		a->run();
	       
		delete a;
	}

	return 0;
}

