all:
	g++ --std=c++11 *.cpp -lGL -lGLU -lglut -o hw4.out

run: all
	./hw4.out

clean:
	rm -f hw4.out

report:
	generate-md --input=report.md --output=report --layout=github

handin:
	tar czf	hw4.tar.gz *.cpp *.h Makefile report.pdf chess.in
