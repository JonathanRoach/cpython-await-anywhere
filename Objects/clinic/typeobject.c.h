/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_modsupport.h"    // _PyArg_BadArgument()

PyDoc_STRVAR(type___instancecheck____doc__,
"__instancecheck__($self, instance, /)\n"
"--\n"
"\n"
"Check if an object is an instance.");

static struct {char name[];} type___instancecheck___MethodName = {.name = "__instancecheck__",};
#define TYPE___INSTANCECHECK___METHODDEF    \
    {type___instancecheck___MethodName.name, (PyCFunction)type___instancecheck__, METH_O|METH_C_STACK_FRUGAL, type___instancecheck____doc__},

static int
type___instancecheck___impl(PyTypeObject *self, PyObject *instance);

static PyObject *
type___instancecheck__(PyObject *self, PyObject *instance)
{
    PyObject *return_value = NULL;
    int _return_value;

    _return_value = type___instancecheck___impl((PyTypeObject *)self, instance);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyBool_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(type___subclasscheck____doc__,
"__subclasscheck__($self, subclass, /)\n"
"--\n"
"\n"
"Check if a class is a subclass.");

static struct {char name[];} type___subclasscheck___MethodName = {.name = "__subclasscheck__",};
#define TYPE___SUBCLASSCHECK___METHODDEF    \
    {type___subclasscheck___MethodName.name, (PyCFunction)type___subclasscheck__, METH_O|METH_C_STACK_FRUGAL, type___subclasscheck____doc__},

static int
type___subclasscheck___impl(PyTypeObject *self, PyObject *subclass);

static PyObject *
type___subclasscheck__(PyObject *self, PyObject *subclass)
{
    PyObject *return_value = NULL;
    int _return_value;

    _return_value = type___subclasscheck___impl((PyTypeObject *)self, subclass);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyBool_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(type_mro__doc__,
"mro($self, /)\n"
"--\n"
"\n"
"Return a type\'s method resolution order.");

static struct {char name[];} type_mro_MethodName = {.name = "mro",};
#define TYPE_MRO_METHODDEF    \
    {type_mro_MethodName.name, (PyCFunction)type_mro, METH_NOARGS|METH_C_STACK_FRUGAL, type_mro__doc__},

static PyObject *
type_mro_impl(PyTypeObject *self);

static PyObject *
type_mro(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return type_mro_impl((PyTypeObject *)self);
}

PyDoc_STRVAR(type___subclasses____doc__,
"__subclasses__($self, /)\n"
"--\n"
"\n"
"Return a list of immediate subclasses.");

static struct {char name[];} type___subclasses___MethodName = {.name = "__subclasses__",};
#define TYPE___SUBCLASSES___METHODDEF    \
    {type___subclasses___MethodName.name, (PyCFunction)type___subclasses__, METH_NOARGS|METH_C_STACK_FRUGAL, type___subclasses____doc__},

static PyObject *
type___subclasses___impl(PyTypeObject *self);

static PyObject *
type___subclasses__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return type___subclasses___impl((PyTypeObject *)self);
}

PyDoc_STRVAR(type___dir____doc__,
"__dir__($self, /)\n"
"--\n"
"\n"
"Specialized __dir__ implementation for types.");

static struct {char name[];} type___dir___MethodName = {.name = "__dir__",};
#define TYPE___DIR___METHODDEF    \
    {type___dir___MethodName.name, (PyCFunction)type___dir__, METH_NOARGS|METH_C_STACK_FRUGAL, type___dir____doc__},

static PyObject *
type___dir___impl(PyTypeObject *self);

static PyObject *
type___dir__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return type___dir___impl((PyTypeObject *)self);
}

PyDoc_STRVAR(type___sizeof____doc__,
"__sizeof__($self, /)\n"
"--\n"
"\n"
"Return memory consumption of the type object.");

static struct {char name[];} type___sizeof___MethodName = {.name = "__sizeof__",};
#define TYPE___SIZEOF___METHODDEF    \
    {type___sizeof___MethodName.name, (PyCFunction)type___sizeof__, METH_NOARGS|METH_C_STACK_FRUGAL, type___sizeof____doc__},

static PyObject *
type___sizeof___impl(PyTypeObject *self);

static PyObject *
type___sizeof__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return type___sizeof___impl((PyTypeObject *)self);
}

