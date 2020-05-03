ELF_FILE = hash
CPP = main.cpp
INC = inc/
HASH_ASM = $(INC)find.asm
HASH_OBJ = $(INC)find.o 

PROFILER = false

OPTIMIZE = -O0
FLAG = $(OPTIMIZE) -masm=intel -Iinc/ -no-pie
TYPE = "default"

all: build profiler clean

plot: build
	@./$(ELF_FILE) plot
	@gnuplot plot_script
	@$(MAKE) clean

run: build
	@/usr/bin/time -f "\n%E real\n%U user\n%S sys" ./$(ELF_FILE) $(TYPE)
	@$(MAKE) profiler TYPE=$(TYPE)
	@$(MAKE) clean
build:
	@nasm -f elf64 $(HASH_ASM) -o $(HASH_OBJ)
	@g++ -o $(ELF_FILE) $(FLAG) $(CPP) $(HASH_OBJ)

profiler: 
	@valgrind --tool=callgrind  ./$(ELF_FILE) $(TYPE)
	@kcachegrind

clean:
	@rm out* -f
	@rm call* -f
	@rm hash -f