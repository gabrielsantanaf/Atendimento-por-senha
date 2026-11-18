# Sprint 1 — Dupla C (Armazenamento e trava)

**Histórias:** A-01 (3 pts) e A-02 (3 pts), 6 pontos no total.
**Branch:** `dupla-c/armazenamento`
**Arquivos da dupla:** `armazenamento.h`, `armazenamento.c`, `testes/teste_armazenamento.c`
**Ambiente:** Linux, dentro do Docker (`docker compose exec dev bash`).

## Dependências

| Precisa de | Situação |
|---|---|
| `inicializar()` da Dupla A (F-01), usada quando não existe `fila.dat` | Pronta (PR #1) |
| Nada da Dupla D. Ela só vai usar o armazenamento na I-05 (sprint 2) | — |

As assinaturas de `armazenamento.h` são contrato: **não mudar** sem combinar com o time.

## A-01 — `carregar_fila()` e `salvar_fila()`

**Critério de aceite:** gravar, fechar e reabrir devolve a fila idêntica; sem arquivo, cria fila nova.

- [x] `carregar_fila`: `fread` da struct inteira; sem arquivo (`ENOENT`), chama `inicializar()`
- [x] Detectar arquivo corrompido ou gravado com outro `TAM` (tamanho errado ou índices fora do vetor)
- [x] `salvar_fila`: grava num temporário `fila.dat.tmp.<pid>` e troca com `rename()`
- [x] Teste: sem arquivo → fila nova
- [x] Teste: um processo grava e **outro processo** (`fork`) lê a fila idêntica
- [ ] Teste manual: gerar senhas, fechar o programa, abrir de novo, conferir com "Ver fila" (depende dos menus da Dupla D)

## A-02 — Trava com `fila.lock`

**Critério de aceite:** o segundo programa espera enquanto o primeiro tem a trava; `fila.lock` é apagado no fim.

- [x] `travar`: `fopen("fila.lock", "wx")` em loop, esperando 100 ms (`nanosleep`) entre as tentativas
- [x] Gravar o PID dentro do `fila.lock` (`cat fila.lock` mostra quem está com a trava)
- [x] Aviso depois de 3 s esperando ("se ninguém estiver usando, apague o fila.lock")
- [x] Se o programa sair com `exit()` segurando a trava, o `fila.lock` é apagado (`atexit`)
- [x] `destravar`: `remove("fila.lock")`
- [x] Teste: o segundo processo espera o primeiro soltar a trava
- [x] Teste: 4 processos × 25 gravações com trava → nenhuma perdida
- [ ] Teste manual com dois terminais (já fica pronto para a A-03 da sprint 2)

## Como testar

```sh
docker compose exec dev bash
./compilar.sh
./teste_armazenamento               # testes de aceite da A-01 e A-02
./teste_armazenamento --sem-trava   # mostra senhas se perdendo sem a trava
```

Os testes rodam numa pasta em `/tmp` e não mexem no `fila.dat` do projeto.

## Para a review (o que mostrar rodando)

1. `./teste_armazenamento` com tudo `[OK]`.
2. `./teste_armazenamento --sem-trava`: com trava não se perde nada; sem trava, várias gravações somem. É a condição de corrida do documento acontecendo de verdade.
3. (Só depois da I-05, quando os menus usarem a trava) Em dois terminais: no primeiro, `touch fila.lock`; no segundo, rodar uma ação do menu. Ela fica esperando até o `rm fila.lock`.

## O que a dupla precisa saber explicar

- **Por que `"wx"` funciona como trava?** No Linux, `fopen(..., "wx")` vira `open(O_CREAT | O_EXCL)`. O sistema operacional verifica se o arquivo existe e o cria **numa operação só**, então dois processos nunca conseguem criar ao mesmo tempo. Um `if (existe) ... else criar` feito em C teria uma brecha entre o teste e a criação.
- **Por que gravar num temporário e usar `rename()`?** O `rename` no Linux troca o arquivo de uma vez. Se o programa cair no meio do `fwrite`, quem se estraga é o temporário, e o `fila.dat` antigo continua inteiro.
- **Por que o temporário tem o PID no nome?** Para dois processos nunca escreverem no mesmo temporário (importa na demonstração sem trava).
- **E se o programa fechar com o `fila.lock` criado?** Se sair por erro (`exit`), o `atexit` apaga a trava. Se for morto (Ctrl+C, terminal fechado), a trava fica; depois de 3 s o outro programa avisa para apagar à mão. Detectar trava abandonada automaticamente está fora do escopo.
- **Por que o filho do `fork` usa `_exit` no teste?** O filho herda uma cópia do estado do pai. Com `exit()`, o `atexit` do filho poderia apagar a trava que é do pai.

## Definition of Done

- [ ] Compila sem warnings com `gcc -Wall` (no Docker)
- [ ] `./teste_armazenamento` passa
- [ ] Revisado por alguém de **outra dupla** (sugestão: Dupla D, que vai usar estas funções na I-05)
- [ ] Merge no `main` via Pull Request
