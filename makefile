C_FILES = $(wildcard src/*.c)

projeto2: projeto2 $(C_FILES)
	gcc -o projeto2 $(C_FILES) -lm -Wall
