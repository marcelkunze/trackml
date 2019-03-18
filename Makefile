CC=g++
CFLAGS=-c -Wall -Wno-unused-result -std=c++11 -O3 -g -fPIC
INCLUDES=-I. -I.. -I$(shell root-config --incdir)
#(root-config --incdir)
LIBDIRS=-L/usr/lib
LIBS=-L../lib
LDFLAGS=$(shell root-config --glibs)

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

SOURCES=main.cxx
SHLIB=Tracker.cxx Reconstruction.cxx PolarModule.cxx XMLP.cxx
SHOBJ=$(SHLIB:.cxx=.o)

HEADERS=$(SRC:.cxx=.h) parallel.h Graph.h Parameters.h Point.h
OBJECTS=$(SOURCES:.cxx=.o)
EXECUTABLE=tracker
LIBRARY=libtrackml.so

all: $(LIBRARY) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) Trainer.o $(LIBRARY)
	$(CC) $(LDFLAGS) $(OBJECTS) Trainer.o $(LIBRARY) $(ROOTLIBS) $(LIBS) -o $@

$(LIBRARY): $(SHOBJ) 
	$(CC) $(INCLUDES) -std=c++11 -fPIC -shared -o $(LIBRARY) $(ROOTLIBS) $(SHOBJ)

libmodel.so: $(SHOBJ) $(HEADERS)
	g++ -DEVAL -O3 -g -std=c++11 -Wfatal-errors -fPIC -shared -pthread -lpthread -o libmodel.so $(SHOBJ) extern.cxx
	zip cloudkitchen.zip libmodel.so detectors.csv adjacency model.py metadata
	zip cloudkitchen.zip paths/* graph/* xmlp/*

eval: libmodel.so
	$(CC)  -DEVAL -Wall -std=c++11 -O3 -g -fPIC eval.cxx libmodel.so -o $@

makeGraph: makeGraph.o Trainer.o $(LIBRARY)
	$(CC) $(LDFLAGS) makeGraph.o Trainer.o $(LIBRARY) $(ROOTLIBS) $(LIBS) -o $@

%.o: %.cxx $(HEADERS)
	$(CC) -DEVAL $(CFLAGS) $< -o $@ 

Trainer.o: Trainer.cxx $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

#Reconstruction.o: Reconstruction.cxx $(HEADERS)
#	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

main.o: main.cxx $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

makeGraph.o: makeGraph.cxx $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(EXECUTABLE) Trainer.o $(LIBRARY) $(SHOBJ) $(OBJECTS) libmodel.so eval

graph: makeGraph
	-rm paths/* graph/*
	./makeGraph 21102 10 3 
	./makeGraph 21002 90
