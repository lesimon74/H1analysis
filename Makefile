CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -O2 $(shell root-config --cflags)
# Linker flags
LDFLAGS = $(shell root-config --libs) -lROOTNTuple
HEADERS = h1event.h

all: libH1event.so h1analysis

h1analysis: h1analysis.cxx convert.cxx
	$(CXX) -o$@ $(CXXFLAGS) $^ $(LDFLAGS) 
	

libH1event.so: MyDict.cxx
	$(CXX) -shared -fPIC -o$@ $(CXXFLAGS) $< $(LDFLAGS)

MyDict.cxx: $(HEADERS) Linkdef.h
	rootcling -f $@ $^

clean:
	rm -rf *.o libH1Event.so *.dSYM *.pcm MyDict.cxx libH1event.so h1analysis *.root .DS_Store