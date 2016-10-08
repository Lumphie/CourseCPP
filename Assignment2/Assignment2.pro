TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CXXFLAGS += -Wpedantic -Wall -Werror -Weffc++ -Wextra

# GCC does not have pendantic flag
QMAKE_CXXFLAGS += -Wall -Werror -Weffc++ -Wextra

SOURCES += main.cpp \
    turtle.cpp

HEADERS += \
    turtle.h

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# C++11
# QMAKE_CXX = g++-5
# QMAKE_LINK = g++-5
# QMAKE_CC = gcc-5
# QMAKE_CXXFLAGS += -std=c++11
