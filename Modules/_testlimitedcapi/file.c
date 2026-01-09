#include "pyconfig.h"   // Py_GIL_DISABLED
#ifndef Py_GIL_DISABLED
   // Need limited C API 3.13 for PyLong_AsInt()
#  define Py_LIMITED_API 0x030d0000
#endif

#include "parts.h"
#include "util.h"
#include "clinic/file.c.h"


/*[clinic input]
module _testcapi
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=6361033e795369fc]*/


static PyObject *
pyfile_fromfd(PyObject *module, PyObject *args)
{
    int fd;
    const char *name;
    Py_ssize_t size;
    const char *mode;
    int buffering;
    const char *encoding;
    const char *errors;
    const char *newline;
    int closefd;
    if (!PyArg_ParseTuple(args,
                          "iz#z#"
                          "iz#z#"
                          "z#i",
                          &fd, &name, &size, &mode, &size,
                          &buffering, &encoding, &size, &errors, &size,
                          &newline, &size, &closefd)) {
        return NULL;
    }

    return PyFile_FromFd(fd, name, mode, buffering,
                         encoding, errors, newline, closefd);
}


/*[clinic input]
@c_stack_frugal
_testcapi.pyfile_getline

    file: object
    n: int
    /

[clinic start generated code]*/

static PyObject *
_testcapi_pyfile_getline_impl(PyObject *module, PyObject *file, int n)
/*[clinic end generated code: output=137fde2774563266 input=6ac0397363d99f20]*/
{
    return PyFile_GetLine(file, n);
}


/*[clinic input]
@c_stack_frugal
_testcapi.pyfile_writeobject

    obj: object
    file: object
    flags: int
    /

[clinic start generated code]*/

static PyObject *
_testcapi_pyfile_writeobject_impl(PyObject *module, PyObject *obj,
                                  PyObject *file, int flags)
/*[clinic end generated code: output=ebb4d802e3db489c input=289eee5dcc9fbfcf]*/
{
    NULLABLE(obj);
    NULLABLE(file);
    RETURN_INT(PyFile_WriteObject(obj, file, flags));
}


static PyObject *
pyfile_writestring(PyObject *module, PyObject *args)
{
    const char *str;
    Py_ssize_t size;
    PyObject *file;
    if (!PyArg_ParseTuple(args, "z#O", &str, &size, &file)) {
        return NULL;
    }
    NULLABLE(file);

    RETURN_INT(PyFile_WriteString(str, file));
}


/*[clinic input]
@c_stack_frugal
_testcapi.pyobject_asfiledescriptor

    obj: object
    /

[clinic start generated code]*/

static PyObject *
_testcapi_pyobject_asfiledescriptor(PyObject *module, PyObject *obj)
/*[clinic end generated code: output=2d640c6a1970c721 input=d6001e8c18acd527]*/
{
    NULLABLE(obj);
    RETURN_INT(PyObject_AsFileDescriptor(obj));
}


static PyMethodDef test_methods[] = {
    {"pyfile_fromfd", pyfile_fromfd, METH_VARARGS|METH_C_STACK_FRUGAL},
    _TESTCAPI_PYFILE_GETLINE_METHODDEF
    _TESTCAPI_PYFILE_WRITEOBJECT_METHODDEF
    {"pyfile_writestring", pyfile_writestring, METH_VARARGS|METH_C_STACK_FRUGAL},
    _TESTCAPI_PYOBJECT_ASFILEDESCRIPTOR_METHODDEF
    {NULL},
};

int
_PyTestLimitedCAPI_Init_File(PyObject *m)
{
    return PyModule_AddFunctions(m, test_methods);
}
