#include <stdio.h>
#include <stdlib.h>


typedef struct hash {
    int qtd;
    int SIZE;
    int **itens;
}Hash;


Hash* criar_hash(int SIZE) {
    Hash* ha = (Hash*) malloc(sizeof(Hash));
    if (ha != NULL) {
        ha->SIZE = SIZE;
        ha->qtd = 0;
        ha->itens = malloc(SIZE * sizeof(int*));
        if (ha->itens == NULL) {
            free(ha);
            return NULL;
        }
        for (int i = 0; i < SIZE; i++) {
            ha->itens[i] = NULL;
        
        }
    } 
    return ha;
}

int inserir_hash_semcolisao(Hash* ha, int valor){
    if (ha == NULL || ha->qtd == ha->SIZE)
        return 0; // Falha na inserção

    int pos = funcaohash(valor, ha->SIZE);
    if (ha->itens[pos] != NULL)
        return 0; // Colisão ocorreu
    int* novo = malloc(sizeof(int));
    if (novo == NULL)
        return 0; // Falha na alocação
    *novo = valor;
    ha->itens[pos] = novo;
    ha->qtd++;
    return 1; // Sucesso na inserção
}    

int busca_hash_semcolisao(Hash *ha, int chave, int *out) {
    if (ha == NULL) 
        return 0; // Falha na busca
    int pos = fucaohash(chave, ha->SIZE);
    if (ha->itens[pos] == NULL) 
        return 0; // Não encontrado
    
    *out = *(ha->itens[pos]);
    return 1;

}

int chave_divisao(int chave, int SIZE) {
    return chave % SIZE;
}

void imprimir_hash(Hash*ha) {
    if(ha!= NULL) {
        printf("Tabela(size%d,qtd = %d)\n", ha->SIZE, ha->qtd);
        for(int i = 0; i <ha->SIZE; i ++) {
            if(ha->itens[i] != NULL)
                printf("[%d] = %d\n", i,*ha->itens[i]);
            else
                printf("[%d] = NULL\n", i);
        }
    }
}



int main() {
    int SIZE = 10, valor, resultado;
    Hash* ha = criar_hash(SIZE);
    
    inserir_hash_semcolisao(ha, 50 );
    
    imprimir_hash(ha);

    int encontrado;
    if (busca_hash_semcolisao(ha, 50, &encontrado))
        printf("Valor buscado: %d\n", encontrado);
    else
        printf("Valor 50 nao encontrado\n");

    liberar_hash(ha);
    return 0;
}
    


