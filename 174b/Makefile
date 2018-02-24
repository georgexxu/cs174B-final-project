CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		174b.o

LIBS =

TARGET =	174b

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
