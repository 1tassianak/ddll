/* MAIN DINAMICO PARA RECEBER DADOS DO USUARIO
#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include "main.h"

#define MAX_STR 100   // Define o tamanho máximo do buffer para strings

// Função auxiliar para limpar caracteres extras na entrada até encontrar linha nova
static void clear_input(void) {
    int c;                          // Armazena cada caractere lido
    while ((c = getchar()) != '\n' && c != EOF) {
        // Continua lendo até o final da linha
    }
}

int main(void) {
    pDDLL lista = NULL;            // Inicializa o ponteiro da lista como nulo
    int tipo = -1;                 // Armazena o tipo de dado: 0=int,1=float,2=string
    int opcao;                     // Armazena a opção selecionada pelo usuário
    char buffer[MAX_STR];          // Buffer para leitura de strings

    // Loop principal que exibe o menu enquanto opcao != 0
    do {
        printf("\n==== MENU Lista Duplamente Encadeada ====\n");  // Imprime cabeçalho do menu
        printf("1. Criar lista (int/float/string)\n");          // Opção 1: criar lista
        printf("2. Inserir no início\n");                      // Opção 2: inserir no início
        printf("3. Inserir no final\n");                       // Opção 3: inserir no final
        printf("4. Inserir na posição N\n");                   // Opção 4: inserir em posição
        printf("5. Remover do início\n");                      // Opção 5: remover do início
        printf("6. Remover do final\n");                       // Opção 6: remover do final
        printf("7. Remover da posição N\n");                   // Opção 7: remover em posição
        printf("8. Buscar primeiro elemento\n");               // Opção 8: buscar início
        printf("9. Buscar último elemento\n");               // Opção 9: buscar fim
        printf("10. Buscar elemento na posição N\n");          // Opção 10: buscar em posição
        printf("11. Verificar vazia\n");                       // Opção 11: verificar lista vazia
        printf("12. Contar elementos\n");                      // Opção 12: contar elementos
        printf("13. Esvaziar lista\n");                        // Opção 13: limpar lista
        printf("14. Destruir lista\n");                       // Opção 14: destruir lista
        printf("15. Imprimir lista\n");                        // Opção 15: imprimir lista
        printf("0. Sair\n");                                   // Opção 0: sair
        printf("Escolha uma opção: ");                          // Solicita ao usuário a escolha

        if (scanf("%d", &opcao) != 1) {  // Lê a opção do usuário
            printf("Entrada inválida. Tente novamente.\n");   // Mensagem de erro de entrada
            clear_input();                                     // Limpa buffer de entrada
            continue;                                          // Volta ao início do loop
        }
        clear_input();   // Descarta o '\n' remanescente no buffer

        // Tratamento da opção de criar lista
        if (opcao == 1) {
            if (lista != NULL) {                 // Verifica se a lista já foi criada
                printf("Lista já existe.\n");   // Informa que não pode recriar
            } else {
                printf("Tipo de dado (0=int, 1=float, 2=string): ");
                if (scanf("%d", &tipo) != 1) { // Lê o tipo de dados
                    printf("Entrada de tipo inválida.\n");
                    clear_input();             // Limpa buffer se inválida
                    continue;                  // Retorna ao menu
                }
                clear_input();                 // Limpa '\n' remanescente

                int ret;                       // Armazena código de retorno da criação
                if (tipo == 0) {
                    ret = cDDLL(&lista, sizeof(int));    // Cria lista de inteiros
                } else if (tipo == 1) {
                    ret = cDDLL(&lista, sizeof(float));  // Cria lista de floats
                } else {
                    ret = cDDLL(&lista, MAX_STR * sizeof(char));  // Cria lista de strings
                }

                if (ret == SUCCESS) {
                    printf("Lista criada com sucesso (tipo %d).\n", tipo);
                } else {
                    printf("Falha ao criar lista.\n");
                }
            }
        }
        // Tratamento das opções de inserção e remoção
        else if (opcao >= 2 && opcao <= 7) {
            if (lista == NULL) {               // Verifica se a lista existe
                printf("Crie a lista primeiro.\n");
            } else {
                int pos;                      // Variável para posição em inserção/remoção
                void *dado = NULL;           // Ponteiro genérico para os dados a inserir

                // Se for opção de inserir, lê o dado conforme o tipo
                if (opcao == 2 || opcao == 3 || opcao == 4) {
                    if (tipo == 0) {
                        int v;                // Variável temporária para int
                        printf("Digite valor inteiro: ");
                        scanf("%d", &v); clear_input();
                        dado = &v;            // Ponteiro para o valor lido
                    } else if (tipo == 1) {
                        float f;              // Variável temporária para float
                        printf("Digite valor float: ");
                        scanf("%f", &f); clear_input();
                        dado = &f;            // Ponteiro para o valor lido
                    } else {
                        printf("Digite string: ");
                        fgets(buffer, MAX_STR, stdin);          // Lê linha de texto
                        buffer[strcspn(buffer, "\n")] = '\0';  // Remove '\n'
                        dado = buffer;       // Ponteiro para a string lida
                    }
                }

                // Executa a operação com base na opção
                switch (opcao) {
                    case 2: {
                        iBegin(lista, dado);              // Insere no início
                        printf("Inserido no inicio.\n");
                        break;
                    }
                    case 3: {
                        iEnd(lista, dado);                // Insere no final
                        printf("Inserido no final.\n");
                        break;
                    }
                    case 4: {
                        printf("Posicao de insercao: ");
                        scanf("%d", &pos); clear_input();
                        iPosition(lista, pos, dado);     // Insere na posição especificada
                        printf("Inserido na posicao %d.\n", pos);
                        break;
                    }
                    case 5: {
                        if (tipo == 0) {
                            int vi;                      // Variável para valor removido
                            rBegin(lista, &vi);          // Remove do início
                            printf("Removido do inicio: %d\n", vi);
                        } else if (tipo == 1) {
                            float vf;
                            rBegin(lista, &vf);          // Remove float
                            printf("Removido do inicio: %.2f\n", vf);
                        } else {
                            char vs[MAX_STR];
                            rBegin(lista, vs);           // Remove string
                            printf("Removido do inicio: %s\n", vs);
                        }
                        break;
                    }
                    case 6: {
                        if (tipo == 0) {
                            int vi;
                            rEnd(lista, &vi);            // Remove do final
                            printf("Removido do final: %d\n", vi);
                        } else if (tipo == 1) {
                            float vf;
                            rEnd(lista, &vf);
                            printf("Removido do final: %.2f\n", vf);
                        } else {
                            char vs[MAX_STR];
                            rEnd(lista, vs);
                            printf("Removido do final: %s\n", vs);
                        }
                        break;
                    }
                    case 7: {
                        printf("Posicao de remocao: ");
                        scanf("%d", &pos); clear_input();
                        if (tipo == 0) {
                            int vi;
                            rPosition(lista, pos, &vi);// Remove em posição
                            printf("Removido da posicao %d: %d\n", pos, vi);
                        } else if (tipo == 1) {
                            float vf;
                            rPosition(lista, pos, &vf);
                            printf("Removido da posicao %d: %.2f\n", pos, vf);
                        } else {
                            char vs[MAX_STR];
                            rPosition(lista, pos, vs);
                            printf("Removido da posicao %d: %s\n", pos, vs);
                        }
                        break;
                    }
                }
                // Exibe o estado atual da lista
                printf("Estado atual: ");
                printList(lista, tipo);
            }
        }
        // Tratamento das buscas sem alterar a lista
        else if (opcao >= 8 && opcao <= 10) {
            if (lista == NULL) {
                printf("Crie a lista primeiro.\n");
            } else {
                int pos;
                if (opcao == 10) {
                    printf("Posicao de busca: ");
                    scanf("%d", &pos); clear_input();
                }
                if (tipo == 0) {
                    int vi;
                    if (opcao == 8) {
                        sBegin(lista, &vi);     // Busca primeiro elemento
                    } else if (opcao == 9) {
                        sEnd(lista, &vi);       // Busca último elemento
                    } else {
                        sPosition(lista, pos, &vi); // Busca em posição
                    }
                    printf("Elemento encontrado: %d\n", vi);
                } else if (tipo == 1) {
                    float vf;
                    if (opcao == 8) {
                        sBegin(lista, &vf);
                    } else if (opcao == 9) {
                        sEnd(lista, &vf);
                    } else {
                        sPosition(lista, pos, &vf);
                    }
                    printf("Elemento encontrado: %.2f\n", vf);
                } else {
                    char vs[MAX_STR];
                    if (opcao == 8) {
                        sBegin(lista, vs);
                    } else if (opcao == 9) {
                        sEnd(lista, vs);
                    } else {
                        sPosition(lista, pos, vs);
                    }
                    printf("Elemento encontrado: %s\n", vs);
                }
                printf("Estado atual: ");
                printList(lista, tipo);
            }
        }
        // Verifica se a lista está vazia
        else if (opcao == 11) {
            if (empty(lista) == SUCCESS) {
                printf("Lista está vazia.\n");
            } else {
                printf("Lista não está vazia.\n");
            }
        }
        // Conta e exibe o número de elementos
        else if (opcao == 12) {
            int total = countElements(lista);  // Conta elementos
            printf("Total de elementos: %d\n", total);
        }
        // Limpa todos os elementos mas mantém a estrutura
        else if (opcao == 13) {
            if (lista != NULL) {
                cleanDDLL(lista);             // Remove todos os nós
                printf("Lista esvaziada.\n");
                printf("Estado atual: ");
                printList(lista, tipo);
            } else {
                printf("Crie a lista primeiro.\n");
            }
        }
        // Destroi a lista completamente e reseta variáveis
        else if (opcao == 14) {
            if (lista != NULL) {
                dDDLL(&lista);               // Destroi a lista
                lista = NULL;                // Reset do ponteiro
                tipo = -1;                   // Reset do tipo
                printf("Lista destruída.\n");
            } else {
                printf("Nenhuma lista para destruir.\n");
            }
        }
        // Imprime a lista sem modificá-la
        else if (opcao == 15) {
            if (lista != NULL) {
                printList(lista, tipo);     // Chama printList
            } else {
                printf("Nenhuma lista criada.\n");
            }
        }
        // Opção inválida diferente de 0
        else if (opcao != 0) {
            printf("Opção inválida.\n");
        }
    } while (opcao != 0);  // Continua até escolher sair

    printf("Encerrando programa...\n");  // Mensagem de finalização
    return 0;                            // Retorna 0 para indicar execução bem-sucedida
}

*/

