all:
	g++ --std=c++11 -DGLM_FORCE_CTOR_INIT *.cpp -pthread -lpng -o hw5.out

run: all
	./hw5.out

clean:
	rm -f hw5.out result.png

handin:
	tar czf	hw5.tar.gz *.cpp *.h Makefile report.pdf
