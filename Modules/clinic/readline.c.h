/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_critical_section.h"// Py_BEGIN_CRITICAL_SECTION()
#include "pycore_modsupport.h"    // _PyArg_CheckPositional()

PyDoc_STRVAR(readline_parse_and_bind__doc__,
"parse_and_bind($module, string, /)\n"
"--\n"
"\n"
"Execute the init line provided in the string argument.");

static struct {char name[];} readline_parse_and_bind_MethodName = {.name = "parse_and_bind",};
#define READLINE_PARSE_AND_BIND_METHODDEF    \
    {readline_parse_and_bind_MethodName.name, (PyCFunction)readline_parse_and_bind, METH_O|METH_C_STACK_FRUGAL, readline_parse_and_bind__doc__},

static PyObject *
readline_parse_and_bind_impl(PyObject *module, PyObject *string);

static PyObject *
readline_parse_and_bind(PyObject *module, PyObject *string)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_parse_and_bind_impl(module, string);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(readline_read_init_file__doc__,
"read_init_file($module, filename=None, /)\n"
"--\n"
"\n"
"Execute a readline initialization file.\n"
"\n"
"The default filename is the last filename used.");

static struct {char name[];} readline_read_init_file_MethodName = {.name = "read_init_file",};
#define READLINE_READ_INIT_FILE_METHODDEF    \
    {readline_read_init_file_MethodName.name, _PyCFunction_CAST(readline_read_init_file), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_read_init_file__doc__},

static PyObject *
readline_read_init_file_impl(PyObject *module, PyObject *filename_obj);

