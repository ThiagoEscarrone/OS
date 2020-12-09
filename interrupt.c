#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
Estados dos processos:
		Pronto 		0
        Executando  1
        Bloqueado   2
        Encerrado 	3
*/
struct Processo{
	int pid; //id do processo
	int estado; //estado do processo
	int duracao; //duracao do processo
	int prioridade; //prioridade do processo
	struct Processo *next;
};
typedef struct Processo Proc;

struct Interrupcao{
	int pidProc; //pid do processo
	int tipoInt; //tipo da interrupção
	int prioridateInt; //prioridade da interrupção
	int periodoInt; //periodo da interrupção
	int pronto; //flag para verificar se a interrupcao foi tratada - 0 NÃO - 1 SIM
	struct Interrupcao *next;

};
typedef struct Interrupcao Int;

Proc* criaEstruturaProcessos(void){
	return NULL;
}

Int* criaEstruturaInterrupcao(void){
	return NULL;
}
	
int lstProcVaz(Proc* p){
	return( p == NULL);
}


int lstIntVaz(Int* i){
	return( i == NULL);
}

Proc* criaProcesso(Proc* p){
	Proc* novoProcesso = (Proc*) malloc(sizeof(Proc));
	int empty = lstProcVaz(p);
	
	if(empty == 1){
		novoProcesso->pid = 1;

	}else{
		novoProcesso->pid = p->pid + 1;
	}
	novoProcesso->estado = 0;
	novoProcesso->duracao = 1 + (rand() % 10);
	novoProcesso->prioridade = 1 + (rand() % 10);
	novoProcesso->next = p;
	return novoProcesso;
}

/*Tipos de Interrupcao
  Tempo		 	 0
  Exit(CTRL-C)	 1
  Escalonador  	 2
  E/S		     3
*/

Int* criaInterrupcao(int pidP, Int* i){
	Int* novaInterrupcao = (Int*) malloc(sizeof(Int));

	novaInterrupcao->pidProc = pidP;
	novaInterrupcao->tipoInt = 0 + (rand() % 3);
	novaInterrupcao->periodoInt = 1 + (rand() % 5);
	novaInterrupcao->next = i;

	return novaInterrupcao;
}

const char* mostraEstado(int p){
	switch(p){
		case 0:
			return "Pronto\t";
		break;
		case 1:
			return "Executando";
		break;
		case 2:
			return "Bloqueado";
		break;
		case 3:
			return "Encerrado";
		break;
		default:
			return "-";
		}
}

const char* mostraTipo(int t){
	switch(t){
		case 0:
			return "Tempo\t";
		break;
		
		case 1:
			return "Exit(CTRL-C)";
		break;
		
		case 2:
			return "Escalonador";
		break;
		
		default:
			return "E/S";
	}
}

void exibeInfo(Proc* l, Int* interr){
	Proc* p;
	Int* i;
	int empty = lstIntVaz(interr);

	printf("\n===================LISTA DE PROCESSOS==============================\n");
	printf("|PID \tEstado \t\tPrioridade \tTempo restante de execução\t\n");
	for(p=l; p!=NULL; p = p->next)
		printf("|%d \t%s \t%d \t\t0:00:%d \t\n", p->pid, mostraEstado(p->estado), p->prioridade, p->duracao);
	
	printf("===================================================================\n");


	if(empty == 0){
		printf("========VETOR DE INTERRUPCAO===============\n");
		printf("|PID \tTipo \t\tTempo restante\t\n");
		for(i=interr; i!=NULL; i = i->next)
			printf("|%d \t%s \t0:00:%d\t\n", i->pidProc, mostraTipo(i->tipoInt), i->periodoInt);
		
		printf("===========================================\n\n\n");
	}else{
		printf("========VETOR DE INTERRUPCAO===============\n");
		printf("|PID \tTipo \tTempo restante\t\n");
		printf("|  Vetor de interrupção vazio\n");
		
		printf("===========================================\n\n\n");
	}
}

Proc* procuraProcesso(Proc* l, int pid){
	Proc* p;
	
	for(p = l; p != NULL; p = p->next){
		if(p->pid == pid){
			return p;
		}
	}
	return NULL;
}


Proc* escolheProcesso(Proc* l){
	Proc* p;
	int maior=0, maiorPID=0;

	for(p = l; p!= NULL; p = p->next){
            if(p->estado == 0){
				if(p->prioridade > maior){
					maior = p->prioridade;
					maiorPID = p->pid;
				}
            }
    }
	Proc* a = procuraProcesso(l, maiorPID);
	return a;
}

void executaInterrupcao(int pid, Int* interr, Proc* l){
	interr = criaInterrupcao(pid, interr);
	
	while(interr->periodoInt > 0){
		exibeInfo(l, interr);
		interr->periodoInt--;
	}
}

void Execucao(Proc* l, Int* interr, int n){
    Proc* a;
    int cont=0,i=0,chanceInterrupcao=0, pid=0, resultInt=0;

    for (int i = 0; i < n; ++i){
	    a = escolheProcesso(l);
	    a->estado = 1;
		chanceInterrupcao = (rand() % 10);
		float b = a->duracao/2.0;
		int metade = (int)b;
			
		while(a->duracao > 0){
			exibeInfo(l, interr);
			if(a->duracao == metade && chanceInterrupcao < 8){
				pid = a->pid;
				a->estado = 2;
				executaInterrupcao(pid, interr, l);
				a->estado = 1;
			}
			a->duracao--;
		}
		if(a->duracao == 0)
			a->estado = 3;
	}
}

void main(int argc, char *argv[]){

  	if(argc != 2){
    	printf("uso: <binario> <numero de processos>\n");
  	}else{
  		int nprocessos = atoi(argv[1]);
		srand(time(NULL));
		Proc* p = criaEstruturaProcessos();
		Int* interrupcao = criaEstruturaInterrupcao();

		int i=0;	
		for(i; i < nprocessos; i++)
			p = criaProcesso(p);
		Execucao(p, interrupcao, nprocessos);
	}

}
