CFLAGS:= -Wall 
CC:=g++

%.o: %.cpp
	$(CC) $(CFLAGS)  -o $@ -c $<

memory: memory.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $<

decoder: decoder.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $<

simulator: simulator.o memory.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $^


test: memory simulator decoder

clean:
	rm -rf *.o
