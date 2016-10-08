TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


unix:macx{
  # Mac only, the settings for Kees
  QMAKE_CXXFLAGS += -Wpedantic -Wall -Werror -Weffc++ -Wextra
}



unix:!macx{
  # Linux only, the settings for Travis CI and Richel
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  contains(QMAKE_HOST.name,pc-157-103) {
    message("Host is student computer")
    QMAKE_CXX = g++-4.8
    QMAKE_LINK = g++-4.8
    QMAKE_CC = gcc-4.8
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
  }
  !contains(QMAKE_HOST.name,pc-157-103) {
    message("Host is not student computer")
    QMAKE_CXX = g++-5
    QMAKE_LINK = g++-5
    QMAKE_CC = gcc-5
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
  }
}

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
