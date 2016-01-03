#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
bin_PROGRAMS=lcdtests

# Because a.out is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
#noinst_PROGRAMS=lcdtests

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

# Sources for the a.out 
lcdtests_SOURCES= lcdtests.cpp

# Linker options for a.out
#lcdtests_LDFLAGS = $(top_srcdir)/liblcd/liblcd.la /wks/buildroot-2015.08.1/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/liblog4cplus.la
lcdtests_LDFLAGS = $(top_srcdir)/liblcd/liblcd.la 

# Compiler options for a.out
lcdtests_CPPFLAGS = -I$(top_srcdir)/include


