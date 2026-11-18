#!/bin/sh
# Compila os programas e os testes (Linux / dentro do Docker).
# Uso: ./compilar.sh
set -e
cd "$(dirname "$0")"

gcc -Wall cliente.c fila.c armazenamento.c -o cliente
gcc -Wall atendente.c fila.c armazenamento.c -o atendente
gcc -Wall -I. testes/teste_fila.c fila.c -o teste_fila
gcc -Wall -I. testes/teste_armazenamento.c fila.c armazenamento.c -o teste_armazenamento

echo "Compilado: ./cliente ./atendente ./teste_fila ./teste_armazenamento"
