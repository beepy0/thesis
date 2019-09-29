CPP = g++ -O2 -Wno-deprecated
CPPDEBUG = g++ -O2 -Wno-deprecated -g


xis.o : xis.h xis.cpp gen_scheme.h
	$(CPP) -c xis.cpp

sketches.o : sketches.h sketches.cpp
	$(CPP) -c sketches.cpp


clean:
	rm -f *.out
	rm -f *.o
	rm -f *~

