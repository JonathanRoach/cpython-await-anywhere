/*[clinic input]
preserve
[clinic start generated code]*/

#include "pycore_critical_section.h"// Py_BEGIN_CRITICAL_SECTION()

PyDoc_STRVAR(set_pop__doc__,
"pop($self, /)\n"
"--\n"
"\n"
"Remove and return an arbitrary set element.\n"
"\n"
"Raises KeyError if the set is empty.");

static struct {char name[];} set_pop_MethodName = {.name = "pop",};
#define SET_POP_METHODDEF    \
    {set_pop_MethodName.name, (PyCFunction)set_pop, METH_NOARGS|METH_C_STACK_FRUGAL, set_pop__doc__},

static PyObject *
set_pop_impl(PySetObject *so);

static PyObject *
set_pop(PyObject *so, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set_pop_impl((PySetObject *)so);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(set_update__doc__,
"update($self, /, *others)\n"
"--\n"
"\n"
"Update the set, adding elements from all others.");

static struct {char name[];} set_update_MethodName = {.name = "update",};
#define SET_UPDATE_METHODDEF    \
    {set_update_MethodName.name, _PyCFunction_CAST(set_update), METH_FASTCALL|METH_C_STACK_FRUGAL, set_update__doc__},

static PyObject *
set_update_impl(PySetObject *so, PyObject * const *others,
                Py_ssize_t others_length);

static PyObject *
set_update(PyObject *so, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject * const *others;
    Py_ssize_t others_length;

    others = args;
    others_length = nargs;
    return_value = set_update_impl((PySetObject *)so, others, others_length);

    return return_value;
}

PyDoc_STRVAR(set_copy__doc__,
"copy($self, /)\n"
"--\n"
"\n"
"Return a shallow copy of a set.");

static struct {char name[];} set_copy_MethodName = {.name = "copy",};
#define SET_COPY_METHODDEF    \
    {set_copy_MethodName.name, (PyCFunction)set_copy, METH_NOARGS|METH_C_STACK_FRUGAL, set_copy__doc__},

static PyObject *
set_copy_impl(PySetObject *so);

static PyObject *
set_copy(PyObject *so, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set_copy_impl((PySetObject *)so);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(frozenset_copy__doc__,
"copy($self, /)\n"
"--\n"
"\n"
"Return a shallow copy of a set.");

static struct {char name[];} frozenset_copy_MethodName = {.name = "copy",};
#define FROZENSET_COPY_METHODDEF    \
    {frozenset_copy_MethodName.name, (PyCFunction)frozenset_copy, METH_NOARGS|METH_C_STACK_FRUGAL, frozenset_copy__doc__},

static PyObject *
frozenset_copy_impl(PySetObject *so);

static PyObject *
frozenset_copy(PyObject *so, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = frozenset_copy_impl((PySetObject *)so);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(set_clear__doc__,
"clear($self, /)\n"
"--\n"
"\n"
"Remove all elements from this set.");

static struct {char name[];} set_clear_MethodName = {.name = "clear",};
#define SET_CLEAR_METHODDEF    \
    {set_clear_MethodName.name, (PyCFunction)set_clear, METH_NOARGS|METH_C_STACK_FRUGAL, set_clear__doc__},

static PyObject *
set_clear_impl(PySetObject *so);

static PyObject *
set_clear(PyObject *so, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set_clear_impl((PySetObject *)so);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(set_union__doc__,
"union($self, /, *others)\n"
"--\n"
"\n"
"Return a new set with elements from the set and all others.");

static struct {char name[];} set_union_MethodName = {.name = "union",};
#define SET_UNION_METHODDEF    \
    {set_union_MethodName.name, _PyCFunction_CAST(set_union), METH_FASTCALL|METH_C_STACK_FRUGAL, set_union__doc__},

static PyObject *
set_union_impl(PySetObject *so, PyObject * const *others,
               Py_ssize_t others_length);

static PyObject *
set_union(PyObject *so, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject * const *others;
    Py_ssize_t others_length;

    others = args;
    others_length = nargs;
    return_value = set_union_impl((PySetObject *)so, others, others_length);

    return return_value;
}

PyDoc_STRVAR(set_intersection_multi__doc__,
"intersection($self, /, *others)\n"
"--\n"
"\n"
"Return a new set with elements common to the set and all others.");

static struct {char name[];} set_intersection_multi_MethodName = {.name = "intersection",};
#define SET_INTERSECTION_MULTI_METHODDEF    \
    {set_intersection_multi_MethodName.name, _PyCFunction_CAST(set_intersection_multi), METH_FASTCALL|METH_C_STACK_FRUGAL, set_intersection_multi__doc__},

static PyObject *
set_intersection_multi_impl(PySetObject *so, PyObject * const *others,
                            Py_ssize_t others_length);

static PyObject *
set_intersection_multi(PyObject *so, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject * const *others;
    Py_ssize_t others_length;

    others = args;
    others_length = nargs;
    return_value = set_intersection_multi_impl((PySetObject *)so, others, others_length);

    return return_value;
}

PyDoc_STRVAR(set_intersection_update_multi__doc__,
"intersection_update($self, /, *others)\n"
"--\n"
"\n"
"Update the set, keeping only elements found in it and all others.");

static struct {char name[];} set_intersection_update_multi_MethodName = {.name = "intersection_update",};
#define SET_INTERSECTION_UPDATE_MULTI_METHODDEF    \
    {set_intersection_update_multi_MethodName.name, _PyCFunction_CAST(set_intersection_update_multi), METH_FASTCALL|METH_C_STACK_FRUGAL, set_intersection_update_multi__doc__},

static PyObject *
set_intersection_update_multi_impl(PySetObject *so, PyObject * const *others,
                                   Py_ssize_t others_length);

static PyObject *
set_intersection_update_multi(PyObject *so, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject * const *others;
    Py_ssize_t others_length;

    others = args;
    others_length = nargs;
    return_value = set_intersection_update_multi_impl((PySetObject *)so, others, others_length);

    return return_value;
}

PyDoc_STRVAR(set_isdisjoint__doc__,
"isdisjoint($self, other, /)\n"
"--\n"
"\n"
"Return True if two sets have a null intersection.");

static struct {char name[];} set_isdisjoint_MethodName = {.name = "isdisjoint",};
#define SET_ISDISJOINT_METHODDEF    \
    {set_isdisjoint_MethodName.name, (PyCFunction)set_isdisjoint, METH_O|METH_C_STACK_FRUGAL, set_isdisjoint__doc__},

static PyObject *
set_isdisjoint_impl(PySetObject *so, PyObject *other);

static PyObject *
set_isdisjoint(PyObject *so, PyObject *other)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION2(so, other);
    return_value = set_isdisjoint_impl((PySetObject *)so, other);
    Py_END_CRITICAL_SECTION2();

    return return_value;
}

PyDoc_STRVAR(set_difference_update__doc__,
"difference_update($self, /, *others)\n"
"--\n"
"\n"
"Update the set, removing elements found in others.");

static struct {char name[];} set_difference_update_MethodName = {.name = "difference_update",};
#define SET_DIFFERENCE_UPDATE_METHODDEF    \
    {set_difference_update_MethodName.name, _PyCFunction_CAST(set_difference_update), METH_FASTCALL|METH_C_STACK_FRUGAL, set_difference_update__doc__},

static PyObject *
set_difference_update_impl(PySetObject *so, PyObject * const *others,
                           Py_ssize_t others_length);

static PyObject *
set_difference_update(PyObject *so, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject * const *others;
    Py_ssize_t others_length;

    others = args;
    others_length = nargs;
    return_value = set_difference_update_impl((PySetObject *)so, others, others_length);

    return return_value;
}

PyDoc_STRVAR(set_difference_multi__doc__,
"difference($self, /, *others)\n"
"--\n"
"\n"
"Return a new set with elements in the set that are not in the others.");

static struct {char name[];} set_difference_multi_MethodName = {.name = "difference",};
#define SET_DIFFERENCE_MULTI_METHODDEF    \
    {set_difference_multi_MethodName.name, _PyCFunction_CAST(set_difference_multi), METH_FASTCALL|METH_C_STACK_FRUGAL, set_difference_multi__doc__},

static PyObject *
set_difference_multi_impl(PySetObject *so, PyObject * const *others,
                          Py_ssize_t others_length);

static PyObject *
set_difference_multi(PyObject *so, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *return_value = NULL;
    PyObject * const *others;
    Py_ssize_t others_length;

    others = args;
    others_length = nargs;
    return_value = set_difference_multi_impl((PySetObject *)so, others, others_length);

    return return_value;
}

PyDoc_STRVAR(set_symmetric_difference_update__doc__,
"symmetric_difference_update($self, other, /)\n"
"--\n"
"\n"
"Update the set, keeping only elements found in either set, but not in both.");

static struct {char name[];} set_symmetric_difference_update_MethodName = {.name = "symmetric_difference_update",};
#define SET_SYMMETRIC_DIFFERENCE_UPDATE_METHODDEF    \
    {set_symmetric_difference_update_MethodName.name, (PyCFunction)set_symmetric_difference_update, METH_O|METH_C_STACK_FRUGAL, set_symmetric_difference_update__doc__},

static PyObject *
set_symmetric_difference_update_impl(PySetObject *so, PyObject *other);

static PyObject *
set_symmetric_difference_update(PyObject *so, PyObject *other)
{
    PyObject *return_value = NULL;

    return_value = set_symmetric_difference_update_impl((PySetObject *)so, other);

    return return_value;
}

PyDoc_STRVAR(set_symmetric_difference__doc__,
"symmetric_difference($self, other, /)\n"
"--\n"
"\n"
"Return a new set with elements in either the set or other but not both.");

static struct {char name[];} set_symmetric_difference_MethodName = {.name = "symmetric_difference",};
#define SET_SYMMETRIC_DIFFERENCE_METHODDEF    \
    {set_symmetric_difference_MethodName.name, (PyCFunction)set_symmetric_difference, METH_O|METH_C_STACK_FRUGAL, set_symmetric_difference__doc__},

static PyObject *
set_symmetric_difference_impl(PySetObject *so, PyObject *other);

static PyObject *
set_symmetric_difference(PyObject *so, PyObject *other)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION2(so, other);
    return_value = set_symmetric_difference_impl((PySetObject *)so, other);
    Py_END_CRITICAL_SECTION2();

    return return_value;
}

PyDoc_STRVAR(set_issubset__doc__,
"issubset($self, other, /)\n"
"--\n"
"\n"
"Report whether another set contains this set.");

static struct {char name[];} set_issubset_MethodName = {.name = "issubset",};
#define SET_ISSUBSET_METHODDEF    \
    {set_issubset_MethodName.name, (PyCFunction)set_issubset, METH_O|METH_C_STACK_FRUGAL, set_issubset__doc__},

static PyObject *
set_issubset_impl(PySetObject *so, PyObject *other);

static PyObject *
set_issubset(PyObject *so, PyObject *other)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION2(so, other);
    return_value = set_issubset_impl((PySetObject *)so, other);
    Py_END_CRITICAL_SECTION2();

    return return_value;
}

PyDoc_STRVAR(set_issuperset__doc__,
"issuperset($self, other, /)\n"
"--\n"
"\n"
"Report whether this set contains another set.");

static struct {char name[];} set_issuperset_MethodName = {.name = "issuperset",};
#define SET_ISSUPERSET_METHODDEF    \
    {set_issuperset_MethodName.name, (PyCFunction)set_issuperset, METH_O|METH_C_STACK_FRUGAL, set_issuperset__doc__},

static PyObject *
set_issuperset_impl(PySetObject *so, PyObject *other);

static PyObject *
set_issuperset(PyObject *so, PyObject *other)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION2(so, other);
    return_value = set_issuperset_impl((PySetObject *)so, other);
    Py_END_CRITICAL_SECTION2();

    return return_value;
}

