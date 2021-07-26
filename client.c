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

void connlost(void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\nConnection lost\n");
	if (cause)
		printf("     cause: %s\n", cause);

	printf("Reconnecting\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		finished = 1;
	}
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    return 1;
}

void onSend(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	printf("Message with token value %d delivery confirmed\n", response->token);
	opts.onSuccess = onDisconnect;
	opts.onFailure = onDisconnectFailure;
	opts.context = client;
	if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start disconnect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void onSendFailure(void* context, MQTTAsync_failureData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	printf("Message send failed token %d error code %d\n", response->token, response->code);
	opts.onSuccess = onDisconnect;
	opts.onFailure = onDisconnectFailure;
	opts.context = client;
	if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start disconnect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Disconnect failed, rc %d\n", response->code);
	disc_finished = 1;
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
	printf("Successful disconnection\n");
	disc_finished = 1;
}

void onSubscribe(void* context, MQTTAsync_successData* response)
{
	printf("Subscribe succeeded\n");
	subscribed = 1;
}

void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Subscribe failed, rc %d\n", response->code);
	finished = 1;
}

void onConnect(void* context, MQTTAsync_successData* response){
}

void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Connect failed, rc %d\n", response->code);
	finished = 1;
}

int main(){
    MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc, op;
	
	FILE *data = fopen("data.txt", "ra");
	char nome[50]; 
	printf("1-Logar\n2-Cadastrar Usuario\n");
	scanf("%d", &op);
	do{
		switch (op){
			case 1:
				printf("Nome Usuario: ");
				fgets(nome, 50, stdin);
     				nome[strcspn(nome, "\n")] = '\0';
			case 2:
				printf("Novo Usuario: ");
				fgets(nome, 50, stdin);
     				nome[strcspn(nome, "\n")] = '\0';
				fputs(data, strcat(nome, "\n"));
			default:
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

    // liberar a memoria do cliente
    return rc;
}
