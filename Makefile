CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -O2 $(shell root-config --cflags)
# Linker flags
LDFLAGS = $(shell root-config --libs) -lROOTNTuple
HEADERS = h1event.h

all: libH1event.so h1analysis

h1analysis: h1analysis.o convert.o
	$(CXX) -o $@ $^ $(LDFLAGS)

h1analysis.o: h1analysis.cxx convert.h libH1event.so
	$(CXX) -c $(CXXFLAGS) h1analysis.cxx

convert.o: convert.cxx convert.h libH1event.so
	$(CXX) -c $(CXXFLAGS) convert.cxx

libH1event.so: MyDict.cxx
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $< $(LDFLAGS)

MyDict.cxx: $(HEADERS) Linkdef.h
	rootcling -f $@ $^

.PHONY : clean
clean:
	rm -rf *.o libH1Event.so *.dSYM *.pcm MyDict.cxx libH1event.so h1analysis *.root *.pdf