#include <stdio.h>
#include "wlevtree.h"
#include <python3.4m/Python.h>
#include <python3.4m/structmember.h>
#include <locale.h>

static PyObject* PYLEVENSHTEIN;
static PyObject* PYDAMERAU_LEVENSHTEIN;

typedef struct {
    PyObject_HEAD
    wlevtree *tree;
    PyObject *wordlist;
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

    /*
    PyObject *strObj, *tmp;
    unsigned i;
    int size = PyList_Size(self->wordlist);
    printf("%d\n",size);
    fflush(stdout);
    for(i=0; i<size; i++)
    {
        strObj = PyList_GetItem(self->wordlist, i);
        Py_DECREF(strObj);
    }*/
    Py_TYPE(self)->tp_free((PyObject *)self);
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

static PyObject* pywlevtree_set_case_sensitive(wlevtree_wlevtree_obj* self, PyObject *args)
{
    PyObject* boolean;
    if (!PyArg_ParseTuple(args, "O", &boolean))
        return NULL;

    if(PyObject_IsTrue(boolean))
    {
        wlevtree_set_case_sensitive(self->tree,1);
    }
    else
    {
        wlevtree_set_case_sensitive(self->tree,0);
    }
    return Py_None;
}

static PyObject* pywlevtree_set_algorithm(wlevtree_wlevtree_obj* self, PyObject *args)
{
    int algo;
    if (!PyArg_ParseTuple(args, "i", &algo))
    {
        return NULL;
    }
    switch(algo)
    {
    case LEVENSHTEIN:
        wlevtree_set_algorithm(self->tree,LEVENSHTEIN);
        break;
    case DAMERAU_LEVENSHTEIN:
        wlevtree_set_algorithm(self->tree, DAMERAU_LEVENSHTEIN);
        break;
    }
    return Py_None;
}

static index_t pylevtree_search_template(wlevtree_wlevtree_obj* self, PyObject *args, PyObject *kwds)
{
    wchar_t* wordkey;
    index_t number_of_matches=1;
    PyObject* boolean=NULL, *dl=NULL;
    static char *kwlist[] = {"wordkey","number_of_matches","case_sensitive","damerau_levehnshtein", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "u|iOO", kwlist,
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
            wlevtree_set_algorithm(self->tree, DAMERAU_LEVENSHTEIN);
        }
        else
        {
             wlevtree_set_algorithm(self->tree, LEVENSHTEIN);
        }
    }
    if(boolean)
    {
        if(PyObject_IsTrue(boolean))
        {
            wlevtree_set_case_sensitive(self->tree,1);
        }
        else
        {
            wlevtree_set_case_sensitive(self->tree,0);
        }
    }
    wlevtree_search(self->tree, wordkey, number_of_matches);
    return number_of_matches;
}

static PyObject *
wlevtree_levtree_search(wlevtree_wlevtree_obj* self, PyObject *args, PyObject *kwds)
{
    index_t number_of_matches = pylevtree_search_template(self, args, kwds);
    levtree_result res;
    PyObject* tmp, *string;
    PyObject* list = PyList_New(number_of_matches);
    index_t i;
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
    index_t number_of_matches = pylevtree_search_template(self, args, kwds);
    levtree_result res;static PyObject* PYLEVENSHTEIN;
    static PyObject* PYDAMERAU_LEVENSHTEIN;
    PyObject* tmp;
    PyObject* list = PyList_New(number_of_matches);
    index_t i;
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
    PyObject * wordkey;
    if(!PyArg_ParseTuple(args, "O!", &PyUnicode_Type, &wordkey))
    {
        return NULL;
    }
    index_t id = PyList_Size(self->wordlist);
    PyList_Append(self->wordlist, wordkey);
    if(PyUnicode_Check(wordkey))
    {
        wchar_t *cstring = PyUnicode_AsUnicode(wordkey);
        if(PyErr_Occurred())
        {
            return NULL;
        }
        wlevtree_add_word(self->tree, cstring, id);
    }

    return Py_None;
}

static PyObject* wlevtree_get_wordlist(wlevtree_wlevtree_obj* self)
{
    return PyList_AsTuple(self->wordlist);
}

static PyMemberDef Wlevtree_members[] =
{
        {"wordlist", T_OBJECT_EX, offsetof(wlevtree_wlevtree_obj, wordlist), 0,
         "Words dictionary"},
    {NULL}  /* Sentinel */
};


