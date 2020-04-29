ELF_FILE = hash
CPP = main.cpp
INC = inc/
HASH_ASM = $(INC)find.asm
HASH_OBJ = $(INC)find.o 

PROFILER = callgrind_out

FLAG = -O3 -masm=intel -Iinc/ -g -no-pie

all: build profiler clean

plot: build
	@./$(ELF_FILE)
	@gnuplot plot_script
	@$(MAKE) clean

build:
	nasm -f elf64 $(HASH_ASM)
	@g++ -o $(ELF_FILE) $(FLAG) $(CPP) $(HASH_OBJ)

profiler:
	@valgrind --tool=callgrind  ./$(ELF_FILE)
	@kcachegrind

clean:
	@rm out* -f
	@rm call* -f
	@rm hash -f