PyDoc_STRVAR(set_add__doc__,
"add($self, object, /)\n"
"--\n"
"\n"
"Add an element to a set.\n"
"\n"
"This has no effect if the element is already present.");

static struct {char name[];} set_add_MethodName = {.name = "add",};
#define SET_ADD_METHODDEF    \
    {set_add_MethodName.name, (PyCFunction)set_add, METH_O|METH_C_STACK_FRUGAL, set_add__doc__},

static PyObject *
set_add_impl(PySetObject *so, PyObject *key);

static PyObject *
set_add(PyObject *so, PyObject *key)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set_add_impl((PySetObject *)so, key);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(set___contains____doc__,
"__contains__($self, object, /)\n"
"--\n"
"\n"
"x.__contains__(y) <==> y in x.");

static struct {char name[];} set___contains___MethodName = {.name = "__contains__",};
#define SET___CONTAINS___METHODDEF    \
    {set___contains___MethodName.name, (PyCFunction)set___contains__, METH_O|METH_COEXIST|METH_C_STACK_FRUGAL, set___contains____doc__},

static PyObject *
set___contains___impl(PySetObject *so, PyObject *key);

static PyObject *
set___contains__(PyObject *so, PyObject *key)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set___contains___impl((PySetObject *)so, key);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(frozenset___contains____doc__,
"__contains__($self, object, /)\n"
"--\n"
"\n"
"x.__contains__(y) <==> y in x.");

