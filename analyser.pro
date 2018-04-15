TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ./src/main.cpp \
    ./src/lexseq.cpp \
    ./src/errors.cpp \
    ./src/synanalys.cpp \
    ./src/analysistree.cpp \
    ./src/typedetector.cpp

INCLUDEPATH = ./inc/

HEADERS += \
    ./inc/lexseq.h \
    ./inc/iterated.h \
    ./inc/errors.h \
    ./inc/synanalys.h \
    ./inc/analysistree.h \
    ./inc/typedetector.h
QMAKE_CXXFLAGS += -std=c++1z \
    -Wall\
