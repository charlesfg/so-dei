# Requisitos para Implementação de uma Função de Proof-of-Work (PoW) em C

## Introdução
A prova de trabalho (Proof-of-Work - PoW) é um mecanismo utilizado em sistemas como blockchain para validar transações e evitar fraudes. 
Neste projeto a dificuldade será abstraída através do componente implementado pela interface [`pow.h`](pow.h). 


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


### **3. Proof-of-Work com [`pow.h`](pow.h)**
O objetivo do Proof-of-Work é encontrar um número **nonce** tal que o hash gerado a partir dos dados do bloco tenha **quantos zeros iniciais** especificados pela dificuldade. Isso significa que, para validar um bloco, devemos encontrar um nonce adequado.

O processo funciona da seguinte forma:

1. **Inicializar o nonce** com 0.
2. **Concatenar o nonce com os dados do bloco** e calcular o hash SHA-256.
3. **Verificar se o hash começa o número de zeros ex: (`00000...`)**.
4. **Se a condição for atendida, armazenar o nonce e o hash resultante**.
5. **Caso contrário, incrementar o nonce e repetir o processo** até encontrar um hash válido.


Um problema que pode ocorrer é não se encontrar um **nonce** em tempo hábil. Assim, nosso componente só irá tentar computar nounces de 0 até [`POW_MAX_OPS`](pow.h?plain=1#L7). Por isso o resultado [`PoWResult`](pow.h?plain=1#L27) possui um campo de erro que deve ser validado.  

Uma estratégia para  contornar esse problem é retentar a construção do hash alterando o `timestamp`, como pode ser visto no código [`hash_block_sample`](hash_block_sample.c?plain=1#L64).



# **Exemplo funcional**

Junto à biblioteca definimos uma estrutura básica das transações para exemplificar o uso e facilitar a compreensão. Para o projeto **não é necessário** utilizar as definições aqui disponibilizadas.  
Apenas apresentamos para poder exemplificar o funcionamento completo da PoW. 



A seguir temos uma tabela explicando cada tipo de dado do [`deichain.h`](deichain.h).



| **Estrutura**        | <sub>**Campo**</sub>                 | **Descrição** |
|----------------------|--------------------------------------|---------------|
| [`Transaction`](./deichain.h?plain=1#L21)         | <sub>`tx_id`</sub>                   | ID única da transação (ex: PID + número). |
|                      | <sub>`reward`</sub>                  | Recompensa associada à prova de trabalho (PoW). |
|                      | <sub>`value`</sub>                   | Valor ou quantidade transferida. |
|                      | <sub>`timestamp`</sub>               | Momento da criação da transação. |
| [`TransactionBlock`](deichain.h?plain=1#L30)    | <sub>`txb_id`</sub>                  | ID única do bloco (ex: ThreadID + número). |
|                      | <sub>`previous_block_hash`</sub>     | Hash do bloco anterior (SHA-256 em hex + terminador). |
|                      | <sub>`timestamp`</sub>               | Tempo de criação do bloco. |
|                      | <sub>`transactions`</sub>            | Ponteiro para um array de transações. |
|                      | <sub>`nonce`</sub>                   | Solução da PoW para o bloco. |
| **Variável externa** | <sub>`transactions_per_block`</sub>  | Número de transações por bloco **deve ser definida externamente**, por exemplo no controler. |
| **Função inline**    | <sub>`get_transaction_block_size()`</sub> | Calcula o tamanho total de um [`TransactionBlock`](deichain.h?plain=1#L30), incluindo as transações. |


O código do [`hash_block_sample`](hash_block_sample.c?plain=1#L64) implementa uma simulação simplificada do projeto com mecanismo de **Proof of Work (PoW)**, organizando transações em blocos, gerando hashes SHA-256, e ajustando a dificuldade com base em recompensas.


O programa simula a criação de um pequeno ledger de blockchain com 4 blocos. Cada bloco:
1. Contém 3 transações aleatórias.
2. É encadeado pelo hash do bloco anterior.
3. Exige a realização de **Proof of Work** para encontrar um nonce válido.
4. É impresso na tela com seus dados e, ao final, todos os blocos são exibidos. (como deve ser feito no log do projeto)


Para o projeto vocês podem se espelhar na presente implementação para adaptar à necessidade das vossas iplementações.

---

##  **Resumo por Arquivo/Função**

---

### Arquivo [`pow.h`](pow.h)

- **`compute_sha256`**: calcula o hash SHA-256 de um [`TransactionBlock`](deichain.h?plain=1#L30) e o salva em formato hexadecimal.
- **`proof_of_work`**: tenta encontrar um `nonce` (número aleatório) que faz com que o hash do bloco atenda a um critério de dificuldade baseado em recompensa. Mede o tempo gasto, número de operações e retorna sucesso ou erro.
- **`verify_nonce`**: verifica se o `nonce` atual do bloco gera um hash válido.
- **`check_difficulty`**: compara o hash com um padrão de dificuldade baseado na recompensa do bloco.
- **`getDifficultFromReward`**: mapeia o valor da recompensa para um nível de dificuldade (EASY, NORMAL, HARD).

---

### Arquivo [`deichain.h`](deichain.h)

- Define as estruturas centrais:
  - **[`Transaction`](./deichain.h?plain=1#L21)**: contém ID, recompensa, valor e timestamp.
  - **[`TransactionBlock`](deichain.h?plain=1#L30)**: representa um bloco com ID, hash anterior, timestamp, transações e nonce.
- **`get_transaction_block_size()`**: função inline que calcula o tamanho de um bloco com base no número de transações. Encerra o programa se `transactions_per_block` não for definido.

---

### Arquivo [`hash_block_sample.c`](hash_block_sample.c)

- **`generate_random_transaction`**: cria uma transação aleatória com ID única e valores pseudoaleatórios de recompensa e valor.
- **`generate_random_block`**: monta um novo bloco com transações aleatórias, realiza PoW até sucesso, e retorna o bloco.
- **`print_block_info`**: imprime os dados principais de um bloco e suas transações.
- **`blkcpy`**: copia os dados de um bloco para outro (exceto memória de transações, que é compartilhada).
- **`dump_ledger`**: imprime todos os blocos armazenados no ledger.
- **`main()`**: inicializa a seed do gerador de aleatórios, define o número de transações por bloco, calcula o hash inicial de um bloco vazio e depois gera uma sequência de blocos em cadeia, armazenando no ledger.




---

## ⚠️ **Observações Importantes**

### **Gerenciamento de memória (malloc/calloc/free)**:
   - O código aloca memória para transações com `malloc`, mas **não libera** (`free`) posteriormente. Isso gera vazamentos de memória.
   - O [`blkcpy`](hash_block_sample.c?plain=1#L73) copia o ponteiro de transações sem duplicar o conteúdo. Isso pode causar **compartilhamento de memória** entre blocos.



### **Função `compute_sha256`**

A função `compute_sha256` recebe um bloco de transações e retorna uma **representação hexadecimal do hash SHA-256** do bloco. Como a estrutura contém ponteiros, ela é **serializada** para que todos os dados (inclusive as transações apontadas) fiquem em uma forma contínua de bytes. Depois, o hash é calculado e convertido para uma string.


```c
unsigned char *buffer = serialize_block(block, &buffer_sz);
```

 * Serializa o bloco inteiro (estrutura com ponteiros) para um array contínuo de bytes. Isso é **necessário** porque o SHA-256 opera sobre uma sequência linear de dados, e não sobre estruturas com ponteiros.

> ⚠️ A função [`serialize_block()`](pow.c?plain=1#L28) transforma o [`TransactionBlock`](deichain.h?plain=1#L30) (que contém ponteiro para transações) em um array plano de bytes representando os dados do bloco de forma consistente e independente da arquitetura.


Essa função é central no sistema de blockchain do programa, pois ela garante que cada bloco tenha um identificador único e imutável baseado em seu conteúdo.

