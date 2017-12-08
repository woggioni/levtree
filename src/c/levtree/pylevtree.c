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
    Py_CLEAR(self->wordlist);
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
    self->tree = NULL;
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

static PyObject* pylevtree_set_case_sensitive(levtree_levtree_obj* self, PyObject *args)
{
    PyObject* boolean;
    if (!PyArg_ParseTuple(args, "O", &boolean))
        return NULL;

    if(PyObject_IsTrue(boolean))
    {
        levtree_set_case_sensitive(self->tree,1);
    }
    else
    {
        levtree_set_case_sensitive(self->tree,0);
    }
    return Py_None;
}

static PyObject* pylevtree_set_algorithm(levtree_levtree_obj* self, PyObject *args)
{
    int algo;
    if (!PyArg_ParseTuple(args, "i", &algo))
    {
        return NULL;
    }
    switch(algo)
    {
    case LEVENSHTEIN:
        levtree_set_algorithm(self->tree,LEVENSHTEIN);
        break;
    case DAMERAU_LEVENSHTEIN:
        levtree_set_algorithm(self->tree, DAMERAU_LEVENSHTEIN);
        break;
    }
    return Py_None;
}

static index_t pylevtree_search_template(levtree_levtree_obj* self, PyObject *args, PyObject *kwds)
{
    char* wordkey;
    index_t number_of_matches=1;
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
    if(dl)
    {
        if(PyObject_IsTrue(dl))
        {
            levtree_set_algorithm(self->tree, DAMERAU_LEVENSHTEIN);
        }
        else
        {
             levtree_set_algorithm(self->tree, LEVENSHTEIN);
        }
    }
    if(boolean)
    {
        if(PyObject_IsTrue(boolean))
        {
            levtree_set_case_sensitive(self->tree,1);
        }
        else
        {
            levtree_set_case_sensitive(self->tree,0);
        }
    }
    levtree_search(self->tree, wordkey, number_of_matches);
    return number_of_matches;
}

static PyObject *
levtree_levtree_search(levtree_levtree_obj* self, PyObject *args, PyObject *kwds)
{
    index_t number_of_matches = pylevtree_search_template(self, args, kwds);
    levtree_result res;
    PyObject* tmp, *string;
    PyObject* list = PyList_New(number_of_matches);
    index_t i;
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

static PyObject *
levtree_levtree_search_id(levtree_levtree_obj* self, PyObject *args, PyObject *kwds)
{
    index_t number_of_matches = pylevtree_search_template(self, args, kwds);
    levtree_result res;
    PyObject* tmp;
    PyObject* list = PyList_New(number_of_matches);
    index_t i;
    for(i=0; i<number_of_matches; i++)
    {
        res = levtree_get_result(self->tree,i);
        //printf("%p\t id: %u\n",string,res.id);
        tmp = Py_BuildValue("(II)",res.id,res.distance);
        PyList_SetItem(list,i,tmp);
    }
    return list;
}

static PyObject* levtree_levtree_add(levtree_levtree_obj* self, PyObject *args)
{
    wchar_t *cstring;
    PyObject * wordkey;
    if(!PyArg_ParseTuple(args, "O!", &PyString_Type, &wordkey))
    {
        return NULL;
    }
    cstring = PyString_AsString(wordkey);
    if(PyErr_Occurred())
    {
        return NULL;
    }
    index_t id = PyList_Size(self->wordlist);
    PyList_Append(self->wordlist, wordkey);
    levtree_add_word(self->tree, cstring, id);
    return Py_None;
}

static PyObject* levtree_get_wordlist(levtree_levtree_obj* self)
{
    return PyList_AsTuple(self->wordlist);
}

static PyMemberDef Levtree_members[] =
{
    //    {"standing", T_OBJECT_EX, offsetof(Levtree, ), 0,
    //     "Match standing"},
    {NULL}  /* Sentinel */
};


static PyMethodDef Levtree_methods[] =
{
    {"setCaseSensitive", pylevtree_set_case_sensitive, METH_VARARGS, PyDoc_STR("Set case sensityvity of the search method of the tree instance")},
    {"setAlgorithm", pylevtree_set_algorithm, METH_VARARGS, PyDoc_STR("Set the algorithm for string distance calculation.\n\
     You can choose between Levtree.LEVENSHTEIN and Levtree.DAMERAU_LEVENSHTEIN")},
    {"search", levtree_levtree_search, METH_KEYWORDS, "Levenshtein tree search method"},
    {"searchID", levtree_levtree_search_id, METH_KEYWORDS, "Levenshtein tree search method returning tuple index"},
    {"add", levtree_levtree_add, METH_VARARGS, PyDoc_STR("Add a string to the underlying tree structured dictionary")},
    {"wordlist", levtree_get_wordlist, METH_NOARGS, PyDoc_STR("Returns a tuple containing all the words contained into the underlying tree in order of insertion (and thus index)")},
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

