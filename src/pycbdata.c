#include "pycbdata.h"
#include "pyvpi.h"

static PyMethodDef  pyvpi_cbdata_methods[] = {
    {NULL}
};
static PyMemberDef pyvpi_cbdata_members[]  = {
    {"cbhandle",  T_OBJECT, offsetof(s_pyvpi_cbdata, cb_h), READONLY, " handle after register"},
    {"user_data",  T_OBJECT, offsetof(s_pyvpi_cbdata, user_data), 0,  " user data object"},
    {NULL}
};
static PyGetSetDef pyvpi_cbdata_getsets[]  = {
    {"reason", (getter)s_pyvpi_cbdata_getreason, 
    (setter)s_pyvpi_cbdata_setreason,"get/set callback reason.",NULL},
    {"trgobj", (getter)s_pyvpi_cbdata_gettrgobj, 
    (setter)s_pyvpi_cbdata_settrgobj,"get/set trigger object.",NULL},
    {"time", (getter)s_pyvpi_cbdata_gettime, 
    (setter)s_pyvpi_cbdata_settime,"get/set trigger time.",NULL},
    {"value", (getter)s_pyvpi_cbdata_getvalue, 
    (setter)s_pyvpi_cbdata_setvalue,"get/set trigger value.",NULL},
    {"index", (getter)s_pyvpi_cbdata_getindex, 
    (setter)s_pyvpi_cbdata_setindex,"get/set index.",NULL},
    {"callback", (getter)s_pyvpi_cbdata_getcallback, 
    (setter)s_pyvpi_cbdata_setcallback,"get/set callback for trigger.",NULL},
    {NULL}
};

//Type define here.
PyTypeObject pyvpi_cbdata_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyvpi.CbData",            /*tp_name*/
    sizeof(s_pyvpi_cbdata),    /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyvpi_cbdata_Dealloc, /*tp_dealloc*/    
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
    PyObject_GenericGetAttr,   /*tp_getattro*/
    PyObject_GenericSetAttr,   /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "pyvpi cbdata objects",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pyvpi_cbdata_methods,      /* tp_methods */
    pyvpi_cbdata_members,      /* tp_members */
    pyvpi_cbdata_getsets,      /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    offsetof(s_pyvpi_cbdata, dict),    /* tp_dictoffset */
    (initproc) pyvpi_cbdata_Init,      /* tp_init */
    0,                         /* tp_alloc */
    pyvpi_cbdata_New,           /* tp_new */
};

void 
pyvpi_cbdata_Dealloc(p_pyvpi_cbdata self)
{
    //Free self.
    p_pyvpi_value pvalue;
    p_pyvpi_time  ptime;
    pvalue = (p_pyvpi_value) ((size_t)self->_vpi_cbdata.value - offsetof(s_pyvpi_value, _vpi_value));
    Py_DECREF(pvalue);
    ptime  = (p_pyvpi_time)  ((size_t)self->_vpi_cbdata.time - offsetof(s_pyvpi_time, _vpi_time));
    Py_DECREF(ptime);    
    Py_DECREF(self->obj_h);
    Py_XDECREF(self->user_data);
    Py_XDECREF(self->dict);
    pyvpi_verbose("pyvpi.CbData is release,ptr is "FADDR_MARCO".\n",self);
    self->ob_type->tp_free((PyObject*)self);
}
/*
 * CbData initial,
 * params : 
 *      reason  = ? (PLI_INT32)
 *      trg_obj = ? (vpiHandle)
 *      cb_time = ? (PyObject * pyvpi.Time)
 */
