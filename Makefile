IDIR =../include
CXX=g++
CXXFLAGS=-I$(IDIR) -g -Wall -ftest-coverage -fprofile-arcs


ODIR=obj
LDIR =../lib


LIBS=-lm -lgtest -lpthread -lgtest_main --coverage -lgcov
_DEPS = romanToArabic.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mainTest.o romanToArabic.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

mainTest: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
