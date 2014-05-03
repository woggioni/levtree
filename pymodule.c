#include <stdio.h>
#include "levtree.c"
#include <python2.7/Python.h>
#include <python2.7/structmember.h>


typedef struct {
    PyObject_HEAD
    levtree *tree;
    PyObject *wordlist;
    /* Type-specific fields go here. */
} levtree_levtree_obj;


static void
levtree_dealloc(levtree_levtree_obj* self)
{
    levtree_free(self->tree);
    free(self->tree);
    Py_DECREF(self->wordlist);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
levtree_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    levtree_levtree_obj *self;

    self = (levtree_levtree_obj *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->tree = NULL;
    }
    return (PyObject *)self;
}

static int
levtree_levtree_init(levtree_levtree_obj *self, PyObject *args, PyObject *kwds)
{
    int numLines;       /* how many lines we passed for parsing */
    char** carg;        /* argument to pass to the C function*/
    unsigned i;

    PyObject * strObj;  /* one string in the list */

    /* the O! parses for a Python object (listObj) checked
       to be of type PyList_Type */
    if (! PyArg_ParseTuple( args, "O!", &PyList_Type, &self->wordlist))
    {
        return -1;
    }
    Py_INCREF(self->wordlist);
    /* get the number of lines passed to us */
    numLines = PyList_Size(self->wordlist);
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
        strObj = PyList_GetItem(self->wordlist, i); /* Can't fail */

        /* make it a string */
        carg[i] = PyString_AsString( strObj );
    }
    self->tree = (levtree*) malloc(sizeof(levtree));
    levtree_init(self->tree,carg,numLines);
    free(carg);
    return 0;
}

static PyObject *
levtree_levtree_search(levtree_levtree_obj* self, PyObject *args, PyObject *kwds)
{
    char* wordkey;
    index_t number_of_matches=1;
    byte_t case_sensitive=0;
    index_t i;
    PyObject* boolean;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|iO", kwlist,
                                      &wordkey, &number_of_matches,&boolean))
    {
        return NULL;
    }

    if(PyObject_IsTrue(boolean))
    {
        case_sensitive=1;
    }
    if(number_of_matches > self->tree->entry_count) // if some idiot enters a number of results bigger than the list of words given in the constructor
    {
        number_of_matches = self->tree->entry_count;
    }
    self->tree->case_sensitive=case_sensitive;
    //printf("matches: %u", number_of_matches);
    levtree_search(self->tree, wordkey, number_of_matches);
    levtree_result res;
    PyObject* tmp, *string;
    PyObject* list = PyList_New(number_of_matches);
    for(i=0; i<number_of_matches; i++)
    {
        res = levtree_get_result(self->tree,i);
        string = PyList_GetItem(self->wordlist,res.id);
        //printf("%p\t id: %u\n",string,res.id);
        tmp = Py_BuildValue("(OI)",string,res.distance);
        PyList_SetItem(list,i,tmp);
    }
    return list;
}

static PyMemberDef Levtree_members[] =
{
    //    {"standing", T_OBJECT_EX, offsetof(Levtree, ), 0,
    //     "Match standing"},
    {NULL}  /* Sentinel */
};


static PyMethodDef Levtree_methods[] =
{
    {"search", levtree_levtree_search, METH_KEYWORDS, "Levenshtein tree search method"},
    //{"result", levtree_get_result_py, METH_VARARGS, "Levenshtein tree get result method"},
    {NULL}  /* Sentinel */
};

static PyTypeObject levtree_levtree_type =
{
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "levtree.Levtree",             /*tp_name*/
    sizeof(levtree_levtree_obj), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)levtree_dealloc,                         /*tp_dealloc*/
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
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Levtree_methods,             /* tp_methods */
    Levtree_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)levtree_levtree_init,      /* tp_init */
    0,                         /* tp_alloc */
    levtree_new,                 /* tp_new */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initlevtree(void)
{
    PyObject* m;
    levtree_levtree_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&levtree_levtree_type) < 0)
        return;

    m = Py_InitModule3("levtree", NULL,
                       "Example module that creates an extension type.");

    Py_INCREF(&levtree_levtree_type);
    PyModule_AddObject(m, "Levtree", (PyObject *)&levtree_levtree_type);
}
