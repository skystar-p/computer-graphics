all:
	g++ -DGLM_FORCE_CTOR_INIT *.cpp -lpng -lGL -lGLU -lglut -o hw5.out

run: all
	./hw5.out

clean:
	rm -f hw5.out result.png

handin:
	tar czf	hw5.tar.gz *.cpp *.h Makefile report.pdf
