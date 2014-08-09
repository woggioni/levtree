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
    Py_CLEAR(self->wordlist);
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
    self->tree = NULL;
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
    PyObject* intuple;

    /* the O! parses for a Python object (listObj) checked
       to be of type PyList_Type */
    if (!(PyArg_ParseTuple(args, "O!", &PyTuple_Type, &intuple)))
    {
        return -1;
    }
    /* get the number of lines passed to us */
    numLines = PyTuple_Size(intuple);
    carg = malloc(sizeof(char*)*numLines);

    /* should raise an error here. */
    if (numLines < 0)
    {
        return -1; /* Not a list */
    }

    self->wordlist = PyList_New(numLines);
    Py_IncRef(self->wordlist);
    for(i=0; i<numLines; i++)
    {
        strObj = PyTuple_GetItem(intuple, i);
        //PyList_Append(self->wordlist, string);
        PyList_SetItem(self->wordlist, i, strObj);
        Py_IncRef(strObj);
    }

    /* iterate over items of the list, grabbing strings, and parsing
       for numbers */
    for (i=0; i<numLines; i++)
    {

        /* grab the string object from the next element of the list */
        strObj = PyList_GetItem(self->wordlist, i); /* Can't fail */

        /* make it a string */


        if(PyUnicode_Check(strObj))
        {
            carg[i] = PyUnicode_AsUnicode( strObj );
            if(PyErr_Occurred())
            {
                return -1;
            }
        }
        else
        {
            strObj = PyUnicode_FromEncodedObject(strObj,NULL,NULL);
            if(PyErr_Occurred())
            {
                return -1;
            }
            carg[i] = PyUnicode_AsUnicode( strObj );
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
    PyObject* boolean=NULL, *dl=NULL;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive","damerau_levehnshtein", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "u|iOO", kwlist,
                                      &wordkey, &number_of_matches,&boolean, &dl))
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
    if(boolean && PyObject_IsTrue(boolean))
    {
        if(dl && PyObject_IsTrue(dl))
        {
            wtree_search_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            wtree_search(self->tree, wordkey, number_of_matches);
        }
    }
    else
    {
        if(dl && PyObject_IsTrue(dl))
        {
            wtree_isearch_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            wtree_isearch(self->tree, wordkey, number_of_matches);
        }
    }
    levtree_result res;
    PyObject* tmp, *string;
    PyObject* list = PyList_New(number_of_matches);
    for(i=0; i<number_of_matches; i++)
    {
        res = wlevtree_get_result(self->tree,i);
        string = PyList_GetItem(self->wordlist,res.id);
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
    PyObject* boolean=NULL,*dl=NULL;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive","damerau_levehnshtein", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "u|iOO", kwlist,
                                      &wordkey, &number_of_matches,&boolean,&dl))
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
    if(boolean && PyObject_IsTrue(boolean))
    {
        if(dl && PyObject_IsTrue(dl))
        {
            wtree_search_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            wtree_search(self->tree, wordkey, number_of_matches);
        }
    }
    else
    {
        if(dl && PyObject_IsTrue(dl))
        {
            wtree_isearch_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            wtree_isearch(self->tree, wordkey, number_of_matches);
        }
    }
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

static PyObject* wlevtree_levtree_add(wlevtree_wlevtree_obj* self, PyObject *args)
{
    wchar_t *cstring;
    PyObject * wordkey;
    if(!PyArg_ParseTuple(args, "O!", &PyUnicode_Type, &wordkey))
    {
        return NULL;
    }
    index_t id = PyList_Size(self->wordlist);
    PyList_Append(self->wordlist, wordkey);
    if(PyUnicode_Check(wordkey))
    {
        cstring = PyUnicode_AsUnicode(wordkey);
        if(PyErr_Occurred())
        {
            return NULL;
        }
    }
    wlevtree_add_word(self->tree, cstring, id);
    return Py_None;
}

static PyObject* wlevtree_get_wordlist(wlevtree_wlevtree_obj* self)
{
    return PyList_AsTuple(self->wordlist);
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
    {"add", wlevtree_levtree_add, METH_VARARGS, PyDoc_STR("Levenshtein tree search method returning tuple index")},
    {"wordlist", wlevtree_get_wordlist, METH_NOARGS, PyDoc_STR("Returns a tuple containing all the words contained into the underlying tree in order of insertion (and thus index)")},
    //{"result", levtree_get_result_py, METH_VARARGS, "Levenshtein tree get result method"},
    {NULL}  /* Sentinel */
};

static PyTypeObject wlevtree_wlevtree_type =
{
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "levtree.wlevtree",             /*tp_name*/
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
    PyModule_AddObject(m, "levtree", (PyObject *)&levtree_levtree_type);
    Py_INCREF(&wlevtree_wlevtree_type);
    PyModule_AddObject(m, "wlevtree", (PyObject *)&wlevtree_wlevtree_type);
}

