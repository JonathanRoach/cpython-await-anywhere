/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_critical_section.h"// Py_BEGIN_CRITICAL_SECTION()
#include "pycore_modsupport.h"    // _PyArg_CheckPositional()

PyDoc_STRVAR(dict_fromkeys__doc__,
"fromkeys($type, iterable, value=None, /)\n"
"--\n"
"\n"
"Create a new dictionary with keys from iterable and values set to value.");

static struct {char name[];} dict_fromkeys_MethodName = {.name = "fromkeys",};
#define DICT_FROMKEYS_METHODDEF    \
    {dict_fromkeys_MethodName.name, _PyCFunction_CAST(dict_fromkeys), METH_FASTCALL|METH_CLASS|METH_C_STACK_FRUGAL, dict_fromkeys__doc__},

static PyObject *
dict_fromkeys_impl(PyTypeObject *type, PyObject *iterable, PyObject *value);

static PyObject *
dict_fromkeys(PyObject *type, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *iterable;
    PyObject *value = Py_None;

    if (!_PyArg_CheckPositional("fromkeys", nargs, 1, 2)) {
        goto exit;
    }
    iterable = args[0];
    if (nargs < 2) {
        goto skip_optional;
    }
    value = args[1];
skip_optional:
    return_value = dict_fromkeys_impl((PyTypeObject *)type, iterable, value);

exit:
    return return_value;
}

PyDoc_STRVAR(dict_copy__doc__,
"copy($self, /)\n"
"--\n"
"\n"
"Return a shallow copy of the dict.");

static struct {char name[];} dict_copy_MethodName = {.name = "copy",};
#define DICT_COPY_METHODDEF    \
    {dict_copy_MethodName.name, (PyCFunction)dict_copy, METH_NOARGS|METH_C_STACK_FRUGAL, dict_copy__doc__},

static PyObject *
dict_copy_impl(PyDictObject *self);

static PyObject *
dict_copy(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return dict_copy_impl((PyDictObject *)self);
}

PyDoc_STRVAR(dict___contains____doc__,
"__contains__($self, key, /)\n"
"--\n"
"\n"
"True if the dictionary has the specified key, else False.");

static struct {char name[];} dict___contains___MethodName = {.name = "__contains__",};
#define DICT___CONTAINS___METHODDEF    \
    {dict___contains___MethodName.name, (PyCFunction)dict___contains__, METH_O|METH_COEXIST|METH_C_STACK_FRUGAL, dict___contains____doc__},

static PyObject *
dict___contains___impl(PyDictObject *self, PyObject *key);

static PyObject *
dict___contains__(PyObject *self, PyObject *key)
{
    PyObject *return_value = NULL;

    return_value = dict___contains___impl((PyDictObject *)self, key);

    return return_value;
}

PyDoc_STRVAR(dict_get__doc__,
"get($self, key, default=None, /)\n"
"--\n"
"\n"
"Return the value for key if key is in the dictionary, else default.");

static struct {char name[];} dict_get_MethodName = {.name = "get",};
#define DICT_GET_METHODDEF    \
    {dict_get_MethodName.name, _PyCFunction_CAST(dict_get), METH_FASTCALL|METH_C_STACK_FRUGAL, dict_get__doc__},

static PyObject *
dict_get_impl(PyDictObject *self, PyObject *key, PyObject *default_value);

static PyObject *
dict_get(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *key;
    PyObject *default_value = Py_None;

    if (!_PyArg_CheckPositional("get", nargs, 1, 2)) {
        goto exit;
    }
    key = args[0];
    if (nargs < 2) {
        goto skip_optional;
    }
    default_value = args[1];
skip_optional:
    return_value = dict_get_impl((PyDictObject *)self, key, default_value);

exit:
    return return_value;
}

PyDoc_STRVAR(dict_setdefault__doc__,
"setdefault($self, key, default=None, /)\n"
"--\n"
"\n"
"Insert key with a value of default if key is not in the dictionary.\n"
"\n"
"Return the value for key if key is in the dictionary, else default.");

static struct {char name[];} dict_setdefault_MethodName = {.name = "setdefault",};
#define DICT_SETDEFAULT_METHODDEF    \
    {dict_setdefault_MethodName.name, _PyCFunction_CAST(dict_setdefault), METH_FASTCALL|METH_C_STACK_FRUGAL, dict_setdefault__doc__},

static PyObject *
dict_setdefault_impl(PyDictObject *self, PyObject *key,
                     PyObject *default_value);

