@echo off
rem Compila os programas e os testes DENTRO do Docker (o projeto usa recursos do Linux).
rem Precisa do Docker Desktop aberto.
rem Uso: compilar.bat
cd /d "%~dp0"

docker compose run --rm dev ./compilar.sh || goto erro
exit /b 0

:erro
echo.
echo Falha na compilacao. O Docker Desktop esta aberto?
exit /b 1