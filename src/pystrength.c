#include "pystrength.h"

static PyMethodDef  pyvpi_strengthval_methods[] = {
    {NULL}
};
static PyMemberDef pyvpi_strengthval_members[]  = {
    {"logic",   T_INT,   offsetof(s_pyvpi_strengthval, _vpi_strengthval.logic), 0, "strength logic value."},
    {"s0",      T_INT,   offsetof(s_pyvpi_strengthval, _vpi_strengthval.s0),    0, "strength s0"},
    {"s1",      T_INT,   offsetof(s_pyvpi_strengthval, _vpi_strengthval.s1),    0, "strength s1"},
    {NULL}
};
static PyGetSetDef pyvpi_strengthval_getsets[]  = {
    {NULL}
};

PyTypeObject pyvpi_strengthval_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyvpi._Strength",         /*tp_name*/
    sizeof(s_pyvpi_strengthval),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyvpi_strengthval_Dealloc, /*tp_dealloc*/    
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "pyvpi strengthval objects",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pyvpi_strengthval_methods,       /* tp_methods */
    pyvpi_strengthval_members,       /* tp_members */
    pyvpi_strengthval_getsets,       /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) pyvpi_strengthval_Init,      /* tp_init */
    0,                         /* tp_alloc */
    pyvpi_strengthval_New,           /* tp_new */
};


void pyvpi_strengthval_Dealloc(p_pyvpi_strengthval self)
{
    //Free self.
    pyvpi_verbose("pyvpi._Strengthval is release, "
        "ptr is "FADDR_MARCO".\n",self);
    self->ob_type->tp_free((PyObject*)self);
}

int  pyvpi_strengthval_Init(s_pyvpi_strengthval *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"logic","s0","s1", NULL};    
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist,                                      
                                      &self->_vpi_strengthval.logic,
                                      &self->_vpi_strengthval.s0,
                                      &self->_vpi_strengthval.s1))
        return -1;
    pyvpi_verbose("pyvpi._Strengthval is Initial.\n");
    return 0;
}

PyObject * pyvpi_strengthval_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    p_pyvpi_strengthval self = (p_pyvpi_strengthval)type->tp_alloc(type, 0);
    pyvpi_verbose("pyvpi._Strengthval is allocate,ptr is "FADDR_MARCO", "
        "type ptr is "FADDR_MARCO".\n",self,type);
    return (PyObject *) self;
}
