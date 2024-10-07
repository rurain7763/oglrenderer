PROJECT = renderer
CC = g++
LANG_STD = -std=c++17
FLAGS = -Wall -Wfatal-errors -x c++
INCLUDE_PATH = -I/opt/homebrew/include -I./vendor/
SRC_FILES = ./src/*.cpp \
			./src/Engine/*.cpp \
			./src/Logger/*.cpp \
			./src/Shader/*.cpp \
			./src/VertexArray/*.cpp \
			./src/VertexBuffer/*.cpp \
			./src/Matrix/*cpp \
			./src/IndexBuffer/*.cpp \
			./src/Camera/*.cpp \
			./src/Texture/*.cpp \
			./vendor/upng/*.c
LIB_PATH = -L/opt/homebrew/lib
LIB = -lSDL2 -lglew -lglut -lglm -framework OpenGL

build:
	${CC} ${FLAGS} ${LANG_STD} ${INCLUDE_PATH} ${SRC_FILES} ${LIB_PATH} ${LIB} -o ${PROJECT}.exe

run:
	./${PROJECT}.exe