TEMPLATE = app
CONFIG += debug
QT = gui core
SOURCES += src/main.cpp \
 src/FileLister.cpp \
 src/MainDialog.cpp \
 src/Settings.cpp \
 src/OptionsDialog.cpp
HEADERS += src/FileLister.h src/MainDialog.h src/Settings.h src/OptionsDialog.h
INCLUDEPATH = ./src
