frameworks = -Ilib/raylib/src -Llib/raylib/src -lraylib  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
build_target = build/paint
src = src/paint.c src/ui.c

all: raylib.a
	gcc -o $(build_target) main.c $(frameworks)  $(src)

raylib.a:
	cd lib/raylib/src && make

run: all
	./$(build_target)
