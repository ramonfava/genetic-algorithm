/* 
 * File:   genetico.h
 * Author: Jaime Dantas, Alexandre Luz, Ramon Fava
 *
 * Created on April 11, 2016, 7:08 PM
 */

#ifndef _GENETICO_H_
#define _GENETICO_H_
//#include "time.h"
//#include <vector>

class Genetico{
public:
	//propriedades
	int tamCromossomo;
	int tamPop;
	float probMutacao;
	float probCrossover;
	int qtMaxGeracoes;

	//elementos
	std::vector<int> cromossomo;
	std::vector< std::vector<int> > populacao;
	std::vector< std::vector<int> > selecionados;
	std::vector< std::vector<int> > popCrossover;
	//vetor de aptidao para cada cromossomo da população
	std::vector<int> aptidao;

	//construtores
	Genetico(int tamCromossomo, int tamPop, float probMutacao, float probCrossover, int qtMaxGeracoes);
	~Genetico();

	//funcoes
	void criarCromossomo();
	void criarPop();
	void crossover();
	void mutacao();
	int fitness(int posicao);
	void avaliar();
	void selecaoNatural();
	void run();
	int min();
	int zeroColisao();
	void imprimir(std::vector<int> melhorSolucao);
};

/*
	construtor
*/
Genetico::Genetico(int tamCromossomo, int tamPop, float probMutacao, float probCrossover, int qtMaxGeracoes){
	srand(time(0));
	this->tamCromossomo = tamCromossomo;
	this->tamPop = tamPop;
	this->probMutacao = probMutacao;
	this->probCrossover = probCrossover;
	this->qtMaxGeracoes = qtMaxGeracoes;

	this->populacao.reserve(tamPop);
	this->selecionados.reserve(tamPop);
	this->popCrossover.reserve(tamPop);
	for (int i = 0; i < tamPop; ++i)
	{
		this->populacao[i].reserve(tamCromossomo);
		this->selecionados[i].reserve(tamCromossomo);
		this->popCrossover[i].reserve(tamCromossomo);
	}
	this->cromossomo.reserve(tamCromossomo);
	this->aptidao.reserve(tamPop);
}

/*
	destructor
*/
Genetico::~Genetico(){
}

void Genetico::run(){
	int posicao;
	criarPop();
	int epocas = 0;
	while(true)
	{
		avaliar();
		posicao = zeroColisao();
		if(posicao != -1){
			// for (int i = 0; i < this->tamCromossomo; ++i)
			// {
			// 	std::cout<<this->populacao[posicao][i]<<" ";
			// }
			// std::cout<<std::endl;
			imprimir(this->populacao[posicao]);
			break;
		}
		selecaoNatural();
		crossover();
		mutacao();
		++epocas;
		if(epocas == this->qtMaxGeracoes){
			posicao = min();
			//std::cout<<"Solução não ótima encontrada com numColisoes = "<<aptidao[posicao]/2<<std::endl;
			// for (int i = 0; i < this->tamCromossomo; ++i)
			// {
			// 	std::cout<<this->populacao[posicao][i]<<" ";
			// }
			// std::cout<<std::endl;
			imprimir(this->populacao[posicao]);
			break;
		}
	}
}

void Genetico::imprimir(std::vector<int> melhorSolucao){
    std::vector<bool> colisoes;
    colisoes.reserve(this->tamCromossomo);
    this->cromossomo = melhorSolucao;
    int distancia;
    
    for (int i = 0; i < this->tamCromossomo; ++i)
    {
        colisoes[i] = false;
    }

    for (int referencia = 0; referencia < this->tamCromossomo; ++referencia)
    {
        //checar linha
        for (int j = 0; j < this->tamCromossomo; ++j)
        {
            if(referencia != j)
            {
                //mesma linha
                if(this->cromossomo[referencia] == this->cromossomo[j]){ 
                    colisoes[referencia] = true;
                }
                //mesma diagonal
                distancia = abs(referencia-j);//distancia em colunas
                //se a linha de referência for igual a linha j+distância em colunas, 
                //quer dizer que está na mesma diagonal
                if(this->cromossomo[referencia] == this->cromossomo[j]+distancia
                   || this->cromossomo[referencia] == this->cromossomo[j]-distancia){
                    colisoes[referencia] = true;
                }
            }
        }
    }

    for (int i = 0; i < this->tamCromossomo; ++i)
    {
        std::cout<<melhorSolucao[i]<<" ";
    }
    std::cout<<"- ";
    for (int i = 0; i < this->tamCromossomo; ++i)
    {
        if(colisoes[i])
            std::cout<<i<<" ";
        else
            std::cout<<"x ";
    }
}

/*
	checa se já há uma solução para o problema
*/
int Genetico::zeroColisao(){
	for (int i = 0; i < this->tamPop; ++i)
	{
		if(this->aptidao[i] == 0){
			return i;
		}
	}
	return -1;
}


