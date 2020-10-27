# CWAMProlog
A C Implementation of Prolog Language Based on the Warren Abstract Machine

## Requisitos

Para execução do programa é necessário que o ambiente de execução tenha instalado os programas flex, bison, make, e o compilador gcc.

### Testado nas Seguintes Versões

O CWANProlog foi testado em dois sistemas operacionais para evitar conflitos.

#### Ambiente Linux

- **Versão Ubuntu:** Ubuntu 14.04.6 LTS
- **Versão gcc:** gcc (Ubuntu 4.8.4-2ubuntu1~14.04.4) 4.8.4
- **Versão flex:** flex 2.5.35
- **Versão bison:** bison (GNU Bison) 3.0.2

#### Ambinete macOS

- **Versão macOS:** macOS Catalina 10.15.6
- **Versão clang:** Apple clang version 11.0.3 (clang-1103.0.32.62)
- **Versão flex:** flex 2.5.35 Apple(flex-32)
- **Versão bison:** bison (GNU Bison) 3.7.2

## Instalação

Para realizar a instalação execute o seguinte comando no diretório raiz do programa:

```bash
$ make
```

Recomenda-se a execução do comando make para a instalação. Mas caso não seja possível, análise o arquivo Makefile para entender como o procedimento é instalado. Se a instalação ainda estiver complicada, copie os arquivos para um diretório em comum e o instale no local.

## Execução

Após os arquivos terem sido devidamente instalados, o programa pode ser executado direto no terminal, passando como parâmetro o nome do arquivo Prolog de entrada:

```bash
$ ./bin/main ./prologs/basic.pl
```
