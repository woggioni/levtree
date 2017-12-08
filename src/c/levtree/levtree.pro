TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
TARGET = levtree

SOURCES += levnode.c \
    levtree.c \
    wlevnode.c \
    wlevtree.c \
    levtree_standing.c

HEADERS += \
    levnode.h \
    levtree.h \
    wlevnode.h \
    wlevtree.h \
    levtree_standing.h \
    common.h

OTHER_FILES += \
    setup.py\
    pylevtree.c\
    pywlevtree.c\
    python3.c\

DEFINES += LEVTREE_LIBRARY

unix {
    header.path = /usr/local/include/levtree
    target.path = /usr/local/lib
    header.files = levnode.h levtree.h wlevnode.h wlevtree.h levtree_standing.h common.h
    INSTALLS += target header
}

