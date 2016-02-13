AUTOMAKE_OPTIONS = subdir-objects
AM_LDFLAGS =  -llog4cplus -lpthread
AM_CPPFLAGS = -std=c++11 -I$(top_srcdir)/include

bin_PROGRAMS=homertool
homertool_SOURCES= homertool.cpp $(top_srcdir)/src/Winstar.cpp $(top_srcdir)/src/Display.cpp $(top_srcdir)/src/GpioPin.cpp $(top_srcdir)/src/FakeDisplay.cpp $(top_srcdir)/src/Sysinfo.cpp


