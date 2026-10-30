#ifndef Py_INTERNAL_ABSTRACT_H
#define Py_INTERNAL_ABSTRACT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

// Fast inlined version of PyIndex_Check()
static inline int
_PyIndex_Check(PyObject *obj)
{
    PyNumberMethods *tp_as_number = Py_TYPE(obj)->tp_as_number;
    return (tp_as_number != NULL && tp_as_number->nb_index != NULL);
}

// Inlineable unary ops
PyObject * PyNumber_Negative_Inlineable(PyObject *o, struct _PyInterpreterFrame **inlined);
PyObject * PyNumber_Positive_Inlineable(PyObject *o, struct _PyInterpreterFrame **inlined);
PyObject * PyNumber_Invert_Inlineable(PyObject *o, struct _PyInterpreterFrame **inlined);
PyObject * PyNumber_Absolute_Inlineable(PyObject *o, struct _PyInterpreterFrame **inlined);

// Inlineable binary ops
PyObject *_PyNumber_Add_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_And_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_FloorDivide_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_Lshift_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_MatrixMultiply_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_Multiply_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_Remainder_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_Or_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_PowerNoMod_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_Rshift_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_Subtract_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_TrueDivide_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_Xor_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceAdd_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceAnd_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceFloorDivide_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceLshift_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceMatrixMultiply_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceMultiply_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceRemainder_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceOr_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlacePowerNoMod_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceRshift_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceSubtract_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceTrueDivide_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyNumber_InPlaceXor_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);
PyObject *_PyObject_GetItem_Inlineable(PyObject *lhs, PyObject *rhs, struct _PyInterpreterFrame **inlined);

extern int _PyObject_HasLen(PyObject *o);

/* === Sequence protocol ================================================ */

#define PY_ITERSEARCH_COUNT    1
#define PY_ITERSEARCH_INDEX    2
#define PY_ITERSEARCH_CONTAINS 3

/* Iterate over seq.

   Result depends on the operation:

   PY_ITERSEARCH_COUNT:  return # of times obj appears in seq; -1 if
     error.
   PY_ITERSEARCH_INDEX:  return 0-based index of first occurrence of
     obj in seq; set ValueError and return -1 if none found;
     also return -1 on error.
   PY_ITERSEARCH_CONTAINS:  return 1 if obj in seq, else 0; -1 on
     error. */
extern Py_ssize_t _PySequence_IterSearch(PyObject *seq,
                                         PyObject *obj, int operation);

/* === Mapping protocol ================================================= */

extern int _PyObject_RealIsInstance(PyObject *inst, PyObject *cls);

extern int _PyObject_RealIsSubclass(PyObject *derived, PyObject *cls);

// Convert Python int to Py_ssize_t. Do nothing if the argument is None.
// Export for '_bisect' shared extension.
PyAPI_FUNC(int) _Py_convert_optional_to_ssize_t(PyObject *, void *);

// Same as PyNumber_Index() but can return an instance of a subclass of int.
// Export for 'math' shared extension.
PyAPI_FUNC(PyObject*) _PyNumber_Index(PyObject *o);

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTERNAL_ABSTRACT_H */
