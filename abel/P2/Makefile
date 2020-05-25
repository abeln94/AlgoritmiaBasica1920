
#---------------------------------------------------------
# macros
RM = /bin/rm -f  # remove
CC = g++         # compiler
#---------------------------------------------------------
# vars
EJEC = nikita
#---------------------------------------------------------
# Flags
CPPFLAGS=-I. -std=c++11 -fmax-errors=1 -Ofast -pthread -ftracer -funroll-loops # opciones compilación
#---------------------------------------------------------

all: ${EJEC} #lo que se generará al ejecutar "make"
#---------------------------------------------------------
# "linkar" y generar ejecutable.
${EJEC}: ${EJEC}.cpp
	${CC} ${EJEC}.cpp -o ${EJEC} ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${EJEC}.o ${EJEC} 
