#include "parts.h"
#include "util.h"


static PyObject *
complex_check(PyObject *Py_UNUSED(module), PyObject *obj)
{
    NULLABLE(obj);
    return PyLong_FromLong(PyComplex_Check(obj));
}

static PyObject *
complex_checkexact(PyObject *Py_UNUSED(module), PyObject *obj)
{
    NULLABLE(obj);
    return PyLong_FromLong(PyComplex_CheckExact(obj));
}

static PyObject *
complex_fromdoubles(PyObject *Py_UNUSED(module), PyObject *args)
{
    double real, imag;

    if (!PyArg_ParseTuple(args, "dd", &real, &imag)) {
        return NULL;
    }

    return PyComplex_FromDoubles(real, imag);
}

static PyObject *
complex_realasdouble(PyObject *Py_UNUSED(module), PyObject *obj)
{
    double real;

    NULLABLE(obj);
    real = PyComplex_RealAsDouble(obj);

    if (real == -1. && PyErr_Occurred()) {
        return NULL;
    }

    return PyFloat_FromDouble(real);
}

static PyObject *
complex_imagasdouble(PyObject *Py_UNUSED(module), PyObject *obj)
{
    double imag;

    NULLABLE(obj);
    imag = PyComplex_ImagAsDouble(obj);

    if (imag == -1. && PyErr_Occurred()) {
        return NULL;
    }

    return PyFloat_FromDouble(imag);
}


static PyMethodDef test_methods[] = {
    {"complex_check", complex_check, METH_O|METH_C_STACK_FRUGAL},
    {"complex_checkexact", complex_checkexact, METH_O|METH_C_STACK_FRUGAL},
    {"complex_fromdoubles", complex_fromdoubles, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"complex_realasdouble", complex_realasdouble, METH_O|METH_C_STACK_FRUGAL},
    {"complex_imagasdouble", complex_imagasdouble, METH_O|METH_C_STACK_FRUGAL},
    {NULL},
};

int
_PyTestLimitedCAPI_Init_Complex(PyObject *mod)
{
    if (PyModule_AddFunctions(mod, test_methods) < 0) {
        return -1;
    }

    return 0;
}
