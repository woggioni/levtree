TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    levnode.c \
    levtree.c

HEADERS += \
    levnode.h \
    levtree.h

OTHER_FILES += \
    pymodule.c\
    setup.py