static struct {char name[];} frozenset___contains___MethodName = {.name = "__contains__",};
#define FROZENSET___CONTAINS___METHODDEF    \
    {frozenset___contains___MethodName.name, (PyCFunction)frozenset___contains__, METH_O|METH_COEXIST|METH_C_STACK_FRUGAL, frozenset___contains____doc__},

static PyObject *
frozenset___contains___impl(PySetObject *so, PyObject *key);

static PyObject *
frozenset___contains__(PyObject *so, PyObject *key)
{
    PyObject *return_value = NULL;

    return_value = frozenset___contains___impl((PySetObject *)so, key);

    return return_value;
}

PyDoc_STRVAR(set_remove__doc__,
"remove($self, object, /)\n"
"--\n"
"\n"
"Remove an element from a set; it must be a member.\n"
"\n"
"If the element is not a member, raise a KeyError.");

static struct {char name[];} set_remove_MethodName = {.name = "remove",};
#define SET_REMOVE_METHODDEF    \
    {set_remove_MethodName.name, (PyCFunction)set_remove, METH_O|METH_C_STACK_FRUGAL, set_remove__doc__},

static PyObject *
set_remove_impl(PySetObject *so, PyObject *key);

static PyObject *
set_remove(PyObject *so, PyObject *key)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set_remove_impl((PySetObject *)so, key);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(set_discard__doc__,
"discard($self, object, /)\n"
"--\n"
"\n"
"Remove an element from a set if it is a member.\n"
"\n"
"Unlike set.remove(), the discard() method does not raise\n"
"an exception when an element is missing from the set.");

