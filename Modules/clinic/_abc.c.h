/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_modsupport.h"    // _PyArg_CheckPositional()

PyDoc_STRVAR(_abc__reset_registry__doc__,
"_reset_registry($module, self, /)\n"
"--\n"
"\n"
"Internal ABC helper to reset registry of a given class.\n"
"\n"
"Should be only used by refleak.py");

static struct {char name[];} _abc__reset_registry_MethodName = {.name = "_reset_registry",};
#define _ABC__RESET_REGISTRY_METHODDEF    \
    {_abc__reset_registry_MethodName.name, (PyCFunction)_abc__reset_registry, METH_O|METH_C_STACK_FRUGAL, _abc__reset_registry__doc__},

PyDoc_STRVAR(_abc__reset_caches__doc__,
"_reset_caches($module, self, /)\n"
"--\n"
"\n"
"Internal ABC helper to reset both caches of a given class.\n"
"\n"
"Should be only used by refleak.py");

static struct {char name[];} _abc__reset_caches_MethodName = {.name = "_reset_caches",};
#define _ABC__RESET_CACHES_METHODDEF    \
    {_abc__reset_caches_MethodName.name, (PyCFunction)_abc__reset_caches, METH_O|METH_C_STACK_FRUGAL, _abc__reset_caches__doc__},

PyDoc_STRVAR(_abc__get_dump__doc__,
"_get_dump($module, self, /)\n"
"--\n"
"\n"
"Internal ABC helper for cache and registry debugging.\n"
"\n"
"Return shallow copies of registry, of both caches, and\n"
"negative cache version. Don\'t call this function directly,\n"
"instead use ABC._dump_registry() for a nice repr.");

static struct {char name[];} _abc__get_dump_MethodName = {.name = "_get_dump",};
#define _ABC__GET_DUMP_METHODDEF    \
    {_abc__get_dump_MethodName.name, (PyCFunction)_abc__get_dump, METH_O|METH_C_STACK_FRUGAL, _abc__get_dump__doc__},

PyDoc_STRVAR(_abc__abc_init__doc__,
"_abc_init($module, self, /)\n"
"--\n"
"\n"
"Internal ABC helper for class set-up. Should be never used outside abc module.");

static struct {char name[];} _abc__abc_init_MethodName = {.name = "_abc_init",};
#define _ABC__ABC_INIT_METHODDEF    \
    {_abc__abc_init_MethodName.name, (PyCFunction)_abc__abc_init, METH_O|METH_C_STACK_FRUGAL, _abc__abc_init__doc__},

PyDoc_STRVAR(_abc__abc_register__doc__,
"_abc_register($module, self, subclass, /)\n"
"--\n"
"\n"
"Internal ABC helper for subclasss registration. Should be never used outside abc module.");

static struct {char name[];} _abc__abc_register_MethodName = {.name = "_abc_register",};
#define _ABC__ABC_REGISTER_METHODDEF    \
    {_abc__abc_register_MethodName.name, _PyCFunction_CAST(_abc__abc_register), METH_FASTCALL|METH_C_STACK_FRUGAL, _abc__abc_register__doc__},

static PyObject *
_abc__abc_register_impl(PyObject *module, PyObject *self, PyObject *subclass);

static PyObject *
_abc__abc_register(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *self;
    PyObject *subclass;

    if (!_PyArg_CheckPositional("_abc_register", nargs, 2, 2)) {
        goto exit;
    }
    self = args[0];
    subclass = args[1];
    return_value = _abc__abc_register_impl(module, self, subclass);

exit:
    return return_value;
}

PyDoc_STRVAR(_abc__abc_instancecheck__doc__,
"_abc_instancecheck($module, self, instance, /)\n"
"--\n"
"\n"
"Internal ABC helper for instance checks. Should be never used outside abc module.");

static struct {char name[];} _abc__abc_instancecheck_MethodName = {.name = "_abc_instancecheck",};
#define _ABC__ABC_INSTANCECHECK_METHODDEF    \
    {_abc__abc_instancecheck_MethodName.name, _PyCFunction_CAST(_abc__abc_instancecheck), METH_FASTCALL|METH_C_STACK_FRUGAL, _abc__abc_instancecheck__doc__},

static PyObject *
_abc__abc_instancecheck_impl(PyObject *module, PyObject *self,
                             PyObject *instance);

static PyObject *
_abc__abc_instancecheck(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *self;
    PyObject *instance;

    if (!_PyArg_CheckPositional("_abc_instancecheck", nargs, 2, 2)) {
        goto exit;
    }
    self = args[0];
    instance = args[1];
    return_value = _abc__abc_instancecheck_impl(module, self, instance);

exit:
    return return_value;
}

PyDoc_STRVAR(_abc__abc_subclasscheck__doc__,
"_abc_subclasscheck($module, self, subclass, /)\n"
"--\n"
"\n"
"Internal ABC helper for subclasss checks. Should be never used outside abc module.");

static struct {char name[];} _abc__abc_subclasscheck_MethodName = {.name = "_abc_subclasscheck",};
#define _ABC__ABC_SUBCLASSCHECK_METHODDEF    \
    {_abc__abc_subclasscheck_MethodName.name, _PyCFunction_CAST(_abc__abc_subclasscheck), METH_FASTCALL|METH_C_STACK_FRUGAL, _abc__abc_subclasscheck__doc__},

static PyObject *
_abc__abc_subclasscheck_impl(PyObject *module, PyObject *self,
                             PyObject *subclass);

static PyObject *
_abc__abc_subclasscheck(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *self;
    PyObject *subclass;

    if (!_PyArg_CheckPositional("_abc_subclasscheck", nargs, 2, 2)) {
        goto exit;
    }
    self = args[0];
    subclass = args[1];
    return_value = _abc__abc_subclasscheck_impl(module, self, subclass);

exit:
    return return_value;
}

PyDoc_STRVAR(_abc_get_cache_token__doc__,
"get_cache_token($module, /)\n"
"--\n"
"\n"
"Returns the current ABC cache token.\n"
"\n"
"The token is an opaque object (supporting equality testing) identifying the\n"
"current version of the ABC cache for virtual subclasses. The token changes\n"
"with every call to register() on any ABC.");

static struct {char name[];} _abc_get_cache_token_MethodName = {.name = "get_cache_token",};
#define _ABC_GET_CACHE_TOKEN_METHODDEF    \
    {_abc_get_cache_token_MethodName.name, (PyCFunction)_abc_get_cache_token, METH_NOARGS|METH_C_STACK_FRUGAL, _abc_get_cache_token__doc__},

static PyObject *
_abc_get_cache_token_impl(PyObject *module);

static PyObject *
_abc_get_cache_token(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _abc_get_cache_token_impl(module);
}
/*[clinic end generated code: output=19db91844a0f99d8 input=a9049054013a1b77]*/
