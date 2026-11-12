@echo off
rem Compila os tres programas no Windows (precisa do gcc/MinGW no PATH).
rem Uso: compilar.bat
cd /d "%~dp0"

gcc -Wall cliente.c fila.c armazenamento.c -o cliente.exe || goto erro
gcc -Wall atendente.c fila.c armazenamento.c -o atendente.exe || goto erro
gcc -Wall -I. testes\teste_fila.c fila.c -o teste_fila.exe || goto erro

echo Compilado: cliente.exe atendente.exe teste_fila.exe
exit /b 0

:erro
echo.
echo Falha na compilacao.
exit /b 1
