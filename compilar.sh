#!/bin/sh
# Compila os tres programas (Linux / dentro do Docker).
# Uso: ./compilar.sh
set -e
cd "$(dirname "$0")"

gcc -Wall cliente.c fila.c armazenamento.c -o cliente
gcc -Wall atendente.c fila.c armazenamento.c -o atendente
gcc -Wall -I. testes/teste_fila.c fila.c -o teste_fila

echo "Compilado: ./cliente ./atendente ./teste_fila"
