#include "pydelays.h"

static PyMethodDef  pyvpi_delays_methods[] = {
    {NULL}
};
static PyMemberDef pyvpi_delays_members[]  = {
//    {"no_of_delays",    T_INT,  offsetof(s_pyvpi_delays, _vpi_delay.no_of_delays),   0, "number of delays"},
    {"time_type",       T_INT,  offsetof(s_pyvpi_delays, _vpi_delay.time_type),      0, "[vpiScaledRealTime, vpiSimTime,vpiSuppressTime]"},
    {"mtm_flag",        T_INT,  offsetof(s_pyvpi_delays, _vpi_delay.mtm_flag),       0, "true for mtm values"},
    {"append_flag",     T_INT,  offsetof(s_pyvpi_delays, _vpi_delay.append_flag),    0, "true for append"},
    {"pulsere_flag",    T_INT,  offsetof(s_pyvpi_delays, _vpi_delay.pulsere_flag),   0, "true for pulsere values"},
    {NULL}
};
static PyGetSetDef pyvpi_delays_getsets[]  = {
    {"delays", (getter)s_pyvpi_delays_getdelays, 
    (setter)s_pyvpi_delays_setdelays,"get/set delays.",NULL},
    {NULL}
};

PyTypeObject pyvpi_delays_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyvpi.Delay",              /*tp_name*/
    sizeof(s_pyvpi_delays),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyvpi_delays_Dealloc, /*tp_dealloc*/    
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
    "pyvpi delays objects",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pyvpi_delays_methods,       /* tp_methods */
    pyvpi_delays_members,       /* tp_members */
    pyvpi_delays_getsets,       /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) pyvpi_delays_Init, /* tp_init */
    0,                         /* tp_alloc */
    pyvpi_delays_New,           /* tp_new */
};


void pyvpi_delays_Dealloc(p_pyvpi_delays self)
{
    //Free self.
    pyvpi_verbose("pyvpi.Delay is release, "
        "ptr is "FADDR_MARCO".\n",self);
    Py_DECREF(self->pdelays);
    self->ob_type->tp_free((PyObject*)self);
}

int  pyvpi_delays_Init(s_pyvpi_delays *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"delays","time_type","mtm_flag","append_flag","pulsere_flag", NULL};
    int i;
    PyObject*   tpl = NULL;
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|Oiiii", kwlist,                                      
                                      &tpl,
                                      &self->_vpi_delay.time_type,
                                      &self->_vpi_delay.mtm_flag,
                                      &self->_vpi_delay.append_flag,
                                      &self->_vpi_delay.pulsere_flag
                                      ))
        return -1;   
    if(tpl != NULL && PyTuple_Check(tpl) && PyTuple_Size(tpl)) {
        self->_vpi_delay.no_of_delays  = PyTuple_Size(tpl);
        //Must check all items in tuple is pyvpi.Time
        for(i=0; i < self->_vpi_delay.no_of_delays;i++){
            if(!PyObject_TypeCheck(PyTuple_GetItem(tpl,i) ,&pyvpi_time_Type)){
                PyErr_Format(PyExc_TypeError,  "Error delays in %d, all items in delays must be pyvpi.Time.", i);
                self->pdelays =   PyTuple_New(0);
                self->_vpi_delay.no_of_delays = 0;
                return -1;
            }
        }
        self->_vpi_delay.da = (p_vpi_time) malloc(self->_vpi_delay.no_of_delays * sizeof(s_vpi_time));  //Allocate memory.
        //Copy all value to da;
        for(i=0; i< self->_vpi_delay.no_of_delays; i++){
            p_pyvpi_time pv = (p_pyvpi_time) PyTuple_GetItem(tpl,i);
            self->_vpi_delay.da[i] = pv->_vpi_time;
        }
        Py_INCREF(tpl);
        self->pdelays = tpl;
    } else {
        self->pdelays       =   PyTuple_New(0);
        self->_vpi_delay.no_of_delays  =   0;
    }
    pyvpi_verbose("pyvpi.Delay is Initial.\n");
    return 0;
}

PyObject * pyvpi_delays_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{   
    p_pyvpi_delays self = (p_pyvpi_delays)type->tp_alloc(type, 0);
    self->_vpi_delay.da = NULL;
    pyvpi_verbose("pyvpi.Delay is allocate,ptr is "FADDR_MARCO", "
        "type ptr is "FADDR_MARCO".\n",self,type);
    return (PyObject *) self;
}

//Get/Set Functions ......
PyObject * s_pyvpi_delays_getdelays(s_pyvpi_delays *self, void *closure){
    Py_INCREF(self->pdelays);
    return self->pdelays;
}
int        s_pyvpi_delays_setdelays(s_pyvpi_delays *self, PyObject *value, void *closure){
    int i;
    if (value == NULL || !PyTuple_Check(value)) {
        PyErr_SetString(VpiError, "Can't set delays to NULL/Non-Tuple.");
        return -1;
    }
    /*
     * Check all value in tuple must be time.
     */
    for(i = 0; i < PyTuple_Size(value); i++) {
        if(!PyObject_TypeCheck(PyTuple_GetItem(value,i) ,&pyvpi_time_Type)){
                PyErr_Format(PyExc_TypeError,  "Error delays in %d, all items in delays must be pyvpi.Time.", i);
                return -1;
        }
    }
    /*
     * Malloc new time memory.
     */
    if(self->_vpi_delay.da != NULL) 
        free(self->_vpi_delay.da);
    self->_vpi_delay.da = NULL;
    
    Py_INCREF(value);
    Py_DECREF(self->pdelays);
    if(PyTuple_Size(value)) {
        self->pdelays       =   value;
        self->_vpi_delay.no_of_delays = PyTuple_Size(value);
        /*Move t_vpi_time for pyvpi_time to *da array. */
        self->_vpi_delay.da = (p_vpi_time) malloc(self->_vpi_delay.no_of_delays * sizeof(s_vpi_time));
        for(i=0; i< PyTuple_Size(value); i++){
            p_pyvpi_time pv = (p_pyvpi_time) PyTuple_GetItem(value,i);
            self->_vpi_delay.da[i] = pv->_vpi_time;    //Struct assign.
        }        
    }
    else {
        self->pdelays       =   value;
        self->_vpi_delay.no_of_delays  =   0;    
    }
    return 0;
}
