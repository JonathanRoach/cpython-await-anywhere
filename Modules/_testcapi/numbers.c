#include "parts.h"
#include "util.h"


static PyObject *
number_check(PyObject *Py_UNUSED(module), PyObject *obj)
{
    NULLABLE(obj);
    return PyLong_FromLong(PyNumber_Check(obj));
}

#define BINARYFUNC(funcsuffix, methsuffix)                           \
    static PyObject *                                                \
    number_##methsuffix(PyObject *Py_UNUSED(module), PyObject *args) \
    {                                                                \
        PyObject *o1, *o2;                                           \
                                                                     \
        if (!PyArg_ParseTuple(args, "OO", &o1, &o2)) {               \
            return NULL;                                             \
        }                                                            \
                                                                     \
        NULLABLE(o1);                                                \
        NULLABLE(o2);                                                \
        return PyNumber_##funcsuffix(o1, o2);                        \
    };

BINARYFUNC(Add, add)
BINARYFUNC(Subtract, subtract)
BINARYFUNC(Multiply, multiply)
BINARYFUNC(MatrixMultiply, matrixmultiply)
BINARYFUNC(FloorDivide, floordivide)
BINARYFUNC(TrueDivide, truedivide)
BINARYFUNC(Remainder, remainder)
BINARYFUNC(Divmod, divmod)

#define TERNARYFUNC(funcsuffix, methsuffix)                          \
    static PyObject *                                                \
    number_##methsuffix(PyObject *Py_UNUSED(module), PyObject *args) \
    {                                                                \
        PyObject *o1, *o2, *o3 = Py_None;                            \
                                                                     \
        if (!PyArg_ParseTuple(args, "OO|O", &o1, &o2, &o3)) {        \
            return NULL;                                             \
        }                                                            \
                                                                     \
        NULLABLE(o1);                                                \
        NULLABLE(o2);                                                \
        return PyNumber_##funcsuffix(o1, o2, o3);                    \
    };

TERNARYFUNC(Power, power)

#define UNARYFUNC(funcsuffix, methsuffix)                            \
    static PyObject *                                                \
    number_##methsuffix(PyObject *Py_UNUSED(module), PyObject *obj)  \
    {                                                                \
        NULLABLE(obj);                                               \
        return PyNumber_##funcsuffix(obj);                           \
    };

UNARYFUNC(Negative, negative)
UNARYFUNC(Positive, positive)
UNARYFUNC(Absolute, absolute)
UNARYFUNC(Invert, invert)

BINARYFUNC(Lshift, lshift)
BINARYFUNC(Rshift, rshift)
BINARYFUNC(And, and)
BINARYFUNC(Xor, xor)
BINARYFUNC(Or, or)

BINARYFUNC(InPlaceAdd, inplaceadd)
BINARYFUNC(InPlaceSubtract, inplacesubtract)
BINARYFUNC(InPlaceMultiply, inplacemultiply)
BINARYFUNC(InPlaceMatrixMultiply, inplacematrixmultiply)
BINARYFUNC(InPlaceFloorDivide, inplacefloordivide)
BINARYFUNC(InPlaceTrueDivide, inplacetruedivide)
BINARYFUNC(InPlaceRemainder, inplaceremainder)

TERNARYFUNC(InPlacePower, inplacepower)

BINARYFUNC(InPlaceLshift, inplacelshift)
BINARYFUNC(InPlaceRshift, inplacershift)
BINARYFUNC(InPlaceAnd, inplaceand)
BINARYFUNC(InPlaceXor, inplacexor)
BINARYFUNC(InPlaceOr, inplaceor)

UNARYFUNC(Long, long)
UNARYFUNC(Float, float)
UNARYFUNC(Index, index)

static PyObject *
number_tobase(PyObject *Py_UNUSED(module), PyObject *args)
{
    PyObject *n;
    int base;

    if (!PyArg_ParseTuple(args, "Oi", &n, &base)) {
        return NULL;
    }

    NULLABLE(n);
    return PyNumber_ToBase(n, base);
}

static PyObject *
number_asssizet(PyObject *Py_UNUSED(module), PyObject *args)
{
    PyObject *o, *exc;
    Py_ssize_t ret;

    if (!PyArg_ParseTuple(args, "OO", &o, &exc)) {
        return NULL;
    }

    NULLABLE(o);
    NULLABLE(exc);
    ret = PyNumber_AsSsize_t(o, exc);

    if (ret == (Py_ssize_t)(-1) && PyErr_Occurred()) {
        return NULL;
    }

    return PyLong_FromSsize_t(ret);
}


static PyMethodDef test_methods[] = {
    {"number_check", number_check, METH_O|METH_C_STACK_FRUGAL},
    {"number_add", number_add, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_subtract", number_subtract, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_multiply", number_multiply, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_matrixmultiply", number_matrixmultiply, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_floordivide", number_floordivide, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_truedivide", number_truedivide, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_remainder", number_remainder, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_divmod", number_divmod, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_power", number_power, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_negative", number_negative, METH_O|METH_C_STACK_FRUGAL},
    {"number_positive", number_positive, METH_O|METH_C_STACK_FRUGAL},
    {"number_absolute", number_absolute, METH_O|METH_C_STACK_FRUGAL},
    {"number_invert", number_invert, METH_O|METH_C_STACK_FRUGAL},
    {"number_lshift", number_lshift, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_rshift", number_rshift, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_and", number_and, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_xor", number_xor, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_or", number_or, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplaceadd", number_inplaceadd, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacesubtract", number_inplacesubtract, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacemultiply", number_inplacemultiply, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacematrixmultiply", number_inplacematrixmultiply, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacefloordivide", number_inplacefloordivide, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacetruedivide", number_inplacetruedivide, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplaceremainder", number_inplaceremainder, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacepower", number_inplacepower, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacelshift", number_inplacelshift, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacershift", number_inplacershift, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplaceand", number_inplaceand, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplacexor", number_inplacexor, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_inplaceor", number_inplaceor, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_long", number_long, METH_O|METH_C_STACK_FRUGAL},
    {"number_float", number_float, METH_O|METH_C_STACK_FRUGAL},
    {"number_index", number_index, METH_O|METH_C_STACK_FRUGAL},
    {"number_tobase", number_tobase, METH_VARARGS|METH_C_STACK_FRUGAL},
    {"number_asssizet", number_asssizet, METH_VARARGS|METH_C_STACK_FRUGAL},
    {NULL},
};

int
_PyTestCapi_Init_Numbers(PyObject *mod)
{
    if (PyModule_AddFunctions(mod, test_methods) < 0) {
        return -1;
    }

    return 0;
}
