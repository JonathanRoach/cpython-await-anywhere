/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(_contextvars_copy_context__doc__,
"copy_context($module, /)\n"
"--\n"
"\n");

static PyObject *
_contextvars_copy_context_impl(PyObject *module);

static PyObject *
_contextvars_copy_context(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _contextvars_copy_context_impl(module);
}

static struct {char name[];} _contextvars_copy_context_MethodName = {.name = "copy_context",};
#define _CONTEXTVARS_COPY_CONTEXT_METHODDEF    \
    {_contextvars_copy_context_MethodName.name, (PyCFunction)_contextvars_copy_context, METH_NOARGS|METH_C_STACK_FRUGAL, _contextvars_copy_context__doc__},
/*[clinic end generated code: output=a7ec1d4bc8e3ed6e input=a9049054013a1b77]*/
