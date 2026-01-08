#include "parts.h"
#include "util.h"

static PyObject *
set_check(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_INT(PySet_Check(obj));
}

static PyObject *
set_checkexact(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_INT(PySet_CheckExact(obj));
}

static PyObject *
frozenset_check(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_INT(PyFrozenSet_Check(obj));
}

static PyObject *
frozenset_checkexact(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_INT(PyFrozenSet_CheckExact(obj));
}

static PyObject *
anyset_check(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_INT(PyAnySet_Check(obj));
}

static PyObject *
anyset_checkexact(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_INT(PyAnySet_CheckExact(obj));
}

static PyObject *
set_new(PyObject *self, PyObject *args)
{
    PyObject *iterable = NULL;
    if (!PyArg_ParseTuple(args, "|O", &iterable)) {
        return NULL;
    }
    return PySet_New(iterable);
}

static PyObject *
frozenset_new(PyObject *self, PyObject *args)
{
    PyObject *iterable = NULL;
    if (!PyArg_ParseTuple(args, "|O", &iterable)) {
        return NULL;
    }
    return PyFrozenSet_New(iterable);
}

static PyObject *
set_size(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_SIZE(PySet_Size(obj));
}

static PyObject *
set_contains(PyObject *self, PyObject *args)
{
    PyObject *obj, *item;
    if (!PyArg_ParseTuple(args, "OO", &obj, &item)) {
        return NULL;
    }
    NULLABLE(obj);
    NULLABLE(item);
    RETURN_INT(PySet_Contains(obj, item));
}

static PyObject *
set_add(PyObject *self, PyObject *args)
{
    PyObject *obj, *item;
    if (!PyArg_ParseTuple(args, "OO", &obj, &item)) {
        return NULL;
    }
    NULLABLE(obj);
    NULLABLE(item);
    RETURN_INT(PySet_Add(obj, item));
}

static PyObject *
set_discard(PyObject *self, PyObject *args)
{
    PyObject *obj, *item;
    if (!PyArg_ParseTuple(args, "OO", &obj, &item)) {
        return NULL;
    }
    NULLABLE(obj);
    NULLABLE(item);
    RETURN_INT(PySet_Discard(obj, item));
}

static PyObject *
set_pop(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    return PySet_Pop(obj);
}

static PyObject *
set_clear(PyObject *self, PyObject *obj)
{
    NULLABLE(obj);
    RETURN_INT(PySet_Clear(obj));
}

static PyObject *
test_frozenset_add_in_capi(PyObject *self, PyObject *Py_UNUSED(obj))
{
    // Test that `frozenset` can be used with `PySet_Add`,
    // when frozenset is just created in CAPI.
    PyObject *fs = PyFrozenSet_New(NULL);
    if (fs == NULL) {
        return NULL;
    }
    PyObject *num = PyLong_FromLong(1);
    if (num == NULL) {
        goto error;
    }
    if (PySet_Add(fs, num) < 0) {
        goto error;
    }
    int contains = PySet_Contains(fs, num);
    if (contains < 0) {
        goto error;
    }
    else if (contains == 0) {
        goto unexpected;
    }
    Py_DECREF(fs);
    Py_DECREF(num);
    Py_RETURN_NONE;

unexpected:
    PyErr_SetString(PyExc_ValueError, "set does not contain expected value");
error:
    Py_DECREF(fs);
    Py_XDECREF(num);
    return NULL;
}

static PyMethodDef test_methods[] = {
    {"set_check", set_check, METH_O|METH_C_STACK_FRUGAL},
    {"set_checkexact", set_checkexact, METH_O|METH_C_STACK_FRUGAL},
    {"frozenset_check", frozenset_check, METH_O|METH_C_STACK_FRUGAL},
    {"frozenset_checkexact", frozenset_checkexact, METH_O|METH_C_STACK_FRUGAL},
    {"anyset_check", anyset_check, METH_O|METH_C_STACK_FRUGAL},
    {"anyset_checkexact", anyset_checkexact, METH_O|METH_C_STACK_FRUGAL},

    {"set_new", set_new, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"frozenset_new", frozenset_new, METH_VARARGS|METH_C_STACK_FRUGAL},

    {"set_size", set_size, METH_O|METH_C_STACK_FRUGAL},
    {"set_contains", set_contains, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"set_add", set_add, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"set_discard", set_discard, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"set_pop", set_pop, METH_O|METH_C_STACK_FRUGAL},
    {"set_clear", set_clear, METH_O|METH_C_STACK_FRUGAL},

    {"test_frozenset_add_in_capi", test_frozenset_add_in_capi, METH_NOARGS|METH_C_STACK_FRUGAL},

    {NULL},
};

int
_PyTestLimitedCAPI_Init_Set(PyObject *m)
{
    if (PyModule_AddFunctions(m, test_methods) < 0) {
        return -1;
    }

    return 0;
}
