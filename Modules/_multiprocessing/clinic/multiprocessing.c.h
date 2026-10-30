/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_modsupport.h"    // _PyArg_CheckPositional()

#if defined(MS_WINDOWS)

PyDoc_STRVAR(_multiprocessing_closesocket__doc__,
"closesocket($module, handle, /)\n"
"--\n"
"\n");

static struct {char name[];} _multiprocessing_closesocket_MethodName = {.name = "closesocket",};
#define _MULTIPROCESSING_CLOSESOCKET_METHODDEF    \
    {_multiprocessing_closesocket_MethodName.name, (PyCFunction)_multiprocessing_closesocket, METH_O|METH_C_STACK_FRUGAL, _multiprocessing_closesocket__doc__},

static PyObject *
_multiprocessing_closesocket_impl(PyObject *module, HANDLE handle);

static PyObject *
_multiprocessing_closesocket(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    HANDLE handle;

    handle = PyLong_AsVoidPtr(arg);
    if (!handle && PyErr_Occurred()) {
        goto exit;
    }
    return_value = _multiprocessing_closesocket_impl(module, handle);

exit:
    return return_value;
}

#endif /* defined(MS_WINDOWS) */

#if defined(MS_WINDOWS)

PyDoc_STRVAR(_multiprocessing_recv__doc__,
"recv($module, handle, size, /)\n"
"--\n"
"\n");

static struct {char name[];} _multiprocessing_recv_MethodName = {.name = "recv",};
#define _MULTIPROCESSING_RECV_METHODDEF    \
    {_multiprocessing_recv_MethodName.name, _PyCFunction_CAST(_multiprocessing_recv), METH_FASTCALL|METH_C_STACK_FRUGAL, _multiprocessing_recv__doc__},

static PyObject *
_multiprocessing_recv_impl(PyObject *module, HANDLE handle, int size);

static PyObject *
_multiprocessing_recv(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    HANDLE handle;
    int size;

    if (!_PyArg_CheckPositional("recv", nargs, 2, 2)) {
        goto exit;
    }
    handle = PyLong_AsVoidPtr(args[0]);
    if (!handle && PyErr_Occurred()) {
        goto exit;
    }
    size = PyLong_AsInt(args[1]);
    if (size == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = _multiprocessing_recv_impl(module, handle, size);

exit:
    return return_value;
}

#endif /* defined(MS_WINDOWS) */

#if defined(MS_WINDOWS)

PyDoc_STRVAR(_multiprocessing_send__doc__,
"send($module, handle, buf, /)\n"
"--\n"
"\n");

static struct {char name[];} _multiprocessing_send_MethodName = {.name = "send",};
#define _MULTIPROCESSING_SEND_METHODDEF    \
    {_multiprocessing_send_MethodName.name, _PyCFunction_CAST(_multiprocessing_send), METH_FASTCALL|METH_C_STACK_FRUGAL, _multiprocessing_send__doc__},

static PyObject *
_multiprocessing_send_impl(PyObject *module, HANDLE handle, Py_buffer *buf);

static PyObject *
_multiprocessing_send(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    HANDLE handle;
    Py_buffer buf = {NULL, NULL};

    if (!_PyArg_CheckPositional("send", nargs, 2, 2)) {
        goto exit;
    }
    handle = PyLong_AsVoidPtr(args[0]);
    if (!handle && PyErr_Occurred()) {
        goto exit;
    }
    if (PyObject_GetBuffer(args[1], &buf, PyBUF_SIMPLE) != 0) {
        goto exit;
    }
    return_value = _multiprocessing_send_impl(module, handle, &buf);

exit:
    /* Cleanup for buf */
    if (buf.obj) {
       PyBuffer_Release(&buf);
    }

    return return_value;
}

#endif /* defined(MS_WINDOWS) */

PyDoc_STRVAR(_multiprocessing_sem_unlink__doc__,
"sem_unlink($module, name, /)\n"
"--\n"
"\n");

static struct {char name[];} _multiprocessing_sem_unlink_MethodName = {.name = "sem_unlink",};
#define _MULTIPROCESSING_SEM_UNLINK_METHODDEF    \
    {_multiprocessing_sem_unlink_MethodName.name, (PyCFunction)_multiprocessing_sem_unlink, METH_O|METH_C_STACK_FRUGAL, _multiprocessing_sem_unlink__doc__},

static PyObject *
_multiprocessing_sem_unlink_impl(PyObject *module, const char *name);

static PyObject *
_multiprocessing_sem_unlink(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    const char *name;

    if (!PyUnicode_Check(arg)) {
        _PyArg_BadArgument("sem_unlink", "argument", "str", arg);
        goto exit;
    }
    Py_ssize_t name_length;
    name = PyUnicode_AsUTF8AndSize(arg, &name_length);
    if (name == NULL) {
        goto exit;
    }
    if (strlen(name) != (size_t)name_length) {
        PyErr_SetString(PyExc_ValueError, "embedded null character");
        goto exit;
    }
    return_value = _multiprocessing_sem_unlink_impl(module, name);

exit:
    return return_value;
}

#ifndef _MULTIPROCESSING_CLOSESOCKET_METHODDEF
    #define _MULTIPROCESSING_CLOSESOCKET_METHODDEF
#endif /* !defined(_MULTIPROCESSING_CLOSESOCKET_METHODDEF) */

#ifndef _MULTIPROCESSING_RECV_METHODDEF
    #define _MULTIPROCESSING_RECV_METHODDEF
#endif /* !defined(_MULTIPROCESSING_RECV_METHODDEF) */

#ifndef _MULTIPROCESSING_SEND_METHODDEF
    #define _MULTIPROCESSING_SEND_METHODDEF
#endif /* !defined(_MULTIPROCESSING_SEND_METHODDEF) */
/*[clinic end generated code: output=5980d0e216659071 input=a9049054013a1b77]*/