static struct {char name[];} set_discard_MethodName = {.name = "discard",};
#define SET_DISCARD_METHODDEF    \
    {set_discard_MethodName.name, (PyCFunction)set_discard, METH_O|METH_C_STACK_FRUGAL, set_discard__doc__},

static PyObject *
set_discard_impl(PySetObject *so, PyObject *key);

static PyObject *
set_discard(PyObject *so, PyObject *key)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set_discard_impl((PySetObject *)so, key);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(set___reduce____doc__,
"__reduce__($self, /)\n"
"--\n"
"\n"
"Return state information for pickling.");

static struct {char name[];} set___reduce___MethodName = {.name = "__reduce__",};
#define SET___REDUCE___METHODDEF    \
    {set___reduce___MethodName.name, (PyCFunction)set___reduce__, METH_NOARGS|METH_C_STACK_FRUGAL, set___reduce____doc__},

static PyObject *
set___reduce___impl(PySetObject *so);

static PyObject *
set___reduce__(PyObject *so, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set___reduce___impl((PySetObject *)so);
    Py_END_CRITICAL_SECTION();

    return return_value;
}

PyDoc_STRVAR(set___sizeof____doc__,
"__sizeof__($self, /)\n"
"--\n"
"\n"
"S.__sizeof__() -> size of S in memory, in bytes.");

static struct {char name[];} set___sizeof___MethodName = {.name = "__sizeof__",};
#define SET___SIZEOF___METHODDEF    \
    {set___sizeof___MethodName.name, (PyCFunction)set___sizeof__, METH_NOARGS|METH_C_STACK_FRUGAL, set___sizeof____doc__},

static PyObject *
set___sizeof___impl(PySetObject *so);

static PyObject *
set___sizeof__(PyObject *so, PyObject *Py_UNUSED(ignored))
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(so);
    return_value = set___sizeof___impl((PySetObject *)so);
    Py_END_CRITICAL_SECTION();

    return return_value;
}
/*[clinic end generated code: output=088d23de33658481 input=a9049054013a1b77]*/
