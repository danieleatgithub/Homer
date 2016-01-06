#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libdpy.la

#######################################
# Build information for each library

# Sources for libdpy
libdpy_la_SOURCES = libdpy.cpp pin.cpp

# Linker options libTestProgram
libdpy_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libdpy_la_CPPFLAGS = -I$(top_srcdir)/include

