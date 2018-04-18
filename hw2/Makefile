all:
	g++ --std=c++11 *.cpp -lGL -lGLU -lglut -o hw2.out

run: all
	./hw2.out

clean:
	rm -f hw2.out

report:
	generate-md --input=report.md --output=report --layout=github

handin:
	tar czf	hw2.tar.gz *.cpp *.h Makefile report.pdf
