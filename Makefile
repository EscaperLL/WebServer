MAINSOUCE := TestWeb.cpp Base/LoggingTest.cpp

SOUCE := $(wildcard *.cpp Base/*.cpp)

override SOUCE :=$(filter-out $(MAINSOUCE),$(SOUCE))

OBJS :=$(patsubst *.cpp,*.o,$(SOUCE))

CC :=g++
LIBS := -lpthread
INCLUDE:= -I./usr/local/lib
CFLAGS  := -std=c++11 -g -Wall -O3 -D_PTHREADS
CXXFLAGS:= $(CFLAGS)

subTAGET1 :=TestWeb
subTAGET2 :=LoggingTest

.PHONY : objs clean veryclean rebuild all tests debug
all : $(subTAGET1) $(subTAGET2)
objs : $(OBJS)
rebuild: veryclean all

tests : $(subTAGET1) $(subTAGET2)
clean :
	find . -name '*.o' | xargs rm -f
veryclean :
	find . -name '*.o' | xargs rm -f
	find . -name $(TARGET) | xargs rm -f
	find . -name $(subTAGET1) | xargs rm -f
	find . -name $(subTAGET2) | xargs rm -f
debug:
	@echo $(SOURCE)

$(subTAGET1) : $(OBJS) TestWeb.o
	$(CC) $(CXXFLAGS) -o  $@ $^ $(LDFLAGS) $(LIBS)

$(subTAGET2) : $(OBJS) Base/LoggingTest.o
	$(CC) $(CXXFLAGS)  -o $@ $^ $(LDFLAGS) $(LIBS)
