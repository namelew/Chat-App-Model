# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "MQTTAsync.h"

#define ADDRESS     "tcp://localhost:1883"
#define QOS         1
#define TIMEOUT     10000L

int finished = 0;
int disc_finished = 0;
int subscribed = 0;

int main(){
    MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc, op;
	
	FILE *data = fopen("data.txt", "a");
	char nome[50]; 
	printf("1-Logar\n2-Cadastrar Usuario\n");
	scanf("%d", &op);
	getchar();
	do{
		if(op == 1){
			printf("Nome Usuario: ");
			scanf("%s", nome);
			break;
		}
		else if(op == 2){
			printf("Novo Usuario: ");
			scanf("%s", nome);
			char *temp = strcat(nome, "\n");
			fputs(temp, data);
			break;
		}else{
			printf("Opcao Invalida!Digite Novamente: ");
			scanf("%d", &op);
		}
	}while(op != 1 || op != 2);

	if(rc = MQTTAsync_create(&client, ADDRESS, strcat(nome, "_User"), MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTASYNC_SUCCESS){
		printf("Failed to create client object, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
    // conectar com o broker Mosquitto
    
    /*
    faca{
        escolha(x){
            caso 1:
                msg=pedir_mensagem();
                mandar_para(msg);
            caso 2:
                fechar_chat();
        }
        quit = pergunta_sair();
    }enquanto(quit != Q || quit != q);
    */

   	MQTTAsync_destroy(&client);
	fclose(data);
    return rc;
}