static PyObject *
dict_setdefault(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *key;
    PyObject *default_value = Py_None;

    if (!_PyArg_CheckPositional("setdefault", nargs, 1, 2)) {
        goto exit;
    }
    key = args[0];
    if (nargs < 2) {
        goto skip_optional;
    }
    default_value = args[1];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(self);
    return_value = dict_setdefault_impl((PyDictObject *)self, key, default_value);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(dict_clear__doc__,
"clear($self, /)\n"
"--\n"
"\n"
"Remove all items from the dict.");

static struct {char name[];} dict_clear_MethodName = {.name = "clear",};
#define DICT_CLEAR_METHODDEF    \
    {dict_clear_MethodName.name, (PyCFunction)dict_clear, METH_NOARGS|METH_C_STACK_FRUGAL, dict_clear__doc__},

static PyObject *
dict_clear_impl(PyDictObject *self);

static PyObject *
dict_clear(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return dict_clear_impl((PyDictObject *)self);
}

PyDoc_STRVAR(dict_pop__doc__,
"pop($self, key, default=<unrepresentable>, /)\n"
"--\n"
"\n"
"D.pop(k[,d]) -> v, remove specified key and return the corresponding value.\n"
"\n"
"If the key is not found, return the default if given; otherwise,\n"
"raise a KeyError.");

static struct {char name[];} dict_pop_MethodName = {.name = "pop",};
#define DICT_POP_METHODDEF    \
    {dict_pop_MethodName.name, _PyCFunction_CAST(dict_pop), METH_FASTCALL|METH_C_STACK_FRUGAL, dict_pop__doc__},

static PyObject *
dict_pop_impl(PyDictObject *self, PyObject *key, PyObject *default_value);

static PyObject *
dict_pop(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *key;
    PyObject *default_value = NULL;

    if (!_PyArg_CheckPositional("pop", nargs, 1, 2)) {
        goto exit;
    }
    key = args[0];
    if (nargs < 2) {
        goto skip_optional;
    }
    default_value = args[1];
skip_optional:
    return_value = dict_pop_impl((PyDictObject *)self, key, default_value);

exit:
    return return_value;
}

PyDoc_STRVAR(dict_popitem__doc__,
"popitem($self, /)\n"
"--\n"
"\n"
"Remove and return a (key, value) pair as a 2-tuple.\n"
"\n"
"Pairs are returned in LIFO (last-in, first-out) order.\n"
"Raises KeyError if the dict is empty.");

static struct {char name[];} dict_popitem_MethodName = {.name = "popitem",};
#define DICT_POPITEM_METHODDEF    \
    {dict_popitem_MethodName.name, (PyCFunction)dict_popitem, METH_NOARGS|METH_C_STACK_FRUGAL, dict_popitem__doc__},

static PyObject *
dict_popitem_impl(PyDictObject *self);

static PyObject *
dict_popitem(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(self);
    return_value = dict_popitem_impl((PyDictObject *)self);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(dict___sizeof____doc__,
"__sizeof__($self, /)\n"
"--\n"
"\n"
"Return the size of the dict in memory, in bytes.");

static struct {char name[];} dict___sizeof___MethodName = {.name = "__sizeof__",};
#define DICT___SIZEOF___METHODDEF    \
    {dict___sizeof___MethodName.name, (PyCFunction)dict___sizeof__, METH_NOARGS|METH_C_STACK_FRUGAL, dict___sizeof____doc__},

static PyObject *
dict___sizeof___impl(PyDictObject *self);

static PyObject *
dict___sizeof__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return dict___sizeof___impl((PyDictObject *)self);
}

PyDoc_STRVAR(dict___reversed____doc__,
"__reversed__($self, /)\n"
"--\n"
"\n"
"Return a reverse iterator over the dict keys.");

static struct {char name[];} dict___reversed___MethodName = {.name = "__reversed__",};
#define DICT___REVERSED___METHODDEF    \
    {dict___reversed___MethodName.name, (PyCFunction)dict___reversed__, METH_NOARGS|METH_C_STACK_FRUGAL, dict___reversed____doc__},

static PyObject *
dict___reversed___impl(PyDictObject *self);

static PyObject *
dict___reversed__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return dict___reversed___impl((PyDictObject *)self);
}

PyDoc_STRVAR(dict_keys__doc__,
"keys($self, /)\n"
"--\n"
"\n"
"Return a set-like object providing a view on the dict\'s keys.");

static struct {char name[];} dict_keys_MethodName = {.name = "keys",};
#define DICT_KEYS_METHODDEF    \
    {dict_keys_MethodName.name, (PyCFunction)dict_keys, METH_NOARGS|METH_C_STACK_FRUGAL, dict_keys__doc__},

static PyObject *
dict_keys_impl(PyDictObject *self);

static PyObject *
dict_keys(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return dict_keys_impl((PyDictObject *)self);
}

PyDoc_STRVAR(dict_items__doc__,
"items($self, /)\n"
"--\n"
"\n"
"Return a set-like object providing a view on the dict\'s items.");

static struct {char name[];} dict_items_MethodName = {.name = "items",};
#define DICT_ITEMS_METHODDEF    \
    {dict_items_MethodName.name, (PyCFunction)dict_items, METH_NOARGS|METH_C_STACK_FRUGAL, dict_items__doc__},

static PyObject *
dict_items_impl(PyDictObject *self);

static PyObject *
dict_items(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return dict_items_impl((PyDictObject *)self);
}

PyDoc_STRVAR(dict_values__doc__,
"values($self, /)\n"
"--\n"
"\n"
"Return an object providing a view on the dict\'s values.");

static struct {char name[];} dict_values_MethodName = {.name = "values",};
#define DICT_VALUES_METHODDEF    \
    {dict_values_MethodName.name, (PyCFunction)dict_values, METH_NOARGS|METH_C_STACK_FRUGAL, dict_values__doc__},

static PyObject *
dict_values_impl(PyDictObject *self);

static PyObject *
dict_values(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return dict_values_impl((PyDictObject *)self);
}
/*[clinic end generated code: output=a71f3929bc403012 input=a9049054013a1b77]*/
