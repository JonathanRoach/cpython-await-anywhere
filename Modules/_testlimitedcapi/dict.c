#include "parts.h"
#include "util.h"


static PyObject *
dict_check(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PyLong_FromLong(PyDict_Check(obj));
}

static PyObject *
dict_checkexact(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PyLong_FromLong(PyDict_CheckExact(obj));
}

static PyObject *
dict_new(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return PyDict_New();
}

static PyObject *
dictproxy_new(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PyDictProxy_New(obj);
}

static PyObject *
dict_clear(PyObject *self, PyObject *obj)
{
    PyDict_Clear(obj);
    Py_RETURN_NONE;
}

static PyObject *
dict_copy(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PyDict_Copy(obj);
}

static PyObject *
dict_contains(PyObject *self, PyObject *args)
{
    PyObject *obj, *key;
    if (!PyArg_ParseTuple(args, "OO", &obj, &key)) {
        return NULL;
    }
    NULLABLE(obj);
    NULLABLE(key);
    RETURN_INT(PyDict_Contains(obj, key));
}

static PyObject *
dict_size(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_SIZE(PyDict_Size(obj));
}

static PyObject *
dict_getitem(PyObject *self, PyObject *args)
{
    PyObject *mapping, *key;
    if (!PyArg_ParseTuple(args, "OO", &mapping, &key)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(key);
    PyObject *value = PyDict_GetItem(mapping, key);
    if (value == NULL) {
        if (PyErr_Occurred()) {
            return NULL;
        }
        return Py_NewRef(PyExc_KeyError);
    }
    return Py_NewRef(value);
}

static PyObject *
dict_getitemstring(PyObject *self, PyObject *args)
{
    PyObject *mapping;
    const char *key;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "Oz#", &mapping, &key, &size)) {
        return NULL;
    }
    NULLABLE(mapping);
    PyObject *value = PyDict_GetItemString(mapping, key);
    if (value == NULL) {
        if (PyErr_Occurred()) {
            return NULL;
        }
        return Py_NewRef(PyExc_KeyError);
    }
    return Py_NewRef(value);
}

static PyObject *
dict_getitemwitherror(PyObject *self, PyObject *args)
{
    PyObject *mapping, *key;
    if (!PyArg_ParseTuple(args, "OO", &mapping, &key)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(key);
    PyObject *value = PyDict_GetItemWithError(mapping, key);
    if (value == NULL) {
        if (PyErr_Occurred()) {
            return NULL;
        }
        return Py_NewRef(PyExc_KeyError);
    }
    return Py_NewRef(value);
}


static PyObject *
dict_setitem(PyObject *self, PyObject *args)
{
    PyObject *mapping, *key, *value;
    if (!PyArg_ParseTuple(args, "OOO", &mapping, &key, &value)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(key);
    NULLABLE(value);
    RETURN_INT(PyDict_SetItem(mapping, key, value));
}

static PyObject *
dict_setitemstring(PyObject *self, PyObject *args)
{
    PyObject *mapping, *value;
    const char *key;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "Oz#O", &mapping, &key, &size, &value)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(value);
    RETURN_INT(PyDict_SetItemString(mapping, key, value));
}

static PyObject *
dict_delitem(PyObject *self, PyObject *args)
{
    PyObject *mapping, *key;
    if (!PyArg_ParseTuple(args, "OO", &mapping, &key)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(key);
    RETURN_INT(PyDict_DelItem(mapping, key));
}

static PyObject *
dict_delitemstring(PyObject *self, PyObject *args)
{
    PyObject *mapping;
    const char *key;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "Oz#", &mapping, &key, &size)) {
        return NULL;
    }
    NULLABLE(mapping);
    RETURN_INT(PyDict_DelItemString(mapping, key));
}

static PyObject *
dict_keys(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PyDict_Keys(obj);
}

static PyObject *
dict_values(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PyDict_Values(obj);
}

static PyObject *
dict_items(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PyDict_Items(obj);
}

static PyObject *
dict_next(PyObject *self, PyObject *args)
{
    PyObject *mapping, *key = UNINITIALIZED_PTR, *value = UNINITIALIZED_PTR;
    Py_ssize_t pos;
    if (!PyArg_ParseTuple(args, "On", &mapping, &pos)) {
        return NULL;
    }
    NULLABLE(mapping);
    int rc = PyDict_Next(mapping, &pos, &key, &value);
    if (rc != 0) {
        return Py_BuildValue("inOO", rc, pos, key, value);
    }
    assert(key == UNINITIALIZED_PTR);
    assert(value == UNINITIALIZED_PTR);
    if (PyErr_Occurred()) {
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject *
dict_merge(PyObject *self, PyObject *args)
{
    PyObject *mapping, *mapping2;
    int override;
    if (!PyArg_ParseTuple(args, "OOi", &mapping, &mapping2, &override)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(mapping2);
    RETURN_INT(PyDict_Merge(mapping, mapping2, override));
}

static PyObject *
dict_update(PyObject *self, PyObject *args)
{
    PyObject *mapping, *mapping2;
    if (!PyArg_ParseTuple(args, "OO", &mapping, &mapping2)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(mapping2);
    RETURN_INT(PyDict_Update(mapping, mapping2));
}

static PyObject *
dict_mergefromseq2(PyObject *self, PyObject *args)
{
    PyObject *mapping, *seq;
    int override;
    if (!PyArg_ParseTuple(args, "OOi", &mapping, &seq, &override)) {
        return NULL;
    }
    NULLABLE(mapping);
    NULLABLE(seq);
    RETURN_INT(PyDict_MergeFromSeq2(mapping, seq, override));
}


static PyMethodDef test_methods[] = {
    {"dict_check", dict_check, METH_O|METH_C_STACK_FRUGAL},
    {"dict_checkexact", dict_checkexact, METH_O|METH_C_STACK_FRUGAL},
    {"dict_new", dict_new, METH_NOARGS|METH_C_STACK_FRUGAL},
    {"dictproxy_new", dictproxy_new, METH_O|METH_C_STACK_FRUGAL},
    {"dict_clear", dict_clear, METH_O|METH_C_STACK_FRUGAL},
    {"dict_copy", dict_copy, METH_O|METH_C_STACK_FRUGAL},
    {"dict_size", dict_size, METH_O|METH_C_STACK_FRUGAL},
    {"dict_getitem", dict_getitem, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_getitemwitherror", dict_getitemwitherror, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_getitemstring", dict_getitemstring, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_contains", dict_contains, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_setitem", dict_setitem, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_setitemstring", dict_setitemstring, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_delitem", dict_delitem, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_delitemstring", dict_delitemstring, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_keys", dict_keys, METH_O|METH_C_STACK_FRUGAL},
    {"dict_values", dict_values, METH_O|METH_C_STACK_FRUGAL},
    {"dict_items", dict_items, METH_O|METH_C_STACK_FRUGAL},
    {"dict_next", dict_next, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_merge", dict_merge, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_update", dict_update, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"dict_mergefromseq2", dict_mergefromseq2, METH_VARARGS|METH_C_STACK_FRUGAL},
    {NULL},
};

int
_PyTestLimitedCAPI_Init_Dict(PyObject *m)
{
    if (PyModule_AddFunctions(m, test_methods) < 0) {
        return -1;
    }

    return 0;
}
