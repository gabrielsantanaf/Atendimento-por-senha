# Atendimento por Senhas

Dois programas em C que compartilham uma fila circular gravada no arquivo `fila.dat`:

- **cliente**: gera uma senha nova e consulta quantas pessoas estão na frente.
- **atendente**: chama a próxima senha, mostra a fila e roda a demonstração fila linear × circular.

Os programas não conversam diretamente: um grava a fila no arquivo e o outro lê depois.
O acesso é protegido pela trava `fila.lock`.

A documentação de arquitetura, backlog e sprints fica em [docs/](docs/).

## Estrutura

| Arquivo | Responsabilidade | Dupla |
|---|---|---|
| `fila.h` / `fila.c` | Struct da fila, enfileirar, desenfileirar, posição (vetor, índices, módulo) | A |
| `armazenamento.h` / `armazenamento.c` | Ler/gravar `fila.dat`, travar/destravar com `fila.lock` | C |
| `cliente.c` | Menu do cliente (`main` próprio) | D |
| `atendente.c` | Menu do atendente + demonstração linear × circular (`main` próprio) | D / B |
| `testes/teste_fila.c` | Testes da fila (F-05) | A / B |
| `docs/` | Documentação do projeto | — |

`fila.dat` e `fila.lock` são criados ao rodar e não vão para o repositório.

As assinaturas de `fila.h` e `armazenamento.h` são o contrato entre as duplas: não mude sem combinar com o time.

## Compilar

```sh
gcc -Wall cliente.c fila.c armazenamento.c -o cliente
gcc -Wall atendente.c fila.c armazenamento.c -o atendente
gcc -Wall -I. testes/teste_fila.c fila.c -o teste_fila
```

## Executar

Abra dois terminais na pasta do projeto:

```sh
./cliente      # terminal 1
./atendente    # terminal 2
```

Se um programa fechar no meio e deixar o `fila.lock`, apague o arquivo na mão.

## Definition of Done

- [ ] Compila sem erros e sem warnings com `gcc -Wall`.
- [ ] Cumpre o critério de aceite, mostrado rodando.
- [ ] Foi revisada por alguém de outra dupla.
- [ ] Está no repositório principal, sem quebrar o que já funcionava.