int  
pyvpi_cbdata_Init(p_pyvpi_cbdata self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"reason","trgobj","value","time","callback", NULL};
    p_pyvpi_value pvalue = NULL;
    p_pyvpi_time  ptime = NULL;
    p_pyvpi_handle obj_h = NULL;
    PyObject* callback = NULL;

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|iOOOO", kwlist,
                                      &self->_vpi_cbdata.reason,
                                      &obj_h,
                                      &pvalue,
                                      &ptime,
                                      &callback))
    {
        PyErr_SetString(VpiError, "The pyvpi.CbData initial args must be "
            "(reason = int,trgobj = pyvpi.Handle, value = pyvpi.Value"
            ", time = pyvpi.Time, callback = *func).");
        return -1;
    }
    /* Update trgobj */        
    if(obj_h == NULL) {
        self->_vpi_cbdata.obj = NULL;
        obj_h = (p_pyvpi_handle) Py_None;
    }
    else if(Py_TYPE(obj_h) ==  &pyvpi_handle_Type) {
        self->_vpi_cbdata.obj = obj_h->_vpi_handle;
    }
    else {
        PyErr_SetString(VpiError, "The 2nd arg must be pyvpi.Handle.");
        return -1;
    }
    Py_INCREF(obj_h);
    Py_XDECREF(self->obj_h);
    self->obj_h = (PyObject *)obj_h;

    /* Initial cbdata's value field. */
    if(pvalue == NULL) {
        pvalue = (p_pyvpi_value) pyvpi_value_New(&pyvpi_value_Type,DumbTuple,DumbDict);
        pyvpi_value_Init(pvalue,DumbTuple,DumbDict);
    }
    else if(Py_TYPE(pvalue) ==  &pyvpi_value_Type){
        Py_INCREF(pvalue);
        if(self->_vpi_cbdata.value != NULL) {
            p_pyvpi_value _pvalue  = (p_pyvpi_value)
                ((size_t)self->_vpi_cbdata.value - offsetof(s_pyvpi_value, _vpi_value));
            Py_DECREF(_pvalue);
        }
    }
    else {
        PyErr_SetString(VpiError, "The 3rd arg must be  pyvpi.Value.");
        return -1;
    }
    self->_vpi_cbdata.value = &pvalue->_vpi_value;

    /* Initial cbdata's time  */
    if(ptime == NULL) {
        ptime = (p_pyvpi_time) pyvpi_time_New(&pyvpi_time_Type,DumbTuple,DumbDict);
        pyvpi_time_Init(ptime,DumbTuple,DumbDict);
    }
    else if(Py_TYPE(ptime) == &pyvpi_time_Type) {
        Py_INCREF(ptime);
        if(self->_vpi_cbdata.time != NULL) {
            p_pyvpi_time _ptime = (p_pyvpi_time)
                ((size_t)self->_vpi_cbdata.time - offsetof(s_pyvpi_time, _vpi_time));
            Py_DECREF(_ptime);
        }
    }
    else {
        PyErr_SetString(VpiError, "The 4th arg must be pyvpi.Time.");
        return -1;
    }
    self->_vpi_cbdata.time = &ptime->_vpi_time;
  
    /* Initial callback function */
    if(callback != NULL && PyCallable_Check(callback)) {
        Py_XDECREF(self->callback);
        Py_INCREF(callback);
        self->callback = callback;
    }
    else if(callback != NULL) {
        PyErr_SetString(VpiError, "The 5th arg must be a function.");
        return -1;
    }
    self->_vpi_cbdata.cb_rtn = _pyvpi_cb_rtn;  //All CbObject's callback is _pyvpi_cb_rtn.
    self->_vpi_cbdata.user_data = (PLI_BYTE8 *) self;   //The user_data always be self.
    pyvpi_verbose("pyvpi.CbData is Initial.\n");
    return 0;
}

PyObject *
pyvpi_cbdata_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    p_pyvpi_cbdata   self;
        
    self = (p_pyvpi_cbdata)type->tp_alloc(type, 0);
    self->_vpi_cbdata.reason = cbValueChange;
    self->_vpi_cbdata.value = NULL;
    self->_vpi_cbdata.time  = NULL;    
    Py_INCREF(Py_None);
    self->cb_h = Py_None;    //Initial callback handle none.
    Py_INCREF(Py_None);
    self->user_data = Py_None;    //user_data
    self->dict = PyDict_New();
    Py_INCREF(Py_None);
    self->callback = Py_None;
    if(!self){
        PyErr_SetString(VpiError, "Can't allocate a pyvpi.CbData memory.");
        return NULL;
    }
    pyvpi_verbose("pyvpi.CbData is allocate,ptr is "FADDR_MARCO", "
        "type ptr is "FADDR_MARCO".\n",self,type);
    return (PyObject *) self;
}

//Get/Set Functions
//reason
PyObject *
s_pyvpi_cbdata_getreason(s_pyvpi_cbdata *self, void *closure)
{
    return Py_BuildValue("i", self->_vpi_cbdata.reason);
}
int        
s_pyvpi_cbdata_setreason(s_pyvpi_cbdata *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set reason to NULL.");
        return -1;
    }

    if (! PyInt_Check(value)) {
        PyErr_SetString(VpiError,
                        "The reason must be an int.");
        return -1;
    }
    self->_vpi_cbdata.reason = PyInt_AS_LONG(value);
    return 0;
}
//trigger object
PyObject *
s_pyvpi_cbdata_gettrgobj(s_pyvpi_cbdata *self, void *closure)
{
    Py_INCREF(self->obj_h);
    return self->obj_h;
}

int        
s_pyvpi_cbdata_settrgobj(s_pyvpi_cbdata *self, PyObject *value, void *closure)
{
    p_pyvpi_handle  handle;
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set trigger object to NULL.");
        return -1;
    }
    if (!(PyObject_TypeCheck(value,&pyvpi_handle_Type))) {
        PyErr_SetString(VpiError,
                        "The reason trigger object be a pyvpi._vpiHandle.");
        return -1;
    }
    
    handle = (p_pyvpi_handle) value;
    Py_INCREF(handle);
    Py_DECREF(self->obj_h);
    self->obj_h =   (PyObject *)handle;
    self->_vpi_cbdata.obj = handle->_vpi_handle;
    return 0;
}

