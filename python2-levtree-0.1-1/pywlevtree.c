#include <stdio.h>
#include "pylevtree.c"
#include "wlevtree.h"
//#include <python2.7/Python.h>
//#include <python2.7/structmember.h>


typedef struct {
    PyObject_HEAD
    wlevtree *tree;
    PyObject *wordlist;
    /* Type-specific fields go here. */
} wlevtree_wlevtree_obj;

static int
wlevtree_clear(wlevtree_wlevtree_obj *self)
{
    PyObject *tmp;

    tmp = self->wordlist;
    self->wordlist = NULL;
    Py_XDECREF(tmp);
    return 0;
}

static void
wlevtree_dealloc(wlevtree_wlevtree_obj* self)
{
    wlevtree_clear(self);
    if(self->tree!=NULL)
    {
        wlevtree_free(self->tree);
    }
    free(self->tree);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
wlevtree_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    wlevtree_wlevtree_obj *self;

    self = (wlevtree_wlevtree_obj *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->tree = NULL;
    }
    return (PyObject *)self;
}

static int
wlevtree_python_init(wlevtree_wlevtree_obj *self, PyObject *args, PyObject *kwds)
{
    int numLines;       /* how many lines we passed for parsing */
    wchar_t** carg;        /* argument to pass to the C function*/
    unsigned i;

    PyObject * strObj;  /* one string in the list */

    /* the O! parses for a Python object (listObj) checked
       to be of type PyList_Type */
    int res;
    if (!(res=PyArg_ParseTuple( args, "O!", &PyTuple_Type, &self->wordlist)))
    {
        return -1;
    }
    Py_INCREF(self->wordlist);
    /* get the number of lines passed to us */
    numLines = PyTuple_Size(self->wordlist);
    carg = malloc(sizeof(char*)*numLines);

    /* should raise an error here. */
    if (numLines < 0)
    {
        return -1; /* Not a list */
    }


    /* iterate over items of the list, grabbing strings, and parsing
       for numbers */
    for (i=0; i<numLines; i++)
    {

        /* grab the string object from the next element of the list */
        strObj = PyTuple_GetItem(self->wordlist, i); /* Can't fail */

        /* make it a string */
        carg[i] = PyUnicode_AsUnicode( strObj );
        if(PyErr_Occurred())
        {
            return -1;
        }
    }
    self->tree = (wlevtree*) malloc(sizeof(wlevtree));
    wlevtree_init(self->tree,carg,numLines);
    free(carg);
    return 0;
}

static PyObject *
wlevtree_levtree_search(wlevtree_wlevtree_obj* self, PyObject *args, PyObject *kwds)
{
    wchar_t* wordkey;
    index_t number_of_matches=1;
    byte_t case_sensitive=0;
    index_t i;
    PyObject* boolean=NULL;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "u|iO", kwlist,
                                      &wordkey, &number_of_matches,&boolean))
    {
        return NULL;
    }

    if(boolean && PyObject_IsTrue(boolean))
    {
        case_sensitive=1;
    }
    if(number_of_matches > self->tree->entry_count) // if some idiot enters a number of results bigger than the list of words given in the constructor
    {
        number_of_matches = self->tree->entry_count;
    }
    self->tree->case_sensitive=case_sensitive;
    //printf("matches: %u", number_of_matches);
    wlevtree_search(self->tree, wordkey, number_of_matches);
    levtree_result res;
    PyObject* tmp, *string;
    PyObject* list = PyList_New(number_of_matches);
    for(i=0; i<number_of_matches; i++)
    {
        res = wlevtree_get_result(self->tree,i);
        string = PyTuple_GetItem(self->wordlist,res.id);
        //printf("%p\t id: %u\n",string,res.id);
        tmp = Py_BuildValue("(OI)",string,res.distance);
        PyList_SetItem(list,i,tmp);
    }
    return list;
}

static PyObject *
wlevtree_levtree_search_id(wlevtree_wlevtree_obj* self, PyObject *args, PyObject *kwds)
{
    wchar_t* wordkey;
    index_t number_of_matches=1;
    byte_t case_sensitive=0;
    index_t i;
    PyObject* boolean=NULL;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "u|iO", kwlist,
                                      &wordkey, &number_of_matches,&boolean))
    {
        return NULL;
    }

    if(boolean && PyObject_IsTrue(boolean))
    {
        case_sensitive=1;
    }
    if(number_of_matches > self->tree->entry_count) // if some idiot enters a number of results bigger than the list of words given in the constructor
    {
        number_of_matches = self->tree->entry_count;
    }
    self->tree->case_sensitive=case_sensitive;
    //printf("matches: %u", number_of_matches);
    wlevtree_search(self->tree, wordkey, number_of_matches);
    levtree_result res;
    PyObject* tmp;
    PyObject* list = PyList_New(number_of_matches);
    for(i=0; i<number_of_matches; i++)
    {
        res = wlevtree_get_result(self->tree,i);
        //printf("%p\t id: %u\n",string,res.id);
        tmp = Py_BuildValue("(II)",res.id,res.distance);
        PyList_SetItem(list,i,tmp);
    }
    return list;
}

static PyMemberDef Wlevtree_members[] =
{
    //    {"standing", T_OBJECT_EX, offsetof(Levtree, ), 0,
    //     "Match standing"},
    {NULL}  /* Sentinel */
};


static PyMethodDef Wlevtree_methods[] =
{
    {"search", wlevtree_levtree_search, METH_KEYWORDS, "Levenshtein tree search method"},
    {"search_id", wlevtree_levtree_search_id, METH_KEYWORDS, "Levenshtein tree search method returning tuple index"},
    //{"result", levtree_get_result_py, METH_VARARGS, "Levenshtein tree get result method"},
    {NULL}  /* Sentinel */
};

static PyTypeObject wlevtree_wlevtree_type =
{
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "levtree.Levtree",             /*tp_name*/
    sizeof(wlevtree_wlevtree_obj), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)wlevtree_dealloc,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Levensthein distance tree",           /* tp_doc */
    0,		               /* tp_traverse */
    (inquiry)wlevtree_clear,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Wlevtree_methods,             /* tp_methods */
    Wlevtree_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)wlevtree_python_init,      /* tp_init */
    0,                         /* tp_alloc */
    wlevtree_new,                 /* tp_new */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initlevtree(void)
{
    PyObject* m;
    //levtree_levtree_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&levtree_levtree_type) < 0 || PyType_Ready(&wlevtree_wlevtree_type)<0 )
        return;

    m = Py_InitModule3("levtree", NULL,
                       "Example module that creates an extension type.");

    Py_INCREF(&levtree_levtree_type);
    PyModule_AddObject(m, "Levtree", (PyObject *)&levtree_levtree_type);
    Py_INCREF(&wlevtree_wlevtree_type);
    PyModule_AddObject(m, "Wlevtree", (PyObject *)&wlevtree_wlevtree_type);
}

