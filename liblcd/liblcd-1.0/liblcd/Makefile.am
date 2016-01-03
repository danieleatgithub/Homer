#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = liblcd.la

#######################################
# Build information for each library

# Sources for liblcd
liblcd_la_SOURCES = liblcd.cpp pin.cpp

# Linker options libTestProgram
liblcd_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
liblcd_la_CPPFLAGS = -I$(top_srcdir)/include