//trigger time
PyObject * 
s_pyvpi_cbdata_gettime(s_pyvpi_cbdata *self, void *closure)
{
    p_pyvpi_time ptime;
    ptime  = (p_pyvpi_time)  ((size_t)self->_vpi_cbdata.time - offsetof(s_pyvpi_time, _vpi_time));
    Py_INCREF(ptime);
    return (PyObject *) ptime;
}
int        
s_pyvpi_cbdata_settime(s_pyvpi_cbdata *self, PyObject *value, void *closure)
{
    p_pyvpi_time ptime, tmp = (p_pyvpi_time) value;
    ptime  = (p_pyvpi_time)  ((size_t)self->_vpi_cbdata.time - offsetof(s_pyvpi_time, _vpi_time));
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set time object to NULL.");
        return -1;
    }
    if (! PyObject_TypeCheck(value,&pyvpi_time_Type)) {
        PyErr_SetString(VpiError,
                        "The value object be a pyvpi.Time.");
        return -1;
    }
    Py_DECREF(ptime);
    Py_INCREF(tmp);
    self->_vpi_cbdata.time = &tmp->_vpi_time;
    return 0;
}
//value
PyObject * 
s_pyvpi_cbdata_getvalue(s_pyvpi_cbdata *self, void *closure)
{
    p_pyvpi_value pvalue ;
    pvalue = (p_pyvpi_value) ((size_t)self->_vpi_cbdata.value - offsetof(s_pyvpi_value, _vpi_value));
    Py_INCREF(pvalue);
    return (PyObject *) pvalue;
}

int        
s_pyvpi_cbdata_setvalue(s_pyvpi_cbdata *self, PyObject *value, void *closure)
{
    p_pyvpi_value pvalue, tmp = (p_pyvpi_value) value;
    pvalue = (p_pyvpi_value) ((size_t)self->_vpi_cbdata.value - offsetof(s_pyvpi_value, _vpi_value));
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set value object to NULL.");
        return -1;
    }
    if (! PyObject_TypeCheck(value,&pyvpi_value_Type)) {
        PyErr_SetString(VpiError,
                        "The value object be a pyvpi.Value.");
        return -1;
    }
    Py_INCREF(tmp);
    Py_DECREF(pvalue);    
    self->_vpi_cbdata.value = &tmp->_vpi_value;
    return 0;
}
//index
PyObject * 
s_pyvpi_cbdata_getindex(s_pyvpi_cbdata *self, void *closure)
{
    return Py_BuildValue("i", self->_vpi_cbdata.index);
}
int       
s_pyvpi_cbdata_setindex(s_pyvpi_cbdata *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set index to NULL.");
        return -1;
    }

    if (! PyInt_Check(value)) {
        PyErr_SetString(VpiError,
                        "The index must be an int.");
        return -1;
    }
    self->_vpi_cbdata.index = PyInt_AS_LONG(value);
    return 0;
}
//callback
PyObject * 
s_pyvpi_cbdata_getcallback(s_pyvpi_cbdata *self, void *closure)
{
    if(self->callback == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    else {
        return self->callback;
    }
}

int        
s_pyvpi_cbdata_setcallback(s_pyvpi_cbdata *self, PyObject *value, void *closure){
    if (!PyCallable_Check(value)) {
            PyErr_SetString(VpiError, "Parameter must be a callable.");
            return -1;
        }
    Py_XINCREF(value);
    self->callback = value;
    return 0;
}

/* _pyvpi_cb_rtn(p_cb_data) :
 * Function for callback.
 */
PLI_INT32 
_pyvpi_cb_rtn(p_cb_data data)
{
    PyObject *arglist;
    int      blen = 0,nd_alloc = 0;    
    //In this function, we must convert data to s_pyvpi_cbdata;
    p_pyvpi_cbdata self = (s_pyvpi_cbdata *) data->user_data;
    p_pyvpi_value  pv   = (p_pyvpi_value) ((size_t)self->_vpi_cbdata.value - offsetof(s_pyvpi_value, _vpi_value));
    pyvpi_verbose("_pyvpi_cb_rtn in.\n");
    //Return -1 if callback is not callback...
    if (!PyCallable_Check(self->callback)) {
        pyvpi_error("callback is not callable!\n");
        return -1;
    }
    
    //1. We must copy the tmp value to our struct...    
    //First, Check bit length if the value format is vpiVectorVal.
    if(data->value != NULL) {
        if(data->value->format == vpiVectorVal) {
            if(pv->fixed_handle == NULL) {
                blen = vpi_get(vpiSize,data->obj);
                if(pyvpi_CheckError()) {
                    return -1;
                }
            }
            else {
                /* For fixed handle value size is unchange_able */
                blen = ((p_pyvpi_vector)pv->obj)->size;
            }
        }
        pyvip_value_update_value(pv,data->value,blen);
    }

    //2. We must copy the tmp time to our struct...
    *(self->_vpi_cbdata.time) = *(data->time); //This will force change _Time object value.
    arglist = Py_BuildValue("(O)",self);    
    (void) PyObject_CallObject(self->callback, arglist);
    Py_DECREF(arglist);
    pyvpi_verbose("_pyvpi_cb_rtn out.\n");
    return 0;
}
