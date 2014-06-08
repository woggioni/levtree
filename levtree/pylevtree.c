#include <stdio.h>
#include "levtree.h"
#include <python2.7/Python.h>
#include <python2.7/structmember.h>


typedef struct {
    PyObject_HEAD
    levtree *tree;
    PyObject *wordlist;
    /* Type-specific fields go here. */
} levtree_levtree_obj;

static int
levtree_clear(levtree_levtree_obj *self)
{
    PyObject *tmp;

    tmp = self->wordlist;
    self->wordlist = NULL;
    Py_XDECREF(tmp);
    return 0;
}

static void
levtree_dealloc(levtree_levtree_obj* self)
{
    levtree_clear(self);
    if(self->tree!=NULL)
    {
        levtree_free(self->tree);
    }
    free(self->tree);
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
levtree_python_init(levtree_levtree_obj *self, PyObject *args, PyObject *kwds)
{
    int numLines;       /* how many lines we passed for parsing */
    char** carg;        /* argument to pass to the C function*/
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
        carg[i] = PyString_AsString( strObj );
        if(PyErr_Occurred())
        {
            return -1;
        }
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
    PyObject* boolean=NULL, *dl=NULL;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive","damerau_levehnshtein", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|iOO", kwlist,
                                      &wordkey, &number_of_matches,&boolean, &dl))
    {
        return NULL;
    }
    if(number_of_matches > self->tree->entry_count) // if some idiot enters a number of results bigger than the list of words given in the constructor
    {
        number_of_matches = self->tree->entry_count;
    }
    if(boolean && PyObject_IsTrue(boolean))
    {
        if(dl && PyObject_IsTrue(dl))
        {
            tree_search_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            tree_search(self->tree, wordkey, number_of_matches);
        }
    }
    else
    {
        if(dl && PyObject_IsTrue(dl))
        {
            tree_isearch_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            tree_isearch(self->tree, wordkey, number_of_matches);
        }
    }
    levtree_result res;
    PyObject* tmp, *string;
    PyObject* list = PyList_New(number_of_matches);
    for(i=0; i<number_of_matches; i++)
    {
        res = levtree_get_result(self->tree,i);
        string = PyTuple_GetItem(self->wordlist,res.id);
        //printf("%p\t id: %u\n",string,res.id);
        tmp = Py_BuildValue("(OI)",string,res.distance);
        PyList_SetItem(list,i,tmp);
    }
    return list;
}

static PyObject *
levtree_levtree_search_id(levtree_levtree_obj* self, PyObject *args, PyObject *kwds)
{
    char* wordkey;
    index_t number_of_matches=1;
    byte_t case_sensitive=0;
    index_t i;
    PyObject* boolean=NULL, *dl=NULL;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive","damerau_levehnshtein", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|iOO", kwlist,
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
            tree_search_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            tree_search(self->tree, wordkey, number_of_matches);
        }
    }
    else
    {
        if(dl && PyObject_IsTrue(dl))
        {
            tree_isearch_dl(self->tree, wordkey, number_of_matches);
        }
        else
        {
            tree_isearch(self->tree, wordkey, number_of_matches);
        }
    }
    levtree_result res;
    PyObject* tmp;
    PyObject* list = PyList_New(number_of_matches);
    for(i=0; i<number_of_matches; i++)
    {
        res = levtree_get_result(self->tree,i);
        //printf("%p\t id: %u\n",string,res.id);
        tmp = Py_BuildValue("(II)",res.id,res.distance);
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
    {"search_id", levtree_levtree_search_id, METH_KEYWORDS, "Levenshtein tree search method returning tuple index"},
    //{"result", levtree_get_result_py, METH_VARARGS, "Levenshtein tree get result method"},
    {NULL}  /* Sentinel */
};

static PyTypeObject levtree_levtree_type =
{
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "levtree.levtree",             /*tp_name*/
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
    (inquiry)levtree_clear,		               /* tp_clear */
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
    (initproc)levtree_python_init,      /* tp_init */
    0,                         /* tp_alloc */
    levtree_new,                 /* tp_new */
};

