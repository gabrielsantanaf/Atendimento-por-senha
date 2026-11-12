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
| `compilar.sh` / `compilar.bat` | Compilam os três programas (Linux/Docker e Windows) | — |
| `Dockerfile` / `docker-compose.yml` | Ambiente de desenvolvimento com gcc, gdb e valgrind | — |

`fila.dat` e `fila.lock` são criados ao rodar e não vão para o repositório.

As assinaturas de `fila.h` e `armazenamento.h` são o contrato entre as duplas: não mude sem combinar com o time.

## Opção 1: Docker (recomendado, ambiente igual para todos)

Precisa do Docker Desktop aberto. A pasta do projeto é montada dentro do container:
você edita no VS Code normalmente e compila/roda no container.

```sh
docker compose up -d --build      # sobe o container (só na primeira vez ou após mudar o Dockerfile)
docker compose exec dev bash      # abre um terminal dentro do container
```

Dentro do container:

```sh
./compilar.sh
./teste_fila
```

Para testar os dois programas juntos, abra **dois terminais** no Windows e rode
`docker compose exec dev bash` em cada um. Depois:

```sh
./cliente      # terminal 1
./atendente    # terminal 2
```

Os dois rodam no mesmo container e enxergam o mesmo `fila.dat` e `fila.lock`.

Para desligar: `docker compose down`.

O container também tem `gdb` e `valgrind` para depuração.

## Opção 2: Windows nativo (gcc/MinGW no PATH)

```bat
compilar.bat
```

Depois, em dois terminais na pasta do projeto:

```bat
cliente.exe
atendente.exe
```

## Comandos de compilação (o que os scripts fazem)

```sh
gcc -Wall cliente.c fila.c armazenamento.c -o cliente
gcc -Wall atendente.c fila.c armazenamento.c -o atendente
gcc -Wall -I. testes/teste_fila.c fila.c -o teste_fila
```

> Não misture: um `fila.dat` criado no Windows pode não ser lido corretamente pelo
> programa compilado no Linux, e vice-versa. Se trocar de ambiente, apague o `fila.dat`.

Se um programa fechar no meio e deixar o `fila.lock`, apague o arquivo na mão.

## Definition of Done

- [ ] Compila sem erros e sem warnings com `gcc -Wall`.
- [ ] Cumpre o critério de aceite, mostrado rodando.
- [ ] Foi revisada por alguém de outra dupla.
- [ ] Está no repositório principal, sem quebrar o que já funcionava.
