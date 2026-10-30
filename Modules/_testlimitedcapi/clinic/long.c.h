/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(_testlimitedcapi_test_long_api__doc__,
"test_long_api($module, /)\n"
"--\n"
"\n");

static struct {char name[];} _testlimitedcapi_test_long_api_MethodName = {.name = "test_long_api",};
#define _TESTLIMITEDCAPI_TEST_LONG_API_METHODDEF    \
    {_testlimitedcapi_test_long_api_MethodName.name, (PyCFunction)_testlimitedcapi_test_long_api, METH_NOARGS|METH_C_STACK_FRUGAL, _testlimitedcapi_test_long_api__doc__},

static PyObject *
_testlimitedcapi_test_long_api_impl(PyObject *module);

static PyObject *
_testlimitedcapi_test_long_api(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _testlimitedcapi_test_long_api_impl(module);
}

PyDoc_STRVAR(_testlimitedcapi_test_longlong_api__doc__,
"test_longlong_api($module, /)\n"
"--\n"
"\n");

static struct {char name[];} _testlimitedcapi_test_longlong_api_MethodName = {.name = "test_longlong_api",};
#define _TESTLIMITEDCAPI_TEST_LONGLONG_API_METHODDEF    \
    {_testlimitedcapi_test_longlong_api_MethodName.name, (PyCFunction)_testlimitedcapi_test_longlong_api, METH_NOARGS|METH_C_STACK_FRUGAL, _testlimitedcapi_test_longlong_api__doc__},

static PyObject *
_testlimitedcapi_test_longlong_api_impl(PyObject *module);

static PyObject *
_testlimitedcapi_test_longlong_api(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _testlimitedcapi_test_longlong_api_impl(module);
}

PyDoc_STRVAR(_testlimitedcapi_test_long_and_overflow__doc__,
"test_long_and_overflow($module, /)\n"
"--\n"
"\n"
"Test the PyLong_AsLongAndOverflow API.\n"
"\n"
"General conversion to PY_LONG is tested by test_long_api_inner.\n"
"This test will concentrate on proper handling of overflow.");

static struct {char name[];} _testlimitedcapi_test_long_and_overflow_MethodName = {.name = "test_long_and_overflow",};
#define _TESTLIMITEDCAPI_TEST_LONG_AND_OVERFLOW_METHODDEF    \
    {_testlimitedcapi_test_long_and_overflow_MethodName.name, (PyCFunction)_testlimitedcapi_test_long_and_overflow, METH_NOARGS|METH_C_STACK_FRUGAL, _testlimitedcapi_test_long_and_overflow__doc__},

static PyObject *
_testlimitedcapi_test_long_and_overflow_impl(PyObject *module);

static PyObject *
_testlimitedcapi_test_long_and_overflow(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _testlimitedcapi_test_long_and_overflow_impl(module);
}

PyDoc_STRVAR(_testlimitedcapi_test_long_long_and_overflow__doc__,
"test_long_long_and_overflow($module, /)\n"
"--\n"
"\n"
"Test the PyLong_AsLongLongAndOverflow API.\n"
"\n"
"General conversion to long long is tested by test_long_api_inner.\n"
"This test will concentrate on proper handling of overflow.");

static struct {char name[];} _testlimitedcapi_test_long_long_and_overflow_MethodName = {.name = "test_long_long_and_overflow",};
#define _TESTLIMITEDCAPI_TEST_LONG_LONG_AND_OVERFLOW_METHODDEF    \
    {_testlimitedcapi_test_long_long_and_overflow_MethodName.name, (PyCFunction)_testlimitedcapi_test_long_long_and_overflow, METH_NOARGS|METH_C_STACK_FRUGAL, _testlimitedcapi_test_long_long_and_overflow__doc__},

static PyObject *
_testlimitedcapi_test_long_long_and_overflow_impl(PyObject *module);

static PyObject *
_testlimitedcapi_test_long_long_and_overflow(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _testlimitedcapi_test_long_long_and_overflow_impl(module);
}

