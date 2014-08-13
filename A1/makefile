CXXFLAGS = -Wall -g

EXEC = main

OBJS = micro.o pic32f42.o mopsr500.o r34hc22.o

.PHONY: all
all: $(EXEC)

$(EXEC): $(EXEC).cpp $(OBJS)

micro.o: micro.h
pic32f42.o: pic32f42.h micro.h
mopsr500.o: mopsr500.h micro.h
r34hc22.o: r34hc22.h micro.h

.PHONY: clean
clean:
	$(RM) -r $(OBJS) $(EXEC) $(EXEC)