/*
	cria um vetor de inteiros de tamanho n que
	vão representar a posição da rainha no tabuleiro
*/
void Genetico::criarCromossomo(){
	this->cromossomo.clear();//limpar cromossomo caso já tenha valores
	this->cromossomo.reserve(this->tamCromossomo);//definir tamanho do cromossomo

	for(int i=0; i<this->tamCromossomo; i++){
		this->cromossomo.push_back(int(rand()%this->tamCromossomo));
		for(int j=0; j<i; j++){//verifica se a posição já foi escolhida
			if(this->cromossomo[i] == this->cromossomo[j])
				this->cromossomo[i] = int(rand()%this->tamCromossomo);
		}
	}
}

/*
	cria uma população de cromossomos de tamanho "tamPop"
*/
void Genetico::criarPop(){
	//criar populaçao de tamanho tamPop
	for (int i = 0; i < this->tamPop; ++i)
	{
		criarCromossomo();
		this->populacao.push_back(this->cromossomo);
	}
}

/*
	cruzar dois cromossomos a
*/
void Genetico::crossover(){
	int pontoDeCorte;
	float probabilidade;
	//int x,y;

	for (int i = 0; i < this->tamPop-1; i+=2)
	{
		probabilidade = rand()%100;//probabilidade de acontecer crossover
		probabilidade /= 100;
		if(probabilidade <= this->probCrossover){
			pontoDeCorte = int(rand()%this->tamCromossomo-1)+1;
			//de 0 até o ponto de corte 
			for (int n = 0; n < pontoDeCorte; ++n)
			{
				this->popCrossover[i][n] = this->selecionados[i+1][n];//primeiro filho criado
				this->popCrossover[i+1][n] = this->selecionados[i][n];//segundo filho criado
			}
			//do ponto de corte até tam do cromossomo
			for (int n = pontoDeCorte; n < this->tamCromossomo; ++n)
			{
				this->popCrossover[i+1][n] = this->selecionados[i+1][n];//primeiro filho criado
				this->popCrossover[i][n] = this->selecionados[i][n];//segundo filho criado
			}
		}
		else{//se não houver crossover, simplesmente passar os pais adiante
			for (int n = 0; n < this->tamCromossomo; ++n)
			{
				this->popCrossover[i][n] = this->selecionados[i][n];
				this->popCrossover[i+1][n] = this->selecionados[i+1][n];
			}
		}
	}
}

/*
	mutaciona população a uma probabilidade probMutacao
	caso mutacione, cria um novo cromossomo aleatório
	e modifica na população
*/
void Genetico::mutacao(){
	float prob;
	for (int i = 0; i < this->tamPop; ++i)
	{
		prob = rand()%100000;
		prob /= 100000;
		if(prob <= this->probMutacao){
			criarCromossomo();
			this->populacao[i] = this->cromossomo;
		}
		else{
			for (int x = 0; x < this->tamCromossomo; ++x)
			{
				this->populacao[i][x] = this->popCrossover[i][x];
			}
		}
	}
}

/*
	calcula a quantidade de colisões do cromossomo
	na população[posicao]
*/
int Genetico::fitness(int posicao){
	int numColisoes;
	numColisoes = 0;
	int distancia;
	for (int referencia = 0; referencia < this->tamCromossomo; ++referencia)
	{
		//checar linha
		for (int j = 0; j < this->tamCromossomo; ++j)
		{
			if(referencia != j)
			{
				//mesma linha
				if(this->populacao[posicao][referencia] == this->populacao[posicao][j]){ 
					numColisoes++;
				}
				//mesma diagonal
				distancia = abs(referencia-j);//distancia em colunas
				//se a linha de referência for igual a linha j+distância em colunas, 
				//quer dizer que está na mesma diagonal
				if(this->populacao[posicao][referencia] == this->populacao[posicao][j]+distancia
				   || this->populacao[posicao][referencia] == this->populacao[posicao][j]-distancia){
					numColisoes++;
				}
			}
		}
	}
	return numColisoes;
}

/*
	calcula a aptidão de cada cromossomo da população
*/
void Genetico::avaliar(){
	for (int i = 0; i < this->tamPop; ++i)
	{
		this->aptidao[i] = this->fitness(i);
	}
}

/*
	selecão natural através do torneio
	escolher aleatoriamente 2 elementos, e entre
	eles escolher o com a melhor aptidão
*/
void Genetico::selecaoNatural(){
	int elemento1, elemento2, escolhido;
	for (int i = 0; i < this->tamPop; ++i)
	{
		elemento1 = int(rand()%this->tamPop);//de 0 a tamPop-1
		elemento2 = int(rand()%this->tamPop);
		while(elemento1 == elemento2){
			elemento1 = int(rand()%this->tamPop);
			elemento2 = int(rand()%this->tamPop);
		}
		//queremos o com a menor aptidão, ou seja, menor número de colisões
		escolhido = this->aptidao[elemento1] < this->aptidao[elemento2] ? elemento1 : elemento2;
		//adicionar o cromossomo escolhido aos selecionados
		this->selecionados[i] = this->populacao[escolhido];
	}
}

//retorna a posicao do cromossomo com a menor aptidao
int Genetico::min(){
	int posicao = 0;
	for (int i = 1; i < this->tamPop; ++i)
	{
		posicao = (this->aptidao[posicao] < this->aptidao[i]) ? posicao : i;
	}
	return posicao;
}

#endif
