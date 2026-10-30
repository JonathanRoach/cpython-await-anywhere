/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(_contextvars_copy_context__doc__,
"copy_context($module, /)\n"
"--\n"
"\n");

static struct {char name[];} _contextvars_copy_context_MethodName = {.name = "copy_context",};
#define _CONTEXTVARS_COPY_CONTEXT_METHODDEF    \
    {_contextvars_copy_context_MethodName.name, (PyCFunction)_contextvars_copy_context, METH_NOARGS|METH_C_STACK_FRUGAL, _contextvars_copy_context__doc__},

static PyObject *
_contextvars_copy_context_impl(PyObject *module);

static PyObject *
_contextvars_copy_context(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _contextvars_copy_context_impl(module);
}
/*[clinic end generated code: output=de525ba0f1138bbf input=a9049054013a1b77]*/