static PyMethodDef Wlevtree_methods[] =
{
    {"setCaseSensitive", pywlevtree_set_case_sensitive, METH_VARARGS, PyDoc_STR("Set case sensityvity of the search method of the tree instance")},
    {"setAlgorithm", pywlevtree_set_algorithm, METH_VARARGS, PyDoc_STR("Set the algorithm for string distance calculation.\n\
     You can choose between Levtree.LEVENSHTEIN and Levtree.DAMERAU_LEVENSHTEIN")},
    {"search", wlevtree_levtree_search, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Levenshtein tree search method")},
    {"searchID", wlevtree_levtree_search_id, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Levenshtein tree search method returning tuple index")},
    {"add", wlevtree_levtree_add, METH_VARARGS, PyDoc_STR("Levenshtein tree search method returning tuple index")},
    {"wordlist", wlevtree_get_wordlist, METH_NOARGS, PyDoc_STR("Returns a tuple containing all the words contained into the underlying tree in order of insertion (and thus index)")},
    //{"result", levtree_get_result_py, METH_VARARGS, PyDoc_STR("Levenshtein tree get result method")},
    {NULL}  /* Sentinel */
};


static PyTypeObject wlevtree_wlevtree_type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "levtree.Levtree",                              /* tp_name */
        sizeof(wlevtree_wlevtree_obj),                        /* tp_basicsize */
        0,                                              /* tp_itemsize */
        (destructor)wlevtree_dealloc,                   /* tp_dealloc */
        0,                                              /* tp_print */
        0,                                              /* tp_getattr */
        0,                                              /* tp_setattr */
        0,                                              /* tp_reserved */
        0,                                              /* tp_repr */
        0,                                              /* tp_as_number */
        0,                                              /* tp_as_sequence */
        0,                                              /* tp_as_mapping */
        0,                                              /* tp_hash */
        0,                                              /* tp_call */
        0,                                              /* tp_str */
        0,                                              /* tp_getattro */
        0,                                              /* tp_setattro */
        0,                                              /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,                             /* tp_flags */
        "Levensthein distance tree",                    /* tp_doc */
        0,                                              /* tp_traverse */
        0,                                              /* tp_clear */
        0,                                              /* tp_richcompare */
        0,                                              /* tp_weaklistoffset */
        0,                                              /* tp_iter */
        0,                                              /* tp_iternext */
        Wlevtree_methods,                               /* tp_methods */
        0,                                              /* tp_members */
        0,                                              /* tp_getset */
        0,                                              /* tp_base */
        0,                                              /* tp_dict */
        0,                                              /* tp_descr_get */
        0,                                              /* tp_descr_set */
        0,                                              /* tp_dictoffset */
        (initproc)wlevtree_python_init,                 /* tp_init */
        0,                                              /* tp_alloc */
        wlevtree_new,                                   /* tp_new */
        0                                               /* tp_free */
};

static struct PyModuleDef levtree_module = {
        PyModuleDef_HEAD_INIT,
        "levtree",
        "Module for fast fuzzy-string searches",
        -1,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

extern int wlevtree_setup_types(void)
{
    wlevtree_wlevtree_type.tp_new = PyType_GenericNew;
    return PyType_Ready(&wlevtree_wlevtree_type);
}

PyMODINIT_FUNC
PyInit_levtree(void)
{
    PyObject* m;
    PyObject *dict;

    m = PyModule_Create(&levtree_module);
    if (m == NULL || wlevtree_setup_types()<0)
            return NULL;
    Py_INCREF(&wlevtree_wlevtree_type);
    if(PyModule_AddObject(m,"Levtree",(PyObject *)&wlevtree_wlevtree_type))
    {
        return NULL;
    }

    if (!(dict = PyModule_GetDict(m)))
    {
            goto error;
    }

    PYLEVENSHTEIN = Py_BuildValue("i",LEVENSHTEIN);
    PYDAMERAU_LEVENSHTEIN = Py_BuildValue("i",DAMERAU_LEVENSHTEIN);
    PyDict_SetItemString(dict, "LEVENSHTEIN", PYLEVENSHTEIN);
    PyDict_SetItemString(dict, "DAMERAU_LEVENSHTEIN", PYDAMERAU_LEVENSHTEIN);

error:
    if(PyErr_Occurred())
    {
        PyErr_SetString(PyExc_ImportError, "levtree: init failed");
        Py_DECREF(m);
        m = NULL;
    }
    return m;
}

