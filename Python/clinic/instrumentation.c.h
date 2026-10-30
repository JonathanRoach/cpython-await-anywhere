/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_modsupport.h"    // _PyArg_CheckPositional()

PyDoc_STRVAR(monitoring_use_tool_id__doc__,
"use_tool_id($module, tool_id, name, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_use_tool_id_MethodName = {.name = "use_tool_id",};
#define MONITORING_USE_TOOL_ID_METHODDEF    \
    {monitoring_use_tool_id_MethodName.name, _PyCFunction_CAST(monitoring_use_tool_id), METH_FASTCALL|METH_C_STACK_FRUGAL, monitoring_use_tool_id__doc__},

static PyObject *
monitoring_use_tool_id_impl(PyObject *module, int tool_id, PyObject *name);

static PyObject *
monitoring_use_tool_id(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    int tool_id;
    PyObject *name;

    if (!_PyArg_CheckPositional("use_tool_id", nargs, 2, 2)) {
        goto exit;
    }
    tool_id = PyLong_AsInt(args[0]);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    name = args[1];
    return_value = monitoring_use_tool_id_impl(module, tool_id, name);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_clear_tool_id__doc__,
"clear_tool_id($module, tool_id, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_clear_tool_id_MethodName = {.name = "clear_tool_id",};
#define MONITORING_CLEAR_TOOL_ID_METHODDEF    \
    {monitoring_clear_tool_id_MethodName.name, (PyCFunction)monitoring_clear_tool_id, METH_O|METH_C_STACK_FRUGAL, monitoring_clear_tool_id__doc__},

static PyObject *
monitoring_clear_tool_id_impl(PyObject *module, int tool_id);

static PyObject *
monitoring_clear_tool_id(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int tool_id;

    tool_id = PyLong_AsInt(arg);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = monitoring_clear_tool_id_impl(module, tool_id);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_free_tool_id__doc__,
"free_tool_id($module, tool_id, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_free_tool_id_MethodName = {.name = "free_tool_id",};
#define MONITORING_FREE_TOOL_ID_METHODDEF    \
    {monitoring_free_tool_id_MethodName.name, (PyCFunction)monitoring_free_tool_id, METH_O|METH_C_STACK_FRUGAL, monitoring_free_tool_id__doc__},

static PyObject *
monitoring_free_tool_id_impl(PyObject *module, int tool_id);

static PyObject *
monitoring_free_tool_id(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int tool_id;

    tool_id = PyLong_AsInt(arg);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = monitoring_free_tool_id_impl(module, tool_id);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_get_tool__doc__,
"get_tool($module, tool_id, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_get_tool_MethodName = {.name = "get_tool",};
#define MONITORING_GET_TOOL_METHODDEF    \
    {monitoring_get_tool_MethodName.name, (PyCFunction)monitoring_get_tool, METH_O|METH_C_STACK_FRUGAL, monitoring_get_tool__doc__},

static PyObject *
monitoring_get_tool_impl(PyObject *module, int tool_id);

static PyObject *
monitoring_get_tool(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int tool_id;

    tool_id = PyLong_AsInt(arg);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = monitoring_get_tool_impl(module, tool_id);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_register_callback__doc__,
"register_callback($module, tool_id, event, func, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_register_callback_MethodName = {.name = "register_callback",};
#define MONITORING_REGISTER_CALLBACK_METHODDEF    \
    {monitoring_register_callback_MethodName.name, _PyCFunction_CAST(monitoring_register_callback), METH_FASTCALL|METH_C_STACK_FRUGAL, monitoring_register_callback__doc__},

static PyObject *
monitoring_register_callback_impl(PyObject *module, int tool_id, int event,
                                  PyObject *func);

static PyObject *
monitoring_register_callback(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    int tool_id;
    int event;
    PyObject *func;

    if (!_PyArg_CheckPositional("register_callback", nargs, 3, 3)) {
        goto exit;
    }
    tool_id = PyLong_AsInt(args[0]);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    event = PyLong_AsInt(args[1]);
    if (event == -1 && PyErr_Occurred()) {
        goto exit;
    }
    func = args[2];
    return_value = monitoring_register_callback_impl(module, tool_id, event, func);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_get_events__doc__,
"get_events($module, tool_id, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_get_events_MethodName = {.name = "get_events",};
#define MONITORING_GET_EVENTS_METHODDEF    \
    {monitoring_get_events_MethodName.name, (PyCFunction)monitoring_get_events, METH_O|METH_C_STACK_FRUGAL, monitoring_get_events__doc__},

static int
monitoring_get_events_impl(PyObject *module, int tool_id);

static PyObject *
monitoring_get_events(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int tool_id;
    int _return_value;

    tool_id = PyLong_AsInt(arg);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    _return_value = monitoring_get_events_impl(module, tool_id);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyLong_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_set_events__doc__,
"set_events($module, tool_id, event_set, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_set_events_MethodName = {.name = "set_events",};
#define MONITORING_SET_EVENTS_METHODDEF    \
    {monitoring_set_events_MethodName.name, _PyCFunction_CAST(monitoring_set_events), METH_FASTCALL|METH_C_STACK_FRUGAL, monitoring_set_events__doc__},

static PyObject *
monitoring_set_events_impl(PyObject *module, int tool_id, int event_set);

static PyObject *
monitoring_set_events(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    int tool_id;
    int event_set;

    if (!_PyArg_CheckPositional("set_events", nargs, 2, 2)) {
        goto exit;
    }
    tool_id = PyLong_AsInt(args[0]);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    event_set = PyLong_AsInt(args[1]);
    if (event_set == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = monitoring_set_events_impl(module, tool_id, event_set);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_get_local_events__doc__,
"get_local_events($module, tool_id, code, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_get_local_events_MethodName = {.name = "get_local_events",};
#define MONITORING_GET_LOCAL_EVENTS_METHODDEF    \
    {monitoring_get_local_events_MethodName.name, _PyCFunction_CAST(monitoring_get_local_events), METH_FASTCALL|METH_C_STACK_FRUGAL, monitoring_get_local_events__doc__},

static int
monitoring_get_local_events_impl(PyObject *module, int tool_id,
                                 PyObject *code);

static PyObject *
monitoring_get_local_events(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    int tool_id;
    PyObject *code;
    int _return_value;

    if (!_PyArg_CheckPositional("get_local_events", nargs, 2, 2)) {
        goto exit;
    }
    tool_id = PyLong_AsInt(args[0]);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    code = args[1];
    _return_value = monitoring_get_local_events_impl(module, tool_id, code);
    if ((_return_value == -1) && PyErr_Occurred()) {
        goto exit;
    }
    return_value = PyLong_FromLong((long)_return_value);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_set_local_events__doc__,
"set_local_events($module, tool_id, code, event_set, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_set_local_events_MethodName = {.name = "set_local_events",};
#define MONITORING_SET_LOCAL_EVENTS_METHODDEF    \
    {monitoring_set_local_events_MethodName.name, _PyCFunction_CAST(monitoring_set_local_events), METH_FASTCALL|METH_C_STACK_FRUGAL, monitoring_set_local_events__doc__},

static PyObject *
monitoring_set_local_events_impl(PyObject *module, int tool_id,
                                 PyObject *code, int event_set);

static PyObject *
monitoring_set_local_events(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    int tool_id;
    PyObject *code;
    int event_set;

    if (!_PyArg_CheckPositional("set_local_events", nargs, 3, 3)) {
        goto exit;
    }
    tool_id = PyLong_AsInt(args[0]);
    if (tool_id == -1 && PyErr_Occurred()) {
        goto exit;
    }
    code = args[1];
    event_set = PyLong_AsInt(args[2]);
    if (event_set == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = monitoring_set_local_events_impl(module, tool_id, code, event_set);

exit:
    return return_value;
}

PyDoc_STRVAR(monitoring_restart_events__doc__,
"restart_events($module, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring_restart_events_MethodName = {.name = "restart_events",};
#define MONITORING_RESTART_EVENTS_METHODDEF    \
    {monitoring_restart_events_MethodName.name, (PyCFunction)monitoring_restart_events, METH_NOARGS|METH_C_STACK_FRUGAL, monitoring_restart_events__doc__},

static PyObject *
monitoring_restart_events_impl(PyObject *module);

static PyObject *
monitoring_restart_events(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return monitoring_restart_events_impl(module);
}

PyDoc_STRVAR(monitoring__all_events__doc__,
"_all_events($module, /)\n"
"--\n"
"\n");

static struct {char name[];} monitoring__all_events_MethodName = {.name = "_all_events",};
#define MONITORING__ALL_EVENTS_METHODDEF    \
    {monitoring__all_events_MethodName.name, (PyCFunction)monitoring__all_events, METH_NOARGS|METH_C_STACK_FRUGAL, monitoring__all_events__doc__},

static PyObject *
monitoring__all_events_impl(PyObject *module);

static PyObject *
monitoring__all_events(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return monitoring__all_events_impl(module);
}
/*[clinic end generated code: output=7b64feefab25f7e1 input=a9049054013a1b77]*/
