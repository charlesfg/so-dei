# Proof-of-Work in C

## Concept
Proof-of-Work (PoW) is a computational process used to secure blockchains by requiring miners to solve a cryptographic puzzle before adding a new block.

## How it Works
The program hashes the block data with a numeric nonce  (number used once) until a hash is found that starts with a given number of leading zeros (difficulty).

## Adjusting Difficulty
Modify the `difficulty` parameter in `main.c`. Higher values increase computation time.

## Running the Program
1. Compile using `make`
2. Run with `./pow_test`

The program will display statistics such as time taken, number of operations, and hash rate.



# Requisitos para Implementação de uma Função de Proof-of-Work (PoW)

## Introdução
A prova de trabalho (Proof-of-Work - PoW) é um mecanismo utilizado em sistemas como blockchain para validar transações e evitar fraudes. Neste exercício, os alunos devem implementar um componente que realiza PoW com **dificuldade fixa de 5**, seguindo a interface definida abaixo.

## Interface a Ser Implementada

O componente deve seguir a seguinte interface definida no arquivo `pow.h`:

```c
/* pow.h - Proof-of-Work header file */
#ifndef POW_H
#define POW_H

#define HASH_SIZE 33 // SHA256_DIGEST_LENGTH

const int PoW_difficulty = 5;

typedef struct
{
    int nonce;
    char hash[HASH_SIZE];
    double elapsed_time;
    int operations;
} PoWResult;

void compute_sha256(const char *input, char *output);
PoWResult proof_of_work(const char *block_data);
int verify_nonce(const char *block_data, int nonce);

#endif /* POW_H */
```

## Explicação dos Componentes

### **1. O que é SHA-256?**

SHA-256 (Secure Hash Algorithm 256 bits) é uma função criptográfica que gera um hash de tamanho fixo de **256 bits (ou 32 bytes)** a partir de qualquer entrada de dados. O hash é determinístico, ou seja, sempre produzirá a mesma saída para a mesma entrada.

### **2. Biblioteca OpenSSL para SHA-256**
Para computar o hash SHA-256 em C, usaremos a biblioteca OpenSSL, que fornece funções otimizadas para esse cálculo.

#### **Inclusão da Biblioteca**
Para utilizar a funcionalidade de hash, inclua o cabeçalho necessário no seu código:

```c
#include <openssl/sha.h>
```

#### **Uso da Função SHA-256**
A função `SHA256()` da OpenSSL é utilizada para calcular o hash:

```c
void compute_sha256(const char *input, char *output) 
```

### **3. Proof-of-Work com Dificuldade 5**
O objetivo do Proof-of-Work é encontrar um número **nonce** tal que o hash gerado a partir dos dados do bloco tenha **pelo menos 5 zeros iniciais**. Isso significa que, para validar um bloco, devemos encontrar um nonce adequado.

O processo funciona da seguinte forma:

1. **Inicializar o nonce** com 0.
2. **Concatenar o nonce com os dados do bloco** e calcular o hash SHA-256.
3. **Verificar se o hash começa com pelo menos 5 zeros (`00000...`)**.
4. **Se a condição for atendida, armazenar o nonce e o hash resultante**.
5. **Caso contrário, incrementar o nonce e repetir o processo** até encontrar um hash válido.

Exemplo de implementação da função `proof_of_work`:



### **4. Verificação de um Nonce**
A função `verify_nonce` verifica se um nonce fornecido é válido para um determinado bloco. O processo consiste em calcular o hash novamente e verificar se ele atende à dificuldade exigida.

```c
int verify_nonce(const char *block_data, int nonce) 
```

### **5. Testando a Implementação**
Para testar a implementação, crie um programa que execute `proof_of_work` em um bloco de dados e depois valide o nonce encontrado com `verify_nonce`.

```c
#include <stdio.h>
#include "pow.h"

int main() {
    const char *block_data = "Block #1: Alice pays Bob 5 coins";
    
    printf("Executando Proof-of-Work...\n");
    PoWResult result = proof_of_work(block_data);
    
    printf("Nonce encontrado: %d\n", result.nonce);
    printf("Hash: %s\n", result.hash);
    printf("Tempo: %.4f segundos\n", result.elapsed_time);
    printf("Operações realizadas: %d\n", result.operations);
    
    if (verify_nonce(block_data, result.nonce)) {
        printf("Nonce verificado com sucesso!\n");
    } else {
        printf("Falha na verificação do nonce!\n");
    }
    
    return 0;
}
```

## **Conclusão**
Esta atividade introduz os conceitos fundamentais do Proof-of-Work e do uso de SHA-256 para validação. Com essa implementação, os alunos aprendem sobre o uso da biblioteca OpenSSL, manipulação de strings em C e como estruturar uma função de PoW eficiente.

