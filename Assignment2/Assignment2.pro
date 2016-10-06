TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Wpedantic -Wall -Werror -Weffc++ -Wextra

SOURCES += main.cpp \
    turtle.cpp

HEADERS += \
    turtle.h
