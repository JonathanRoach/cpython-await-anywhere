#ifndef Py_INTERNAL_DESCROBJECT_H
#define Py_INTERNAL_DESCROBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_BUILD_CORE
#  error "this header requires Py_BUILD_CORE define"
#endif

typedef struct {
    PyObject_HEAD
    PyObject *prop_get;
    PyObject *prop_set;
    PyObject *prop_del;
    PyObject *prop_doc;
    PyObject *prop_name;
    int getter_doc;
} propertyobject;

typedef propertyobject _PyPropertyObject;

extern PyTypeObject _PyMethodWrapper_Type;

extern PyObject *_PyProperty_Slot_tp_descr_get_inlinable(PyObject *self, PyObject *obj, PyObject *type, struct _PyInterpreterFrame **inlined);
extern PyObject *_PyProperty_Slot_tp_descr_get(PyObject *self, PyObject *obj, PyObject *type);
extern int _PyProperty_Slot_tp_descr_set_inlinable(PyObject *self, PyObject *obj, PyObject *value, struct _PyInterpreterFrame **inlined);
extern int _PyProperty_Slot_tp_descr_set(PyObject *self, PyObject *obj, PyObject *value);

#ifdef __cplusplus
}
#endif
#endif   /* !Py_INTERNAL_DESCROBJECT_H */