static PyObject *
readline_read_init_file(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *filename_obj = Py_None;

    if (!_PyArg_CheckPositional("read_init_file", nargs, 0, 1)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    filename_obj = args[0];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_read_init_file_impl(module, filename_obj);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(readline_read_history_file__doc__,
"read_history_file($module, filename=None, /)\n"
"--\n"
"\n"
"Load a readline history file.\n"
"\n"
"The default filename is ~/.history.");

static struct {char name[];} readline_read_history_file_MethodName = {.name = "read_history_file",};
#define READLINE_READ_HISTORY_FILE_METHODDEF    \
    {readline_read_history_file_MethodName.name, _PyCFunction_CAST(readline_read_history_file), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_read_history_file__doc__},

static PyObject *
readline_read_history_file_impl(PyObject *module, PyObject *filename_obj);

static PyObject *
readline_read_history_file(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *filename_obj = Py_None;

    if (!_PyArg_CheckPositional("read_history_file", nargs, 0, 1)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    filename_obj = args[0];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_read_history_file_impl(module, filename_obj);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(readline_write_history_file__doc__,
"write_history_file($module, filename=None, /)\n"
"--\n"
"\n"
"Save a readline history file.\n"
"\n"
"The default filename is ~/.history.");

static struct {char name[];} readline_write_history_file_MethodName = {.name = "write_history_file",};
#define READLINE_WRITE_HISTORY_FILE_METHODDEF    \
    {readline_write_history_file_MethodName.name, _PyCFunction_CAST(readline_write_history_file), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_write_history_file__doc__},

static PyObject *
readline_write_history_file_impl(PyObject *module, PyObject *filename_obj);

static PyObject *
readline_write_history_file(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *filename_obj = Py_None;

    if (!_PyArg_CheckPositional("write_history_file", nargs, 0, 1)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    filename_obj = args[0];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_write_history_file_impl(module, filename_obj);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

#if defined(HAVE_RL_APPEND_HISTORY)

PyDoc_STRVAR(readline_append_history_file__doc__,
"append_history_file($module, nelements, filename=None, /)\n"
"--\n"
"\n"
"Append the last nelements items of the history list to file.\n"
"\n"
"The default filename is ~/.history.");

static struct {char name[];} readline_append_history_file_MethodName = {.name = "append_history_file",};
#define READLINE_APPEND_HISTORY_FILE_METHODDEF    \
    {readline_append_history_file_MethodName.name, _PyCFunction_CAST(readline_append_history_file), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_append_history_file__doc__},

static PyObject *
readline_append_history_file_impl(PyObject *module, int nelements,
                                  PyObject *filename_obj);

static PyObject *
readline_append_history_file(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    int nelements;
    PyObject *filename_obj = Py_None;

    if (!_PyArg_CheckPositional("append_history_file", nargs, 1, 2)) {
        goto exit;
    }
    nelements = PyLong_AsInt(args[0]);
    if (nelements == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (nargs < 2) {
        goto skip_optional;
    }
    filename_obj = args[1];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_append_history_file_impl(module, nelements, filename_obj);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

#endif /* defined(HAVE_RL_APPEND_HISTORY) */

PyDoc_STRVAR(readline_set_history_length__doc__,
"set_history_length($module, length, /)\n"
"--\n"
"\n"
"Set the maximal number of lines which will be written to the history file.\n"
"\n"
"A negative length is used to inhibit history truncation.");

static struct {char name[];} readline_set_history_length_MethodName = {.name = "set_history_length",};
#define READLINE_SET_HISTORY_LENGTH_METHODDEF    \
    {readline_set_history_length_MethodName.name, (PyCFunction)readline_set_history_length, METH_O|METH_C_STACK_FRUGAL, readline_set_history_length__doc__},

static PyObject *
readline_set_history_length_impl(PyObject *module, int length);

static PyObject *
readline_set_history_length(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int length;

    length = PyLong_AsInt(arg);
    if (length == -1 && PyErr_Occurred()) {
        goto exit;
    }
    return_value = readline_set_history_length_impl(module, length);

exit:
    return return_value;
}

PyDoc_STRVAR(readline_get_history_length__doc__,
"get_history_length($module, /)\n"
"--\n"
"\n"
"Return the maximum number of lines that will be written to the history file.");

static struct {char name[];} readline_get_history_length_MethodName = {.name = "get_history_length",};
#define READLINE_GET_HISTORY_LENGTH_METHODDEF    \
    {readline_get_history_length_MethodName.name, (PyCFunction)readline_get_history_length, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_history_length__doc__},

static PyObject *
readline_get_history_length_impl(PyObject *module);

static PyObject *
readline_get_history_length(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return readline_get_history_length_impl(module);
}

PyDoc_STRVAR(readline_set_completion_display_matches_hook__doc__,
"set_completion_display_matches_hook($module, function=None, /)\n"
"--\n"
"\n"
"Set or remove the completion display function.\n"
"\n"
"The function is called as\n"
"  function(substitution, [matches], longest_match_length)\n"
"once each time matches need to be displayed.");

static struct {char name[];} readline_set_completion_display_matches_hook_MethodName = {.name = "set_completion_display_matches_hook",};
#define READLINE_SET_COMPLETION_DISPLAY_MATCHES_HOOK_METHODDEF    \
    {readline_set_completion_display_matches_hook_MethodName.name, _PyCFunction_CAST(readline_set_completion_display_matches_hook), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_set_completion_display_matches_hook__doc__},

static PyObject *
readline_set_completion_display_matches_hook_impl(PyObject *module,
                                                  PyObject *function);

static PyObject *
readline_set_completion_display_matches_hook(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *function = Py_None;

    if (!_PyArg_CheckPositional("set_completion_display_matches_hook", nargs, 0, 1)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    function = args[0];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_set_completion_display_matches_hook_impl(module, function);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(readline_set_startup_hook__doc__,
"set_startup_hook($module, function=None, /)\n"
"--\n"
"\n"
"Set or remove the function invoked by the rl_startup_hook callback.\n"
"\n"
"The function is called with no arguments just\n"
"before readline prints the first prompt.");

static struct {char name[];} readline_set_startup_hook_MethodName = {.name = "set_startup_hook",};
#define READLINE_SET_STARTUP_HOOK_METHODDEF    \
    {readline_set_startup_hook_MethodName.name, _PyCFunction_CAST(readline_set_startup_hook), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_set_startup_hook__doc__},

static PyObject *
readline_set_startup_hook_impl(PyObject *module, PyObject *function);

static PyObject *
readline_set_startup_hook(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *function = Py_None;

    if (!_PyArg_CheckPositional("set_startup_hook", nargs, 0, 1)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    function = args[0];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_set_startup_hook_impl(module, function);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

#if defined(HAVE_RL_PRE_INPUT_HOOK)

PyDoc_STRVAR(readline_set_pre_input_hook__doc__,
"set_pre_input_hook($module, function=None, /)\n"
"--\n"
"\n"
"Set or remove the function invoked by the rl_pre_input_hook callback.\n"
"\n"
"The function is called with no arguments after the first prompt\n"
"has been printed and just before readline starts reading input\n"
"characters.");

static struct {char name[];} readline_set_pre_input_hook_MethodName = {.name = "set_pre_input_hook",};
#define READLINE_SET_PRE_INPUT_HOOK_METHODDEF    \
    {readline_set_pre_input_hook_MethodName.name, _PyCFunction_CAST(readline_set_pre_input_hook), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_set_pre_input_hook__doc__},

static PyObject *
readline_set_pre_input_hook_impl(PyObject *module, PyObject *function);

static PyObject *
readline_set_pre_input_hook(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *function = Py_None;

    if (!_PyArg_CheckPositional("set_pre_input_hook", nargs, 0, 1)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    function = args[0];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_set_pre_input_hook_impl(module, function);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

#endif /* defined(HAVE_RL_PRE_INPUT_HOOK) */

PyDoc_STRVAR(readline_get_completion_type__doc__,
"get_completion_type($module, /)\n"
"--\n"
"\n"
"Get the type of completion being attempted.");

static struct {char name[];} readline_get_completion_type_MethodName = {.name = "get_completion_type",};
#define READLINE_GET_COMPLETION_TYPE_METHODDEF    \
    {readline_get_completion_type_MethodName.name, (PyCFunction)readline_get_completion_type, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_completion_type__doc__},

static PyObject *
readline_get_completion_type_impl(PyObject *module);

static PyObject *
readline_get_completion_type(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return readline_get_completion_type_impl(module);
}

PyDoc_STRVAR(readline_get_begidx__doc__,
"get_begidx($module, /)\n"
"--\n"
"\n"
"Get the beginning index of the completion scope.");

static struct {char name[];} readline_get_begidx_MethodName = {.name = "get_begidx",};
#define READLINE_GET_BEGIDX_METHODDEF    \
    {readline_get_begidx_MethodName.name, (PyCFunction)readline_get_begidx, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_begidx__doc__},

static PyObject *
readline_get_begidx_impl(PyObject *module);

static PyObject *
readline_get_begidx(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return readline_get_begidx_impl(module);
}

PyDoc_STRVAR(readline_get_endidx__doc__,
"get_endidx($module, /)\n"
"--\n"
"\n"
"Get the ending index of the completion scope.");

static struct {char name[];} readline_get_endidx_MethodName = {.name = "get_endidx",};
#define READLINE_GET_ENDIDX_METHODDEF    \
    {readline_get_endidx_MethodName.name, (PyCFunction)readline_get_endidx, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_endidx__doc__},

static PyObject *
readline_get_endidx_impl(PyObject *module);

static PyObject *
readline_get_endidx(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return readline_get_endidx_impl(module);
}

PyDoc_STRVAR(readline_set_completer_delims__doc__,
"set_completer_delims($module, string, /)\n"
"--\n"
"\n"
"Set the word delimiters for completion.");

static struct {char name[];} readline_set_completer_delims_MethodName = {.name = "set_completer_delims",};
#define READLINE_SET_COMPLETER_DELIMS_METHODDEF    \
    {readline_set_completer_delims_MethodName.name, (PyCFunction)readline_set_completer_delims, METH_O|METH_C_STACK_FRUGAL, readline_set_completer_delims__doc__},

static PyObject *
readline_set_completer_delims_impl(PyObject *module, PyObject *string);

static PyObject *
readline_set_completer_delims(PyObject *module, PyObject *string)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_set_completer_delims_impl(module, string);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(readline_remove_history_item__doc__,
"remove_history_item($module, pos, /)\n"
"--\n"
"\n"
"Remove history item given by its zero-based position.");

static struct {char name[];} readline_remove_history_item_MethodName = {.name = "remove_history_item",};
#define READLINE_REMOVE_HISTORY_ITEM_METHODDEF    \
    {readline_remove_history_item_MethodName.name, (PyCFunction)readline_remove_history_item, METH_O|METH_C_STACK_FRUGAL, readline_remove_history_item__doc__},

static PyObject *
readline_remove_history_item_impl(PyObject *module, int entry_number);

static PyObject *
readline_remove_history_item(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int entry_number;

    entry_number = PyLong_AsInt(arg);
    if (entry_number == -1 && PyErr_Occurred()) {
        goto exit;
    }
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_remove_history_item_impl(module, entry_number);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(readline_replace_history_item__doc__,
"replace_history_item($module, pos, line, /)\n"
"--\n"
"\n"
"Replaces history item given by its position with contents of line.\n"
"\n"
"pos is zero-based.");

static struct {char name[];} readline_replace_history_item_MethodName = {.name = "replace_history_item",};
#define READLINE_REPLACE_HISTORY_ITEM_METHODDEF    \
    {readline_replace_history_item_MethodName.name, _PyCFunction_CAST(readline_replace_history_item), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_replace_history_item__doc__},

static PyObject *
readline_replace_history_item_impl(PyObject *module, int entry_number,
                                   PyObject *line);

static PyObject *
readline_replace_history_item(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    int entry_number;
    PyObject *line;

    if (!_PyArg_CheckPositional("replace_history_item", nargs, 2, 2)) {
        goto exit;
    }
    entry_number = PyLong_AsInt(args[0]);
    if (entry_number == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (!PyUnicode_Check(args[1])) {
        _PyArg_BadArgument("replace_history_item", "argument 2", "str", args[1]);
        goto exit;
    }
    line = args[1];
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_replace_history_item_impl(module, entry_number, line);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(readline_add_history__doc__,
"add_history($module, string, /)\n"
"--\n"
"\n"
"Add an item to the history buffer.");

static struct {char name[];} readline_add_history_MethodName = {.name = "add_history",};
#define READLINE_ADD_HISTORY_METHODDEF    \
    {readline_add_history_MethodName.name, (PyCFunction)readline_add_history, METH_O|METH_C_STACK_FRUGAL, readline_add_history__doc__},

static PyObject *
readline_add_history_impl(PyObject *module, PyObject *string);

static PyObject *
readline_add_history(PyObject *module, PyObject *string)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_add_history_impl(module, string);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(readline_set_auto_history__doc__,
"set_auto_history($module, enabled, /)\n"
"--\n"
"\n"
"Enables or disables automatic history.");

static struct {char name[];} readline_set_auto_history_MethodName = {.name = "set_auto_history",};
#define READLINE_SET_AUTO_HISTORY_METHODDEF    \
    {readline_set_auto_history_MethodName.name, (PyCFunction)readline_set_auto_history, METH_O|METH_C_STACK_FRUGAL, readline_set_auto_history__doc__},

static PyObject *
readline_set_auto_history_impl(PyObject *module,
                               int _should_auto_add_history);

static PyObject *
readline_set_auto_history(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int _should_auto_add_history;

    _should_auto_add_history = PyObject_IsTrue(arg);
    if (_should_auto_add_history < 0) {
        goto exit;
    }
    return_value = readline_set_auto_history_impl(module, _should_auto_add_history);

exit:
    return return_value;
}

PyDoc_STRVAR(readline_get_completer_delims__doc__,
"get_completer_delims($module, /)\n"
"--\n"
"\n"
"Get the word delimiters for completion.");

static struct {char name[];} readline_get_completer_delims_MethodName = {.name = "get_completer_delims",};
#define READLINE_GET_COMPLETER_DELIMS_METHODDEF    \
    {readline_get_completer_delims_MethodName.name, (PyCFunction)readline_get_completer_delims, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_completer_delims__doc__},

static PyObject *
readline_get_completer_delims_impl(PyObject *module);

static PyObject *
readline_get_completer_delims(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_get_completer_delims_impl(module);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(readline_set_completer__doc__,
"set_completer($module, function=None, /)\n"
"--\n"
"\n"
"Set or remove the completer function.\n"
"\n"
"The function is called as function(text, state),\n"
"for state in 0, 1, 2, ..., until it returns a non-string.\n"
"It should return the next possible completion starting with \'text\'.");

static struct {char name[];} readline_set_completer_MethodName = {.name = "set_completer",};
#define READLINE_SET_COMPLETER_METHODDEF    \
    {readline_set_completer_MethodName.name, _PyCFunction_CAST(readline_set_completer), METH_FASTCALL|METH_C_STACK_FRUGAL, readline_set_completer__doc__},

static PyObject *
readline_set_completer_impl(PyObject *module, PyObject *function);

static PyObject *
readline_set_completer(PyObject *module, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject *function = Py_None;

    if (!_PyArg_CheckPositional("set_completer", nargs, 0, 1)) {
        goto exit;
    }
    if (nargs < 1) {
        goto skip_optional;
    }
    function = args[0];
skip_optional:
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_set_completer_impl(module, function);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(readline_get_completer__doc__,
"get_completer($module, /)\n"
"--\n"
"\n"
"Get the current completer function.");

static struct {char name[];} readline_get_completer_MethodName = {.name = "get_completer",};
#define READLINE_GET_COMPLETER_METHODDEF    \
    {readline_get_completer_MethodName.name, (PyCFunction)readline_get_completer, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_completer__doc__},

static PyObject *
readline_get_completer_impl(PyObject *module);

static PyObject *
readline_get_completer(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return readline_get_completer_impl(module);
}

PyDoc_STRVAR(readline_get_history_item__doc__,
"get_history_item($module, index, /)\n"
"--\n"
"\n"
"Return the current contents of history item at one-based index.");

static struct {char name[];} readline_get_history_item_MethodName = {.name = "get_history_item",};
#define READLINE_GET_HISTORY_ITEM_METHODDEF    \
    {readline_get_history_item_MethodName.name, (PyCFunction)readline_get_history_item, METH_O|METH_C_STACK_FRUGAL, readline_get_history_item__doc__},

static PyObject *
readline_get_history_item_impl(PyObject *module, int idx);

static PyObject *
readline_get_history_item(PyObject *module, PyObject *arg)
{
    PyObject *return_value = NULL;
    int idx;

    idx = PyLong_AsInt(arg);
    if (idx == -1 && PyErr_Occurred()) {
        goto exit;
    }
    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_get_history_item_impl(module, idx);
    Py_END_CRITICAL_SECTION();

exit:
    return return_value;
}

PyDoc_STRVAR(readline_get_current_history_length__doc__,
"get_current_history_length($module, /)\n"
"--\n"
"\n"
"Return the current (not the maximum) length of history.");

static struct {char name[];} readline_get_current_history_length_MethodName = {.name = "get_current_history_length",};
#define READLINE_GET_CURRENT_HISTORY_LENGTH_METHODDEF    \
    {readline_get_current_history_length_MethodName.name, (PyCFunction)readline_get_current_history_length, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_current_history_length__doc__},

static PyObject *
readline_get_current_history_length_impl(PyObject *module);

static PyObject *
readline_get_current_history_length(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_get_current_history_length_impl(module);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(readline_get_line_buffer__doc__,
"get_line_buffer($module, /)\n"
"--\n"
"\n"
"Return the current contents of the line buffer.");

static struct {char name[];} readline_get_line_buffer_MethodName = {.name = "get_line_buffer",};
#define READLINE_GET_LINE_BUFFER_METHODDEF    \
    {readline_get_line_buffer_MethodName.name, (PyCFunction)readline_get_line_buffer, METH_NOARGS|METH_C_STACK_FRUGAL, readline_get_line_buffer__doc__},

static PyObject *
readline_get_line_buffer_impl(PyObject *module);

static PyObject *
readline_get_line_buffer(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_get_line_buffer_impl(module);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

#if defined(HAVE_RL_COMPLETION_APPEND_CHARACTER)

PyDoc_STRVAR(readline_clear_history__doc__,
"clear_history($module, /)\n"
"--\n"
"\n"
"Clear the current readline history.");

static struct {char name[];} readline_clear_history_MethodName = {.name = "clear_history",};
#define READLINE_CLEAR_HISTORY_METHODDEF    \
    {readline_clear_history_MethodName.name, (PyCFunction)readline_clear_history, METH_NOARGS|METH_C_STACK_FRUGAL, readline_clear_history__doc__},

static PyObject *
readline_clear_history_impl(PyObject *module);

static PyObject *
readline_clear_history(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_clear_history_impl(module);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

#endif /* defined(HAVE_RL_COMPLETION_APPEND_CHARACTER) */

PyDoc_STRVAR(readline_insert_text__doc__,
"insert_text($module, string, /)\n"
"--\n"
"\n"
"Insert text into the line buffer at the cursor position.");

static struct {char name[];} readline_insert_text_MethodName = {.name = "insert_text",};
#define READLINE_INSERT_TEXT_METHODDEF    \
    {readline_insert_text_MethodName.name, (PyCFunction)readline_insert_text, METH_O|METH_C_STACK_FRUGAL, readline_insert_text__doc__},

static PyObject *
readline_insert_text_impl(PyObject *module, PyObject *string);

static PyObject *
readline_insert_text(PyObject *module, PyObject *string)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_insert_text_impl(module, string);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(readline_redisplay__doc__,
"redisplay($module, /)\n"
"--\n"
"\n"
"Change what\'s displayed on the screen to reflect contents of the line buffer.");

static struct {char name[];} readline_redisplay_MethodName = {.name = "redisplay",};
#define READLINE_REDISPLAY_METHODDEF    \
    {readline_redisplay_MethodName.name, (PyCFunction)readline_redisplay, METH_NOARGS|METH_C_STACK_FRUGAL, readline_redisplay__doc__},

static PyObject *
readline_redisplay_impl(PyObject *module);

static PyObject *
readline_redisplay(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(module);
    return_value = readline_redisplay_impl(module);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

#ifndef READLINE_APPEND_HISTORY_FILE_METHODDEF
    #define READLINE_APPEND_HISTORY_FILE_METHODDEF
#endif /* !defined(READLINE_APPEND_HISTORY_FILE_METHODDEF) */

#ifndef READLINE_SET_PRE_INPUT_HOOK_METHODDEF
    #define READLINE_SET_PRE_INPUT_HOOK_METHODDEF
#endif /* !defined(READLINE_SET_PRE_INPUT_HOOK_METHODDEF) */

#ifndef READLINE_CLEAR_HISTORY_METHODDEF
    #define READLINE_CLEAR_HISTORY_METHODDEF
#endif /* !defined(READLINE_CLEAR_HISTORY_METHODDEF) */
/*[clinic end generated code: output=9a3167f3470a0099 input=a9049054013a1b77]*/
