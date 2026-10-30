/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(_sysconfig_config_vars__doc__,
"config_vars($module, /)\n"
"--\n"
"\n"
"Returns a dictionary containing build variables intended to be exposed by sysconfig.");

static struct {char name[];} _sysconfig_config_vars_MethodName = {.name = "config_vars",};
#define _SYSCONFIG_CONFIG_VARS_METHODDEF    \
    {_sysconfig_config_vars_MethodName.name, (PyCFunction)_sysconfig_config_vars, METH_NOARGS|METH_C_STACK_FRUGAL, _sysconfig_config_vars__doc__},

static PyObject *
_sysconfig_config_vars_impl(PyObject *module);

static PyObject *
_sysconfig_config_vars(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _sysconfig_config_vars_impl(module);
}
/*[clinic end generated code: output=db26af80c8203e45 input=a9049054013a1b77]*/