PyDoc_STRVAR(_testlimitedcapi_test_long_as_size_t__doc__,
"test_long_as_size_t($module, /)\n"
"--\n"
"\n"
"Test the PyLong_As{Size,Ssize}_t API.\n"
"\n"
"At present this just tests that non-integer arguments are handled correctly.\n"
"It should be extended to test overflow handling.");

static struct {char name[];} _testlimitedcapi_test_long_as_size_t_MethodName = {.name = "test_long_as_size_t",};
#define _TESTLIMITEDCAPI_TEST_LONG_AS_SIZE_T_METHODDEF    \
    {_testlimitedcapi_test_long_as_size_t_MethodName.name, (PyCFunction)_testlimitedcapi_test_long_as_size_t, METH_NOARGS|METH_C_STACK_FRUGAL, _testlimitedcapi_test_long_as_size_t__doc__},

static PyObject *
_testlimitedcapi_test_long_as_size_t_impl(PyObject *module);

static PyObject *
_testlimitedcapi_test_long_as_size_t(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _testlimitedcapi_test_long_as_size_t_impl(module);
}

PyDoc_STRVAR(_testlimitedcapi_test_long_as_unsigned_long_long_mask__doc__,
"test_long_as_unsigned_long_long_mask($module, /)\n"
"--\n"
"\n");

static struct {char name[];} _testlimitedcapi_test_long_as_unsigned_long_long_mask_MethodName = {.name = "test_long_as_unsigned_long_long_mask",};
#define _TESTLIMITEDCAPI_TEST_LONG_AS_UNSIGNED_LONG_LONG_MASK_METHODDEF    \
    {_testlimitedcapi_test_long_as_unsigned_long_long_mask_MethodName.name, (PyCFunction)_testlimitedcapi_test_long_as_unsigned_long_long_mask, METH_NOARGS|METH_C_STACK_FRUGAL, _testlimitedcapi_test_long_as_unsigned_long_long_mask__doc__},

static PyObject *
_testlimitedcapi_test_long_as_unsigned_long_long_mask_impl(PyObject *module);

static PyObject *
_testlimitedcapi_test_long_as_unsigned_long_long_mask(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _testlimitedcapi_test_long_as_unsigned_long_long_mask_impl(module);
}

PyDoc_STRVAR(_testlimitedcapi_test_long_as_double__doc__,
"test_long_as_double($module, /)\n"
"--\n"
"\n");

static struct {char name[];} _testlimitedcapi_test_long_as_double_MethodName = {.name = "test_long_as_double",};
#define _TESTLIMITEDCAPI_TEST_LONG_AS_DOUBLE_METHODDEF    \
    {_testlimitedcapi_test_long_as_double_MethodName.name, (PyCFunction)_testlimitedcapi_test_long_as_double, METH_NOARGS|METH_C_STACK_FRUGAL, _testlimitedcapi_test_long_as_double__doc__},

static PyObject *
_testlimitedcapi_test_long_as_double_impl(PyObject *module);

static PyObject *
_testlimitedcapi_test_long_as_double(PyObject *module, PyObject *Py_UNUSED(ignored))
{
    return _testlimitedcapi_test_long_as_double_impl(module);
}

PyDoc_STRVAR(_testlimitedcapi_PyLong_AsInt__doc__,
"PyLong_AsInt($module, arg, /)\n"
"--\n"
"\n");

static struct {char name[];} _testlimitedcapi_PyLong_AsInt_MethodName = {.name = "PyLong_AsInt",};
#define _TESTLIMITEDCAPI_PYLONG_ASINT_METHODDEF    \
    {_testlimitedcapi_PyLong_AsInt_MethodName.name, (PyCFunction)_testlimitedcapi_PyLong_AsInt, METH_O|METH_C_STACK_FRUGAL, _testlimitedcapi_PyLong_AsInt__doc__},
/*[clinic end generated code: output=e3708a03d2e29c31 input=a9049054013a1b77]*/
