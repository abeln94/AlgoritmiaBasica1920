#---------------------------------------------------------
# macros
RM = /bin/rm -f  # remove
CPPC = g++ # C++ compiler
CC = gcc # C compiler
#---------------------------------------------------------
# vars
EJEC=mainC
EJECPP=mainCPP
RADIX=pure_radix
#---------------------------------------------------------
# Flags: see OptimizationFlags.txt
CPPFLAGS= -I. -std=c++11 -fmax-errors=1 -O3 -Wall # opciones compilación
CFLAGS= -std=c99 -O3 -Wall # opciones compilación
#---------------------------------------------------------
all: ${EJEC} ${EJECPP} #lo que se generará al ejecutar "make"
c: ${EJEC} # only C main is compiled
cpp: ${EJECPP} # only C++ main is compiled
lib: ${RADIX}.o # only lib is compiled (not linked, it hasn't where)
#---------------------------------------------------------
# mainCPP linking
${EJECPP}: ${RADIX}.o ${EJECPP}.o
	${CPPC} ${EJECPP}.o ${RADIX}.o -o ${EJECPP} ${CPPFLAGS}
#---------------------------------------------------------
# mainCPP compilation
${EJECPP}.o: ${EJECPP}.cpp ${RADIX}.h
	${CPPC} -c ${EJECPP}.cpp ${CPPFLAGS}
#---------------------------------------------------------
# mainC linking
${EJEC}: ${RADIX}.o ${EJEC}.o
	${CC} ${EJEC}.o ${RADIX}.o -o ${EJEC} ${CFLAGS}
#---------------------------------------------------------
# mainC compilation
${EJEC}.o: ${EJEC}.c ${RADIX}.h
	${CC} -c ${EJEC}.c ${CFLAGS}
#---------------------------------------------------------
# radix compilation
${RADIX}.o: ${RADIX}.c ${RADIX}.h
	${CC} -c ${RADIX}.c ${CFLAGS}
	
#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${EJEC}.o ${EJECPP}.o ${RADIX}.o ${EJEC} ${EJECPP}
