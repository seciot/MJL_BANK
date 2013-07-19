QT += core gui thread
CONFIG += qt

TEMPLATE = app
TARGET = ..\BIN_IPS420\MJL_BANK.bin

#Debug
#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

#===================== path ===================
ObjPath         = $$PWD/..

OBJECTS_DIR     = $$ObjPath/OBJ
RCC_DIR         = $$OBJECTS_DIR
MOC_DIR         = $$OBJECTS_DIR
UI_DIR          = $$OBJECTS_DIR

AppPath         = $$ObjPath/APP/APP
GuiPath         = $$ObjPath/APP/GUI
EmvPath         = $$ObjPath/APP/EMV
RcsPath         = $$ObjPath/RCS

#==================== file ====================
SandLibFile     =   -lapp       \
                    -ljpeg              \
                    -lemv               \
                    -liconv

#==============================================
RESOURCES   +=  $$RcsPath/*.qrc
#FORMS       +=  $$GuiPath/forms/*.ui

HEADERS     +=  \
#                $$EmvPath/libinc/*.h                \
#                $$EmvPath/inc/*.h                   \
                $$GuiPath/inc/*.h                   \
                $$AppPath/inc/*.h

SOURCES     +=  \
#                $$EmvPath/src/*.c                   \
#                $$EmvPath/src/*.cpp                 \
                $$GuiPath/src/*.cpp                 \
                $$AppPath/src/*.cpp                 \
                $$AppPath/src/*.c

INCLUDEPATH +=  \
#                $$EmvPath/libinc                    \
#                $$EmvPath/inc                       \
                $$GuiPath/inc                       \
                $$AppPath/inc

DEPENDPATH  +=  \
#                $$EmvPath/libinc                    \
#                $$EmvPath/inc                       \
                $$GuiPath/inc                       \
                $$GuiPath/forms                     \
                $$AppPath/inc

LIBS        +=  \
                $$SandLibFile