PyDoc_STRVAR(object___getstate____doc__,
"__getstate__($self, /)\n"
"--\n"
"\n"
"Helper for pickle.");

static struct {char name[];} object___getstate___MethodName = {.name = "__getstate__",};
#define OBJECT___GETSTATE___METHODDEF    \
    {object___getstate___MethodName.name, (PyCFunction)object___getstate__, METH_NOARGS|METH_C_STACK_FRUGAL, object___getstate____doc__},

static PyObject *
object___getstate___impl(PyObject *self);

static PyObject *
object___getstate__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return object___getstate___impl(self);
}

PyDoc_STRVAR(object___reduce____doc__,
"__reduce__($self, /)\n"
"--\n"
"\n"
"Helper for pickle.");

static struct {char name[];} object___reduce___MethodName = {.name = "__reduce__",};
#define OBJECT___REDUCE___METHODDEF    \
    {object___reduce___MethodName.name, (PyCFunction)object___reduce__, METH_NOARGS|METH_C_STACK_FRUGAL, object___reduce____doc__},

static PyObject *
object___reduce___impl(PyObject *self);

static PyObject *
object___reduce__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return object___reduce___impl(self);
}

PyDoc_STRVAR(object___reduce_ex____doc__,
"__reduce_ex__($self, protocol, /)\n"
"--\n"
"\n"
"Helper for pickle.");

static struct {char name[];} object___reduce_ex___MethodName = {.name = "__reduce_ex__",};
#define OBJECT___REDUCE_EX___METHODDEF    \
    {object___reduce_ex___MethodName.name, (PyCFunction)object___reduce_ex__, METH_O|METH_C_STACK_FRUGAL, object___reduce_ex____doc__},

static PyObject *
object___reduce_ex___impl(PyObject *self, int protocol);

static PyObject *
object___reduce_ex__(PyObject *self, PyObject *arg)
{
    PyObject *return_value = NULL;
    int protocol;

    protocol = PyLong_AsInt(arg);
    if (protocol == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = object___reduce_ex___impl(self, protocol);

exit:
    return return_value;
}

PyDoc_STRVAR(object___format____doc__,
"__format__($self, format_spec, /)\n"
"--\n"
"\n"
"Default object formatter.\n"
"\n"
"Return str(self) if format_spec is empty. Raise TypeError otherwise.");

static struct {char name[];} object___format___MethodName = {.name = "__format__",};
#define OBJECT___FORMAT___METHODDEF    \
    {object___format___MethodName.name, (PyCFunction)object___format__, METH_O|METH_C_STACK_FRUGAL, object___format____doc__},

static PyObject *
object___format___impl(PyObject *self, PyObject *format_spec);

static PyObject *
object___format__(PyObject *self, PyObject *arg)
{
    PyObject *return_value = NULL;
    PyObject *format_spec;

    if (!PyUnicode_Check(arg)) {
        _PyArg_BadArgument("__format__", "argument", "str", arg);
        goto exit;
    }
    format_spec = arg;
    return_value = object___format___impl(self, format_spec);

exit:
    return return_value;
}

PyDoc_STRVAR(object___sizeof____doc__,
"__sizeof__($self, /)\n"
"--\n"
"\n"
"Size of object in memory, in bytes.");

static struct {char name[];} object___sizeof___MethodName = {.name = "__sizeof__",};
#define OBJECT___SIZEOF___METHODDEF    \
    {object___sizeof___MethodName.name, (PyCFunction)object___sizeof__, METH_NOARGS|METH_C_STACK_FRUGAL, object___sizeof____doc__},

static PyObject *
object___sizeof___impl(PyObject *self);

static PyObject *
object___sizeof__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return object___sizeof___impl(self);
}

PyDoc_STRVAR(object___dir____doc__,
"__dir__($self, /)\n"
"--\n"
"\n"
"Default dir() implementation.");

static struct {char name[];} object___dir___MethodName = {.name = "__dir__",};
#define OBJECT___DIR___METHODDEF    \
    {object___dir___MethodName.name, (PyCFunction)object___dir__, METH_NOARGS|METH_C_STACK_FRUGAL, object___dir____doc__},

static PyObject *
object___dir___impl(PyObject *self);

static PyObject *
object___dir__(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    return object___dir___impl(self);
}
/*[clinic end generated code: output=018cbc9836dd61fb input=a9049054013a1b77]*/
