AM_LDFLAGS =  -llog4cplus -lpthread
AM_CPPFLAGS = -std=c++11 -I$(top_srcdir)/include -I$(top_srcdir)/include/menu -I$(top_srcdir)/include/sensor

bin_PROGRAMS=homerd

homerd_SOURCES= homerd.cpp \
				HwAcquaA5.cpp \
				../src/Display.cpp \
				../src/GpioPin.cpp \
				../src/KeyButton.cpp \
				../src/KeyPanel.cpp \
				../src/MenuDisplayVisitor.cpp \
				../src/MenuLeaf.cpp \
				../src/MenuMoveVisitor.cpp \
				../src/Observer.cpp \
				../src/SubMenu.cpp \
				../src/Sysinfo.cpp \
				../src/Winstar.cpp


