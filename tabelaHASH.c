// como rodar:
// gcc -o tabelaHASH.c main.c -Wall
// ./auth
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10


typedef struct usuario {
    char username[50];
    unsigned long hash_senha;
    struct usuario *proximo;  // Ponteiro para próximo usuário na lista
} usuario;


typedef struct Hash {
    int qtd;
    int tamanho;
    usuario **itens;  // Array de ponteiros para listas encadeadas
} Hash;

// ==================== FUNÇÕES PARA GERAR HASH ====================


int chaveDivisao(char *str, int tamanho) {
    unsigned long hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash += (unsigned char)str[i];
    }
    return (int)(hash % (unsigned long)tamanho);
}

// Função hash (djb2) para gerar hash para senha
unsigned long valorString(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// ==================== FUNÇÕES DA TABELA HASH ====================


Hash* criaHash(int tamanho) {
    Hash *ha = (Hash*) malloc(sizeof(Hash));
    if (ha != NULL) {
        ha->tamanho = tamanho;
        ha->qtd = 0;
        ha->itens = malloc(tamanho * sizeof(usuario*));
        if (ha->itens == NULL) {
            free(ha);
            return NULL;
        }
        
        for (int i = 0; i < tamanho; i++) {
            ha->itens[i] = NULL;
        }
    }
    return ha;
}

// Insere usuário usando encadeamento separado
int insereHash(Hash *ha, char *username, char *senha) {
    if (ha == NULL)
        return 0;
    

    int pos = chaveDivisao(username, ha->tamanho);
    
    
    usuario *atual = ha->itens[pos];
    while (atual != NULL) {
        if (strcmp(atual->username, username) == 0) {
            printf("Erro: Usuario ja existe!\n");
            return 0;
        }
        atual = atual->proximo;
    }
    
    
    usuario *novo = malloc(sizeof(usuario));
    if (novo == NULL)
        return 0;
    
    strcpy(novo->username, username);
    novo->hash_senha = valorString(senha);
    

    novo->proximo = ha->itens[pos];
    ha->itens[pos] = novo;
    
    ha->qtd++;
    return 1;
}


usuario* buscaHash(Hash *ha, char *username) {
    if (ha == NULL)
        return NULL;
    
    
    int pos = chaveDivisao(username, ha->tamanho);
    
    // Percorre a lista encadeada dessa posição
    usuario *atual = ha->itens[pos];
    while (atual != NULL) {
        if (strcmp(atual->username, username) == 0) {
            return atual;  
        }
        atual = atual->proximo;
    }
    
    return NULL;
}


int removeHash(Hash *ha, char *username) {
    if (ha == NULL)
        return 0;
    
    int pos = chaveDivisao(username, ha->tamanho);
    
    usuario *atual = ha->itens[pos];
    usuario *anterior = NULL;
    
    // Percorre a lista procurando o usuário
    while (atual != NULL) {
        if (strcmp(atual->username, username) == 0) {
            // Remove da lista
            if (anterior == NULL) {
                // É o primeiro da lista
                ha->itens[pos] = atual->proximo;
            } else {
                // Está no meio ou fim
                anterior->proximo = atual->proximo;
            }
            
            free(atual);
            ha->qtd--;
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    
    return 0;  // Não encontrou
}

// Libera toda a memória
void liberaHash(Hash *ha) {
    if (ha != NULL) {
        // Libera cada lista encadeada
        for (int i = 0; i < ha->tamanho; i++) {
            usuario *atual = ha->itens[i];
            while (atual != NULL) {
                usuario *temp = atual;
                atual = atual->proximo;
                free(temp);
            }
        }
        free(ha->itens);
        free(ha);
    }
}

// Imprime a tabela mostrando colisões
void imprimeHash(Hash *ha) {
    if (ha != NULL) {
        printf("\n===== TABELA HASH=====\n");
        printf("Tamanho: %d | Total de usuarios: %d\n\n", ha->tamanho, ha->qtd);
        
        for (int i = 0; i < ha->tamanho; i++) {
            printf("[%d]: ", i);
            
            usuario *atual = ha->itens[i];
            
            if (atual == NULL) {
                printf("vazio\n");
            } else {
                int contador = 0;
                // Percorre a lista encadeada
                while (atual != NULL) {
                    if (contador > 0) printf(" -> ");
                    printf("%s (hash: %lu)", atual->username, atual->hash_senha);
                    atual = atual->proximo;
                    contador++;
                }
                
                // Indica se houve colisão
                if (contador > 1) {
                    printf("  [COLISAO: %d usuarios]\n", contador);
                } else {
                    printf("\n");
                }
            }
        }
        printf("================================================\n\n");
    }
}

// ==================== SISTEMAS DE AUTENTICAÇÃO ====================

void cadastrar(Hash *ha) {
    char username[50], senha[50];
    
    printf("\n--- CADASTRO ---\n");
    printf("Username: ");
    scanf("%49s", username);
    printf("Senha: ");
    scanf("%49s", senha);
    
    if (insereHash(ha, username, senha)) {
        printf("Usuario cadastrado com sucesso!\n");
    } else {
        printf("Erro ao cadastrar!\n");
    }
}

void login(Hash *ha) {
    char username[50], senha[50];
    
    printf("\n--- LOGIN ---\n");
    printf("Username: ");
    scanf("%49s", username);
    printf("Senha: ");
    scanf("%49s", senha);
    
    usuario *user = buscaHash(ha, username);
    
    if (user == NULL) {
        printf("Usuario nao encontrado!\n");
        return;
    }
    
    unsigned long hash_digitado = valorString(senha);
    
    if (hash_digitado == user->hash_senha) {
        printf("Login OK! Bem-vindo, %s!\n", username);
    } else {
        printf("Senha incorreta!\n");
    }
}

void remover(Hash *ha) {
    char username[50];
    
    printf("\n--- REMOVER ---\n");
    printf("Username: ");
    scanf("%49s", username);
    
    if (removeHash(ha, username)) {
        printf("Usuario removido!\n");
    } else {
        printf("Usuario nao encontrado!\n");
    }
}

void menu() {
    printf("\n=== SISTEMA DE AUTENTICACAO ===\n");
    printf("1. Cadastrar\n");
    printf("2. Login\n");
    printf("3. Remover\n");
    printf("4. Imprimir tabela\n");
    printf("5. Sair\n");
    printf("Opcao: ");
}



int main() {
    Hash *ha = criaHash(SIZE);
    
    if (ha == NULL) {
        printf("Erro ao criar tabela!\n");
        return 1;
    }
    
    int opcao;
    
    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = 0;
        }
        
        switch(opcao) {
            case 1:
                cadastrar(ha);
                break;
            case 2:
                login(ha);
                break;
            case 3:
                remover(ha);
                break;
            case 4:
                imprimeHash(ha);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 5);
    
    liberaHash(ha);
    return 0;
}