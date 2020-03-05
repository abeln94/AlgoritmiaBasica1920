#---------------------------------------------------------
# Utilities
RM = /bin/rm -f  # remove
CPPC = g++ # C++ compiler
CC = gcc # C compiler
#---------------------------------------------------------
# Files
EJEC=mainC
EJECPP=mainCPP
RADIX=pure_radix
#---------------------------------------------------------
# Flags: see OptimizationFlags.txt
CPPFLAGS=-std=c++11 -O3 -Wall # opciones compilación
CFLAGS=-Ofast -mcpu=ultrasparc -mtune=ultrasparc -funroll-loops -Wall # opciones compilación
LDFLAGS=-flto # opciones enlace
#---------------------------------------------------------
all: ${EJEC} ${EJECPP} #lo que se generará al ejecutar "make"
c: ${EJEC} # only C main is compiled
cpp: ${EJECPP} # only C++ main is compiled
#---------------------------------------------------------
# CPP MAIN
# Linking
${EJECPP}: ${RADIX}.o ${EJECPP}.o
	${CPPC} ${CPPFLAGS} ${LDFLAGS} ${EJECPP}.o ${RADIX}.o -o ${EJECPP}
# Compilation
${EJECPP}.o: ${EJECPP}.cpp ${RADIX}.h
	${CPPC} ${CPPFLAGS} ${EJECPP}.cpp -c
#---------------------------------------------------------
# C MAIN
# Linking
${EJEC}: ${RADIX}.o ${EJEC}.o
	${CC} ${CFLAGS} ${LDFLAGS} ${EJEC}.o ${RADIX}.o -o ${EJEC}
# Compilation
${EJEC}.o: ${EJEC}.c ${RADIX}.h
	${CC} ${CFLAGS} ${EJEC}.c -c
#---------------------------------------------------------
# RADIX 
# Compilation
${RADIX}.o: ${RADIX}.c ${RADIX}.h
	${CC} ${CFLAGS} ${RADIX}.c -c
#---------------------------------------------------------
# CLEANING
clean:
	$(RM) ${EJEC}.o ${EJECPP}.o ${RADIX}.o ${EJEC} ${EJECPP}
