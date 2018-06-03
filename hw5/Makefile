all:
	g++ *.cpp -lGL -lGLU -lglut -o hw5.out

run: all
	./hw5.out

clean:
	rm -f hw5.out

handin:
	tar czf	hw5.tar.gz *.cpp *.h Makefile report.pdf