Para aprofundamento, pode-se testar diferentes dificuldades e medir o impacto no tempo de execução. Boa prática de programação e otimização são fundamentais para tornar o processo mais eficiente.





# Requisitos para Implementação de uma Função de Proof-of-Work (PoW)

## Introdução
A prova de trabalho (Proof-of-Work - PoW) é um mecanismo utilizado em sistemas como blockchain para validar transações e evitar fraudes. Neste exercício, os alunos devem implementar um componente que realiza PoW com **dificuldade fixa de 5**, seguindo a interface definida abaixo.

## Interface a Ser Implementada
O componente deve seguir a seguinte interface definida no arquivo `pow.h`:

```c
/* pow.h - Proof-of-Work header file */
#ifndef POW_H
#define POW_H

#define HASH_SIZE 33 // SHA256_DIGEST_LENGTH

const int PoW_difficulty = 5;

typedef struct
{
    int nonce;
    char hash[HASH_SIZE];
    double elapsed_time;
    int operations;
} PoWResult;

void compute_sha256(const char *input, char *output);
PoWResult proof_of_work(const char *block_data);
int verify_nonce(const char *block_data, int nonce);

#endif /* POW_H */
```

## Explicação dos Componentes

### **1. O que é SHA-256?**

SHA-256 (Secure Hash Algorithm 256 bits) é uma função criptográfica que gera um hash de tamanho fixo de **256 bits (ou 32 bytes)** a partir de qualquer entrada de dados. O hash é determinístico, ou seja, sempre produzirá a mesma saída para a mesma entrada.

### **2. Biblioteca OpenSSL para SHA-256**
Para computar o hash SHA-256 em C, usaremos a biblioteca OpenSSL, que fornece funções otimizadas para esse cálculo.

#### **Inclusão da Biblioteca**
Para utilizar a funcionalidade de hash, inclua o cabeçalho necessário no seu código:

```c
#include <openssl/sha.h>
```

#### **Uso da Função SHA-256 e Conversão para String**
O resultado da função `SHA256()` é um vetor de **bytes binários**, mas para ser armazenado e manipulado facilmente, ele precisa ser convertido para **hexadecimal**. Isso acontece porque cada byte no hash pode conter qualquer valor de `0x00` a `0xFF`, o que não pode ser representado diretamente como uma string legível.

A conversão é feita imprimindo cada byte em formato hexadecimal (`%02x`), onde cada byte do hash é convertido em **dois caracteres hexadecimais**. Como o SHA-256 gera **32 bytes**, a string final terá **64 caracteres**, representando o hash hexadecimal.

#### **Exemplo Prático**
Imagine que o hash resultante da função SHA-256 seja:
```
{ 0xA3, 0x5F, 0x12, 0xC8, ... }
```
Cada um desses valores binários precisa ser convertido para hexadecimal:
```
"a35f12c8..."
```
Isso permite que o hash possa ser facilmente armazenado e comparado como uma string. Se usássemos os bytes diretamente, teríamos dificuldades ao exibir e comparar os valores, já que alguns bytes podem conter caracteres não imprimíveis.

### **3. Proof-of-Work com Dificuldade 5**
O objetivo do Proof-of-Work é encontrar um número **nonce** tal que o hash gerado a partir dos dados do bloco tenha **pelo menos 5 zeros iniciais**. Isso significa que, para validar um bloco, devemos encontrar um nonce adequado.

O processo funciona da seguinte forma:

1. **Inicializar o nonce** com 0.
2. **Concatenar o nonce com os dados do bloco** e calcular o hash SHA-256.
3. **Verificar se o hash começa com pelo menos 5 zeros (`00000...`)**.
4. **Se a condição for atendida, armazenar o nonce e o hash resultante**.
5. **Caso contrário, incrementar o nonce e repetir o processo** até encontrar um hash válido.

### **4. Verificação de um Nonce**
A função `verify_nonce` verifica se um nonce fornecido é válido para um determinado bloco. O processo consiste em calcular o hash novamente e verificar se ele atende à dificuldade exigida.

### **5. Testando a Implementação**
Para testar a implementação, crie um programa que execute `proof_of_work` em um bloco de dados e depois valide o nonce encontrado com `verify_nonce`.

## **Conclusão**
Esta atividade introduz os conceitos fundamentais do Proof-of-Work e do uso de SHA-256 para validação. Com essa implementação, os alunos aprendem sobre o uso da biblioteca OpenSSL, manipulação de strings em C e como estruturar uma função de PoW eficiente.

Para aprofundamento, pode-se testar diferentes dificuldades e medir o impacto no tempo de execução. Boa prática de programação e otimização são fundamentais para tornar o processo mais eficiente.

