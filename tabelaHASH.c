
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

typedef struct usuario {
    char username[50];  
    unsigned long hash_senha;
    int ocupado;
} usuario;

typedef struct Hash {
    int qtd;
    int tamanho;
    usuario **itens;
} Hash;


int chaveDivisao(char *str, int tamanho) {
    unsigned long hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash += (unsigned char)str[i];
    }
    return (int)(hash % (unsigned long)tamanho);
}


unsigned long valorString(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

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

int insereHash(Hash *ha, char *username, char *senha) {
    if (ha == NULL || ha->qtd == ha->tamanho)
        return 0;
    
    int pos = chaveDivisao(username, ha->tamanho);
    int pos_inicial = pos;
    int primeiro_deletado = -1;
    
    while (ha->itens[pos] != NULL) {
        if (ha->itens[pos]->ocupado == 1) {
            if (strcmp(ha->itens[pos]->username, username) == 0) {
                printf("Erro: Usuario ja existe!\n");
                return 0;
            }
        } else {
            
            if (primeiro_deletado == -1)
                primeiro_deletado = pos;
        }
        pos = (pos + 1) % ha->tamanho;
        
        if (pos == pos_inicial)
            break;
    }
    
    if (primeiro_deletado != -1) {
       
        usuario *novo = ha->itens[primeiro_deletado];
        strcpy(novo->username, username);
        novo->hash_senha = valorString(senha);
        novo->ocupado = 1;
        ha->qtd++;
        return 1;
    }

   
    usuario *novo = malloc(sizeof(usuario));
    if (novo == NULL)
        return 0;
    
    strcpy(novo->username, username);  
    novo->hash_senha = valorString(senha);
    novo->ocupado = 1;
    
    ha->itens[pos] = novo;
    ha->qtd++;
    return 1;
}

usuario* buscaHash(Hash *ha, char *username) {
    if (ha == NULL)
        return NULL;
    
    int pos = chaveDivisao(username, ha->tamanho);
    int pos_inicial = pos;
    
    while (ha->itens[pos] != NULL) {
        if (ha->itens[pos]->ocupado == 1 && strcmp(ha->itens[pos]->username, username) == 0) {
            return ha->itens[pos];
        }
        pos = (pos + 1) % ha->tamanho;
        
        if (pos == pos_inicial)
            break;
    }
    
    return NULL;
}

int removeHash(Hash *ha, char *username) {
    if (ha == NULL)
        return 0;
    
    int pos = chaveDivisao(username, ha->tamanho);
    int pos_inicial = pos;
    
    while (ha->itens[pos] != NULL) {
        if (ha->itens[pos]->ocupado == 1 && strcmp(ha->itens[pos]->username, username) == 0) {
            
            ha->itens[pos]->ocupado = 0;
            ha->qtd--;
            return 1;
        }
        pos = (pos + 1) % ha->tamanho;
        
        if (pos == pos_inicial)
            break;
    }
    
    return 0;
}

void liberaHash(Hash *ha) {
    if (ha != NULL) {
        for (int i = 0; i < ha->tamanho; i++) {
            if (ha->itens[i] != NULL) {
                free(ha->itens[i]);
            }
        }
        free(ha->itens);
        free(ha);
    }
}

void imprimeHash(Hash *ha) {
    if (ha != NULL) {
        printf("\n===== TABELA HASH =====\n");
        printf("Tamanho: %d | Ocupados: %d\n\n", ha->tamanho, ha->qtd);
        
        for (int i = 0; i < ha->tamanho; i++) {
            printf("[%d] = ", i);
            if (ha->itens[i] != NULL && ha->itens[i]->ocupado == 1) {
                printf("%s (hash: %lu)\n", 
                       ha->itens[i]->username, 
                       ha->itens[i]->hash_senha);
            } else {
                printf("vazio\n");
            }
        }
        printf("=======================\n\n");
    }
}

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

