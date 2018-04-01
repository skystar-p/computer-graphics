all:
	g++ *.cpp -lGL -lGLU -lglut -o hw1.out

run: all
	./hw1.out

clean:
	rm -f hw1.out

handin:
	tar czf	hw1.tar.gz *.cpp *.h Makefile report.pdf
