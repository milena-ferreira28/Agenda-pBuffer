#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *pBuffer; // void global

//funções da agenda
void adicionarPessoa();
void removerPessoa();
void buscarPessoa();
void listarPessoas();

//---------------------------MAIN---------------------------//
int main() {
    //Inicializa memória com tamanho de 5 ints + 40 bytes para armazenar o buscar nome(necessário na função buscarPessoa)
    pBuffer = malloc(sizeof(int) * 5 + 40);
    if (pBuffer == NULL) {
        printf("Erro ao alocar memória inicial!\n");
        return 1;
    }

    *(int *)pBuffer = 0;  // Inicializa o contador de pessoas
    int *opcao = (int *)(pBuffer + sizeof(int));  // Aloca o espaço para a opção (usado no menu)
    int *indice = (int *)(pBuffer + sizeof(int) * 2);  // Aloca espaço para o indice (usado na função removerPessoa)
    int *i = (int *)(pBuffer + sizeof(int) * 3);  // Aloca espaço para o i (usado nas funções buscarPessoa e listarPessoas)
    int *encontrados = (int *)(pBuffer + sizeof(int) * 4);  // Aloca espaço para o encontrados (usado na função buscarPessoas)

// do -- while
    do {
        printf("\nMenu:\n");
        printf("1- Adicionar pessoa:\n");
        printf("2- Remover pessoa:\n");
        printf("3- Buscar pessoas:\n");
        printf("4- Listar pessoas:\n");
        printf("5- Sair:\n");
        printf("Escolha uma opcao: ");
        scanf("%d", opcao);

        getchar();

        switch (*opcao) {
            case 1:
                adicionarPessoa(); //chama a função adicionarPessoa
                 opcao = (int *)(pBuffer + sizeof(int)); // Reajusta a opção após o realloc
                break;

            case 2: 
                removerPessoa(); //chama a função removerPessoa
                break;

            case 3:
                buscarPessoa(); //chama a função buscarPessoa
                break;

            case 4:
                listarPessoas(); //chama a função listarPessoas
                break;

            case 5:
                printf("Encerrando o programa...\n"); //sai do loop
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (*opcao !=5);


    // Libera toda a memória alocada que foi alocada no pBuffer durante a execução e encerra o programa
    free(pBuffer);
    return 0;
}

//------------------------------ Função adicionarPessoa -----------------------------//

void adicionarPessoa() {
    int *totalPessoas = (int *)pBuffer; //aponta *totalPessoas para o inicio do pBuffer (inicializado em 0)
    if (pBuffer == NULL) { //testa se não está vazia
        printf("Erro ao alocar memória inicial!\n");
        exit(1);
    }
    // realoca memória para adicionar nova pessoa com valores já selecionados(40 reservados para a função buscar pessoa; 40bytes para o nome, 4 para idade e 40 para email)
    void *novoBuffer = realloc(pBuffer, sizeof(int) * 5 + 40 + (*totalPessoas + 1) * (40 + 4 + 40)); //144
    if (novoBuffer == NULL) { //testa se não está vazia
        printf("Erro ao realocar memória!\n");
        exit(1);
    }

    pBuffer = novoBuffer;  // Atualiza o ponteiro global após o realloc
    totalPessoas = (int *)pBuffer;  // Atualiza o ponteiro de totalPessoas após o realloc
    

    //Aponta para o espaço onde a novaPessoa estará(40 bytes nome, 4 bytes idade e 40 bytes email)
    char *novaPessoa = (char *)(pBuffer) + sizeof(int) * 5 + (*totalPessoas) * (40 + 4 + 40);

    char *nome = novaPessoa; //Aponta para o mesmo lugar que novaPessoa
    char *idade = nome + 40; //Aponta para o nome deslocando 40 bytes a direita
    char *email = idade + 4; //Aponta para a idade deslocando 4 bytes a direita

    //Lê os dados do usuário
    printf("Digite o nome: ");
    fgets(nome, 40, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a idade: ");
    fgets(idade, 4, stdin);
    idade[strcspn(idade, "\n")] = 0;

    printf("Digite o email: ");
    fgets(email, 40, stdin);
    email[strcspn(email, "\n")] = 0;

    (*totalPessoas)++; //Incremento no totalPessoas
    printf("Nova pessoa adicionada com sucesso!\n");
}

//------------------------------ Função removerPessoa -----------------------------//

void removerPessoa() {
    int *totalPessoas = (int *)pBuffer; //aponta *totalPessoas para o inicio do pBuffer
    int *indice = (int *)(pBuffer + sizeof(int) * 2);//aponta para o espaço reservado de indice

    if (*totalPessoas == 0) { //testa se não tem pessoas
        printf("\nNenhuma pessoa cadastrada para remover.\n");
        return;
    }

    listarPessoas(); //lista as pessoas cadastradas para escolha de qual será removida
    printf("Digite o índice da pessoa a ser removida (1 a %d): ", *totalPessoas); //usuário escolhe quem vai remover
    scanf("%d", indice);
    getchar();

    if (*indice < 1 || *indice > *totalPessoas) { //testa para o caso do indice nao existir
        printf("Índice inválido!\n");
        return;
    }
        *indice -= 1; //decremento do valor apontado por indice


    char *pessoaRemovida = (char *)pBuffer + sizeof(int) * 5 + (*indice) * (40 + 4 + 40); //aponta para a pessoa que será removida
    char *proximaPessoa = pessoaRemovida + (40 + 4 + 40); //aponta para a proxima pessoa no buffer

    memmove(pessoaRemovida, proximaPessoa, (*totalPessoas - *indice - 1) * (40 + 4 + 40)); //move as pessoas depois da removida para o lugar dela
    (*totalPessoas)--; //decremento no totalPessoas

    // realloc para reduzir a memória que sobrou pela remoção da pessoa
    void *novoBuffer = realloc(pBuffer, sizeof(int) * 5 + 40 + (*totalPessoas ) * (40 + 4 + 40));
    if (novoBuffer == NULL && (*totalPessoas) > 0) { //testa se está vazio
        printf("Erro ao realocar memória após remoção!\n");
        exit(1);
    }

    pBuffer = novoBuffer;// atualiza o ponteiro global após o realloc
    totalPessoas = (int *)pBuffer;  // atualiza o ponteiro de totalPessoas após o realloc

    printf("Pessoa removida com sucesso!\n");
}

//------------------------------ Função buscarPessoa -----------------------------//

void buscarPessoa() {
    int *totalPessoas = (int *)pBuffer; //aponta *totalPessoas para o inicio do pBuffer
    if (*totalPessoas == 0) { // testa para o caso de não ter pessoas
        printf("Nenhuma pessoa cadastrada para buscar.\n");
        return;
    }

    // aponta nomeBusca para local no buffer reservado no final do mesmo
    char *nomeBusca = (char *)(pBuffer) + sizeof(int) * 5 + (*totalPessoas) * (40 + 4 + 40);
    
    printf("Digite o nome da pessoa a ser buscada: "); //armazena o nome buscado
    fgets(nomeBusca, 40, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int *encontrados = (int *)(pBuffer + sizeof(int) * 4); //aponta encontrados para o espaço reservado a ele no buffer
    *encontrados = 0; // inicaliza em 0

    int *i = (int *)(pBuffer + sizeof(int) * 3); //aponta i para o espaço reservado a ele no buffer

    for (*i = 0; *i < *totalPessoas; (*i)++) {
        char *pessoa = (char *)(pBuffer) + sizeof(int) * 5 + (*i) * (40 + 4 + 40); //aponta para cada nome existente no buffer
        char *nome = pessoa;
        char *idade = pessoa + 40;
        char *email = pessoa + 40 + 4;

        if (strcmp(nome, nomeBusca) == 0) { //se o nome existir imprime suas informações
            printf("\nPessoa %d encontrada:\n", *i + 1);
            printf("Nome: %s\n", nome);
            printf("Idade: %s\n", idade);
            printf("Email: %s\n", email);
            (*encontrados)++; //incremento do encontrados
        }
    }

    if (*encontrados == 0) { //se não existir imprime que nao foi encontrado
        printf("Nenhuma pessoa com o nome '%s' encontrada.\n", nomeBusca);
    }
}

//------------------------------ Função listarPessoas -----------------------------//

void listarPessoas() {
    int *totalPessoas = (int *)pBuffer; //aponta *totalPessoas para o inicio do pBuffer
    if (*totalPessoas == 0) { //testa se não existe pessoas
        printf("\nNenhuma pessoa cadastrada!\n");
        return;
    }

    int *i = (int *)(pBuffer + sizeof(int) * 3); //aponta i para o espaço reservado a ele no buffer

    printf("\nLista de Pessoas Cadastradas:\n");

     for (*i = 0; *i < *totalPessoas; (*i)++) { //imprime as pessoas que ouverem
        char *pessoa = (char *)(pBuffer) + sizeof(int) * 5 + (*i) * (40 + 4 + 40);
        char *nome = pessoa;
        char *idade = nome + 40;
        char *email = idade + 4;
        printf("\nPessoa %d:\n", *i + 1);
        printf("Nome: %s\n", nome);
        printf("Idade: %s\n", idade);
        printf("Email: %s\n", email);
     }
    
}


