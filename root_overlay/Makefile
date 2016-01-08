CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		root_overlay.o

LIBS =

TARGET =	root_overlay

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
