#include "parts.h"


static PyObject *
_testcapi_config_get(PyObject *module, PyObject *name_obj)
{
    const char *name;
    if (PyArg_Parse(name_obj, "s", &name) < 0) {
        return NULL;
    }

    return PyConfig_Get(name);
}


static PyObject *
_testcapi_config_getint(PyObject *module, PyObject *name_obj)
{
    const char *name;
    if (PyArg_Parse(name_obj, "s", &name) < 0) {
        return NULL;
    }

    int value;
    if (PyConfig_GetInt(name, &value) < 0) {
        return NULL;
    }
    return PyLong_FromLong(value);
}


static PyObject *
_testcapi_config_names(PyObject *module, PyObject* Py_UNUSED(args))
{
    return PyConfig_Names();
}


static PyObject *
_testcapi_config_set(PyObject *module, PyObject *args)
{
    const char *name;
    PyObject *value;
    if (PyArg_ParseTuple(args, "sO", &name, &value) < 0) {
        return NULL;
    }

    int res = PyConfig_Set(name, value);
    if (res < 0) {
        return NULL;
    }
    Py_RETURN_NONE;
}


static PyMethodDef test_methods[] = {
    {"config_get", _testcapi_config_get, METH_O|METH_C_STACK_FRUGAL},
    {"config_getint", _testcapi_config_getint, METH_O|METH_C_STACK_FRUGAL},
    {"config_names", _testcapi_config_names, METH_NOARGS|METH_C_STACK_FRUGAL},
    {"config_set", _testcapi_config_set, METH_VARARGS|METH_C_STACK_FRUGAL},
    {NULL}
};

int
_PyTestCapi_Init_Config(PyObject *mod)
{
    return PyModule_AddFunctions(mod, test_methods);
}
