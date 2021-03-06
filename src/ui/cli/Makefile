# Baby Makefile for Linux
#

SRC         = main.cpp search.cpp argutils.cpp searchaction.cpp strutils.cpp \
			  safeutils.cpp diff.cpp impexp.cpp

TESTSRC         = add-entry-test.cpp arg-fields-test.cpp split-test.cpp \
				  safeutils.cpp argutils.cpp searchaction.cpp strutils.cpp \
				  trim-test.cpp search-test.cpp search.cpp

COREPATH        = ../../core
OSPATH          = ../../os
LIBPATH         = ../../../lib/unicodedebug
OBJPATH         = ../../../obj/unicodedebug/cli
BINPATH         = .

#destination related macros
OBJ	 = $(addprefix $(OBJPATH)/,$(subst .cpp,.o,$(SRC)))
EXE	    = $(BINPATH)/pwsafe-cli

TESTOBJ	 = $(addprefix $(OBJPATH)/,$(subst .cpp,.o,$(TESTSRC)))
TESTEXE	= $(BINPATH)/pwsafe-cli-test

LIBCORE	= $(LIBPATH)/libcore.a
LIBOS  	= $(LIBPATH)/libos.a
LIBS    = $(LIBCORE) $(LIBOS)

export CPPFLAGS += -std=c++11
ifeq ($(findstring Darwin, $(shell uname -s)), Darwin)
	XERCESCPPFLAGS=
	PLATFORM=mac
	EXTLIBS=-framework gtest -framework CoreFoundation -framework CoreGraphics
else
	XERCESCPPFLAGS=-DUSE_XML_LIBRARY=XERCES -DWCHAR_INCOMPATIBLE_XMLCH
	PLATFORM=unix
	GTEST_DIR=/usr/src/gtest
	GTEST_HEADERS=/usr/include/gtest/*.h /usr/include/gtest/internal/*.h
	GTEST_SRCS=$(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
	GTEST_OBJ = $(OBJPATH)/gtest-all.o
	EXTLIBS=-luuid -lxerces-c -lpthread -lXtst -lX11
endif

CXXFLAGS += $(CPPFLAGS) -Wall -I$(COREPATH) -I../.. $(XERCESCPPFLAGS) -DUNICODE

# rules
.PHONY: all clean deepclean setup

$(OBJPATH)/%.o : %.c
	$(CC) -g  $(CFLAGS)   -c $< -o $@

$(OBJPATH)/%.o : %.cpp
	$(CXX) -g $(CXXFLAGS) -c $< -o $@

all : setup $(EXE) $(TESTEXE)


$(LIBCORE): 
	$(MAKE) -C $(COREPATH)

$(LIBOS): 
	$(MAKE) -C $(OSPATH)/$(PLATFORM)

$(EXE): $(LIBS) $(OBJ)
	$(CXX) -g $(CXXFLAGS) $(filter %.o,$^)  -o $@ $(LD_FLAGS) \
	-L$(LIBPATH) -lcore -los -lcore $(EXTLIBS)

clean:
	rm -f *~ $(OBJ) $(EXE) $(LIBS) $(DEPENDFILE)

deepclean: clean
	$(MAKE) -C $(COREPATH) clean

setup:
	@mkdir -p $(OBJPATH) $(BINPATH)

test: $(GTEST_OBJ) $(TESTEXE)

$(GTEST_OBJ): $(GTEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) -g $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c -o $@ \
	$(GTEST_DIR)/src/gtest-all.cc

$(TESTEXE): $(LIBS) $(TESTOBJ)
	$(CXX) -v -g $(CXXFLAGS) $(filter %.o,$^)  -o $@  $(LD_FLAGS) \
	-L$(LIBPATH) -lcore -los -lcore $(EXTLIBS) $(GTEST_OBJ)
