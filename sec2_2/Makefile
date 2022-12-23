CXX = g++
RC = windres
CFLAGS = -Wall -O3 -DUNICODE
LDFLAGS = -mwindows -mconsole
OBJS = main.o resource.o
PROGRAM = main.exe


all : $(PROGRAM)

$(PROGRAM) : $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(PROGRAM)

main.o : main.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c $<

resource.o : resource.rc
	$(RC) $< -o $@

.PHONY: clean
clean : 
	del $(OBJS) $(PROGRAM)
