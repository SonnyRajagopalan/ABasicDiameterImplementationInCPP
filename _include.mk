PROJECT_HOME ?= $(CURDIR)
export PROJECT_HOME

LOCAL_INCLUDE_DIR ?= /tmp/local_include

CPP = g++
CPPFLAGS = -Wall -O3 -g
LIB = -L$(LOCAL_INCLUDE_DIR) -luuid -lpthread -lrt
INCLUDE = -I $(PROJECT_HOME)/include -I $(LOCAL_INCLUDE_DIR)
MAKEDEPEND = g++ -MM $(INCLUDE) -o $*.depends $<
BUILDID = `date +%Y%m%d-%H%M%S`
DIRS += avps
DIRS += base
DIRS += common
DIRS += logging
DIRS += messages
DIRS += net
DIRS += platform
DIRS += test
