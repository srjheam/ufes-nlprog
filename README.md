<div align="center">
  <h3 align="center">NLProg</h3>
</div>

## Sobre

NLProg foi o trabalho final desenvolvido ao longo da disciplina de Programação II, ministrada pelo professor Vinícius.

## Começando

### Pré-requisitos

- Linux (sem suporte pro windows, sra. microsoft)
- [GNU Compiler Collection](https://gcc.gnu.org/)

### Instalando

1. Clone o repositório

   ```sh
   git clone https://github.com/srjheam/NLProg.git
   ```

2. Dirija-se ao seu diretório

   ```sh
   cd NLProg/
   ```

3. Compile o código-fonte

   ```sh
   make
   ```

## Usando

Intruções de uso para os três programas.

### NLIndexa

Indexa documentos.

```sh
./nlindexa <train> <bin>
```

Onde,

- `train` - é o arquivo (train.txt) que contém os nomes dos documentos e as classes.
- `bin` - é o arquivo binário de saída com os índices.

### NLBusca

Busca por meio dos índices.

```sh
./nlbusca <indices> <knn>
```

Onde,

- `indices` - é o arquivo binário que contém os indices.
- `knn` - é o número K para o algoritmo de KNN.

### NLTesta

Testa o algoritmo de classificação de documentos.

```sh
./nltesta <indices> <test> <knn> <saida>
```

Onde,

- `indices` - é o arquivo binário que contém os indices.
- `test` - é o arquivo de teste (test.txt) que contém os nomes dos documentos e as classes.
- `knn` - é o número K para o algoritmo de KNN.
- `saida` - é o arquivo de saída dos testes.

## Contribuindo

Este projeto está configurado e integrado com o Visual Studio Code.

- Na aba "Run and Debug" selecione a configuração correspondente ao programa que deseja executar/depurar.

### Valgrind

Há uma regra no [Makefile](./Makefile) para lançar rapidamente o Valgrind. O resultado será salvo em [valgrind-out.txt](./valgrind-out.txt).

```sh
make valgrind VALPROG=<prog> VALARGS=<args>
```

Onde,

- `prog` - é o programa para executar sob o Valgrind.
- `args` - são os argumentos para passar ao programa.

## Suporte

Esse programa não funciona em ambientes não-POSIX e pode até mesmo não funcionar em sistemas não-Linux.

Isso se deve ao fato de usar APIs que são extenções do POSIX e não fazem parte do padrão C.

## Licença

Licenciado sob [Unlicense](./LICENSE).
