# Makefile que percorre todo a biblioteca, procurando todos os códigos fontes .c para compilar

# INCLUDE        - é o diretório que contém os headers
# BUILDIR        - é o diretório onde estarão os binários
# LIBCOMMONDIR   - é o diretório para a biblioteca de tipos comuns.
# LIBCOMMON      - é a biblioteca de tipos comuns.
# LIBENGINEDIR   - é o diretório para a biblioteca de busca e indexação.
# LIBENGINE      - é a biblioteca de busca e indexação.
# NLINDEXA       - é o programa de indexação.
# NLBUSCA        - é o programa de busca.
# NLTESTA        - é o programa de teste.
INCLUDE          = include
LIBCOMMONDIR     = common
LIBCOMMONINCLUDE = $(LIBCOMMONDIR)/include
LIBCOMMON        = common
LIBENGINEDIR     = engine
LIBENGINEINCLUDE = $(LIBENGINEDIR)/include
LIBENGINE        = engine
NLINDEXA         = nlindexa
NLBUSCA          = nlbusca
NLTESTA          = nltesta
VALGRINDOUT      = valgrind-out.txt

# CCompiler - é o compilador usado
# CFLAGS    - são as flags dadas ao compilador durante a compilação
CC = gcc
override CFLAGS += -Wall -Wall -Wextra -Werror -pedantic -ggdb3 -I$(LIBCOMMONINCLUDE) -I$(LIBENGINEINCLUDE) -lm

# Make - é o GNU Make
MAKE = make

all : pre-build build

# Compila o executável e linka as bibliotecas
nlindexa: nlindexa.c
	$(CC) -o $@ $< -L $(LIBCOMMONDIR)/ -l$(LIBCOMMON) -L $(LIBENGINEDIR)/ -l$(LIBENGINE) $(CFLAGS)

nlbusca: nlbusca.c
	$(CC) -o $@ $< -L $(LIBCOMMONDIR)/ -l$(LIBCOMMON) -L $(LIBENGINEDIR)/ -l$(LIBENGINE) $(CFLAGS)

nltesta: nltesta.c
	$(CC) -o $@ $< -L $(LIBCOMMONDIR)/ -l$(LIBCOMMON) -L $(LIBENGINEDIR)/ -l$(LIBENGINE) $(CFLAGS)

# Chama os submakefiles garantindo a compilação das bibliotecas
pre-build:
	$(MAKE) -C $(LIBCOMMONDIR)/
	$(MAKE) -C $(LIBENGINEDIR)/ 

build : nlindexa nlbusca nltesta

# Rode make clean para remover tudo que é gerado pela build
clean:
	rm -f nlindexa nlbusca nltesta $(VALGRINDOUT)
	$(MAKE) -C $(LIBCOMMONDIR)/ clean
	$(MAKE) -C $(LIBENGINEDIR)/ clean

valgrind: all
	rm -f $(VALGRINDOUT)

	valgrind --leak-check=full \
			 --show-leak-kinds=all \
			 --track-origins=yes \
			 --verbose \
			 --log-file=$(VALGRINDOUT) \
			 ./$(VALPROG) $(VALARGS)

	@echo "Head to $(VALGRINDOUT)"