#include <stdio.h>
#include "main.h"

#define MAX_STR 50

int main(void) {
    /*teste de criação para cada tipo*/
    printf("Teste de criação das listas\n");

    pDDLL li_int = NULL;
    if (cDDLL(&li_int, sizeof(int)) == SUCCESS) {
        printf("Lista<int> criada com sucesso.\n");
    } else {
        printf("Falha ao criar Lista<int>.\n");
    }
    printf("Conteúdo int (deve estar vazia): ");
    printList(li_int, 0);

    pDDLL li_float = NULL;
    if (cDDLL(&li_float, sizeof(float)) == SUCCESS) {
        printf("Lista<float> criada com sucesso.\n");
    } else {
        printf("Falha ao criar Lista<float>.\n");
    }
    printf("Conteúdo float (deve estar vazia): ");
    printList(li_float, 1);

    pDDLL li_str = NULL;
    if (cDDLL(&li_str, MAX_STR * sizeof(char)) == SUCCESS) {
        printf("Lista<string> criada com sucesso.\n");
    } else {
        printf("Falha ao criar Lista<string>.\n");
    }
    printf("Conteúdo string (deve estar vazia): ");
    printList(li_str, 2);

    /*inserção de exemplo em cada tipo*/
    int vi = 100;
    iEnd(li_int, &vi);
    printf("Após inserir 100 em Lista<int>: ");
    printList(li_int, 0);

    float vf = 3.14f;
    iEnd(li_float, &vf);
    printf("Após inserir 3.14 em Lista<float>: ");
    printList(li_float, 1);

    char vs[MAX_STR] = "ola_mundo";
    iEnd(li_str, vs);
    printf("Após inserir \"ola_mundo\" em Lista<string>: ");
    printList(li_str, 2);

    /*destrói as três listas de teste*/
    dDDLL(&li_int);
    dDDLL(&li_float);
    dDDLL(&li_str);
    printf("Listas de teste destruídas.\n\n");

    /*exemplo com int*/
    printf("Exemplo Lista int \n");
    pDDLL lista = NULL;
    int value;

    /* 1. Criar a lista para armazenar ints */
    printf("Criando lista...\n");
    cDDLL(&lista, sizeof(int));
    printf("Após cDDLL: "); printList(lista, 0);

    /* 2. Inserir valores no final */
    int nums[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        iEnd(lista, &nums[i]);
        printf("Após iEnd(%d): ", nums[i]); printList(lista, 0);
    }

    /* 3. Inserir no início */
    int first = 5;
    iBegin(lista, &first);
    printf("Após iBegin(%d): ", first); printList(lista, 0);

    /* 4. Inserir na posição 2 */
    int mid = 15;
    iPosition(lista, 2, &mid);
    printf("Após iPosition(2, %d): ", mid); printList(lista, 0);

    /* 5. Buscar elementos e imprimir lista após cada busca */
    sBegin(lista, &value);
    printf("Após sBegin (value=%d): ", value); printList(lista, 0);
    sEnd(lista, &value);
    printf("Após sEnd (value=%d): ", value); printList(lista, 0);
    sPosition(lista, 3, &value);
    printf("Após sPosition(3) (value=%d): ", value); printList(lista, 0);

    /* 6. Remover elementos */
    rBegin(lista, &value);
    printf("Após rBegin (removeu=%d): ", value); printList(lista, 0);
    rEnd(lista, &value);
    printf("Após rEnd (removeu=%d): ", value); printList(lista, 0);
    rPosition(lista, 1, &value);
    printf("Após rPosition(1) (removeu=%d): ", value); printList(lista, 0);

    /* 7. Contagem e vazio */
    printf("Elementos restantes (countElements): %d\n", countElements(lista));
    printf("Vazia? %s\n", empty(lista)==SUCCESS ? "Sim" : "Não");

    /* 8. Limpar e reutilizar */
    cleanDDLL(lista);
    printf("Após cleanDDLL: "); printList(lista, 0);
    printf("Elementos após clean: %d\n", countElements(lista));

    /* 9. Destruir lista */
    dDDLL(&lista);
    printf("Após dDDLL: "); printList(lista, 0);
    printf("Lista destruída.\n");

    return 0;
}
