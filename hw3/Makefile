all:
	g++ --std=c++11 *.cpp -lGL -lGLU -lglut -o hw3.out

run: all
	./hw3.out

clean:
	rm -f hw3.out

report:
	generate-md --input=report.md --output=report --layout=github

handin:
	tar czf	hw3.tar.gz *.cpp *.h Makefile report.pdf
