
#---------------------------------------------------------
# macros
RM = /bin/rm -f  # remove
CC = g++         # compiler
#---------------------------------------------------------
# vars
EJEC = main
#---------------------------------------------------------
# Flags
CPPFLAGS=-I. -std=c++11 -fmax-errors=1 -O3 # opciones compilación
#---------------------------------------------------------

all: ${EJEC} #lo que se generará al ejecutar "make"
#---------------------------------------------------------
# "linkar" y generar ejecutable. Indicamos que "main_p2_e1" requiere tanto "main_p2_e1.o" como "otroFuente.o"
${EJEC}: ${EJEC}.o
	${CC} ${EJEC}.o -o ${EJEC} ${CPPFLAGS}
#---------------------------------------------------------
${EJEC}.o: ${EJEC}.cpp
	${CC} -c ${EJEC}.cpp ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${EJEC}.o ${EJEC} 
