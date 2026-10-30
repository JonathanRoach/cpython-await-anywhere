/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(pwd_getpwuid__doc__,
"getpwuid($module, uidobj, /)\n"
"--\n"
"\n"
"Return the password database entry for the given numeric user ID.\n"
"\n"
"See `help(pwd)` for more on password database entries.");

static struct {char name[];} pwd_getpwuid_MethodName = {.name = "getpwuid",};
#define PWD_GETPWUID_METHODDEF    \
    {pwd_getpwuid_MethodName.name, (PyCFunction)pwd_getpwuid, METH_O|METH_C_STACK_FRUGAL, pwd_getpwuid__doc__},

PyDoc_STRVAR(pwd_getpwnam__doc__,
"getpwnam($module, name, /)\n"
"--\n"
"\n"
"Return the password database entry for the given user name.\n"
"\n"
"See `help(pwd)` for more on password database entries.");

static struct {char name[];} pwd_getpwnam_MethodName = {.name = "getpwnam",};
#define PWD_GETPWNAM_METHODDEF    \
    {pwd_getpwnam_MethodName.name, (PyCFunction)pwd_getpwnam, METH_O|METH_C_STACK_FRUGAL, pwd_getpwnam__doc__},

static PyObject *
pwd_getpwnam_impl(PyObject *module, PyObject *name);

static PyObject *
pwd_getpwnam(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    PyObject *name;

    if (!PyUnicode_Check(arg)) {
        PyErr_Format(PyExc_TypeError, "getpwnam() argument must be str, not %T", arg);
        goto exit;
    }
    name = arg;
    return_value = pwd_getpwnam_impl(module, name);

exit:
    return return_value;
}

#if defined(HAVE_GETPWENT)

PyDoc_STRVAR(pwd_getpwall__doc__,
"getpwall($module, /)\n"
"--\n"
"\n"
"Return a list of all available password database entries, in arbitrary order.\n"
"\n"
"See help(pwd) for more on password database entries.");

static struct {char name[];} pwd_getpwall_MethodName = {.name = "getpwall",};
#define PWD_GETPWALL_METHODDEF    \
    {pwd_getpwall_MethodName.name, (PyCFunction)pwd_getpwall, METH_NOARGS|METH_C_STACK_FRUGAL, pwd_getpwall__doc__},

static PyObject *
pwd_getpwall_impl(PyObject *module);

static PyObject *
pwd_getpwall(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return pwd_getpwall_impl(module);
}

#endif /* defined(HAVE_GETPWENT) */

#ifndef PWD_GETPWALL_METHODDEF
    #define PWD_GETPWALL_METHODDEF
#endif /* !defined(PWD_GETPWALL_METHODDEF) */
/*[clinic end generated code: output=6e6d016dd20d2a1f input=a9049054013a1b77]*/
