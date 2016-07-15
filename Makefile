CXXFLAGS = -c -g
LDFLAGS  = -lgmpxx -lgmp
CXX = g++

RSA: RSA.o RSA_Algorithm.o
	$(CXX) -o RSA RSA.o RSA_Algorithm.o $(LDFLAGS)

RSA.o: RSA.cc
	$(CXX) $(CXXFLAGS) RSA.cc

BreakRSA: BreakRSA.o RSA_Algorithm.o
	$(CXX) -o BreakRSA BreakRSA.o RSA_Algorithm.o $(LDFLAGS)

BreakRSA.o:	BreakRSA.cc
	$(CXX) $(CXXFLAGS) BreakRSA.cc

RSA_Algorithm.o: RSA_Algorithm.cc
	$(CXX) $(CXXFLAGS) RSA_Algorithm.cc

clean:
	rm -f *.o RSA BreakRSA crack-msgs.sh
