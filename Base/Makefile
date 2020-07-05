MAINSOURCE := LoggingTest.cpp

SOURCE := $(wildcard *.cpp )
override SOURCE := $(filter-out $(MAINSOURCE),$(SOURCE))
OBJS :=$(patsubst %.cpp,%.o,$(SOURCE))

TARGET  := LoggingTest
CC      := g++
LIBS    := -lpthread
INCLUDE:= -I./usr/local/lib
CFLAGS  := -std=c++11 -g -Wall -O3 -D_PTHREADS
CXXFLAGS:= $(CFLAGS)

SUBTARGET1 := LoggingTest

.PHONY : objs clean veryclean rebuild all tests debug
all : $(TARGET) 
objs : $(OBJS)
rebuild: veryclean all

clean :
	find . -name '*.o' | xargs rm -f
veryclean :
	find . -name $(TARGET) | xargs rm -f
	debug:
	@echo $(SOURCE)

$(TARGET) : $(OBJS) LoggingTest.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
