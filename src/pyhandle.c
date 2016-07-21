#include "pyhandle.h"

// This variable used to check the handle double free...
static  PyObject* _HandleDict = NULL;

static PyMethodDef  pyvpi_handle_methods[] = {
    {NULL}
};

static PyMemberDef pyvpi_handle_members[]  = {
#ifdef __LP64__
    {"_handle",  T_ULONG,   offsetof(s_pyvpi_handle, _vpi_handle), READONLY, "The real value for vpiHandle."},
#else
    {"_handle",  T_UINT,   offsetof(s_pyvpi_handle, _vpi_handle), READONLY, "The real value for vpiHandle."},
#endif
    {NULL}
};

static PyGetSetDef pyvpi_handle_getsets[]  = {
    {NULL}
};

PyTypeObject pyvpi_handle_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyvpi._handle",              /*tp_name*/
    sizeof(s_pyvpi_handle),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyvpi_handle_Dealloc, /*tp_dealloc*/    
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
    "pyvpi handle objects",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pyvpi_handle_methods,       /* tp_methods */
    pyvpi_handle_members,       /* tp_members */
    pyvpi_handle_getsets,       /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) pyvpi_handle_Init, /* tp_init */
    0,                         /* tp_alloc */
    pyvpi_handle_New,           /* tp_new */
};


void pyvpi_handle_Dealloc(p_pyvpi_handle self)
{
    PyObject * _tmp_h;
    PyObject * key;
    int        cnt;
    //Free self.
    pyvpi_verbose("pyvpi.Handle is release,ptr is "FADDR_MARCO".\n",self);
    //Check this handle is exist or not;
    if(self->_vpi_handle != NULL){
#ifdef __LP64__
        key = Py_BuildValue("k",self->_vpi_handle);
#else
        key = Py_BuildValue("I",self->_vpi_handle);
#endif
        if(self->is_free) {
            //The handle has already release, we must remove key from dict.
            if(PyDict_Contains(_HandleDict, key)) {
                PyDict_DelItem(_HandleDict, key);
            }
        }
        else {
            _tmp_h = PyDict_GetItem(_HandleDict,key);        
            if(_tmp_h != NULL){
                cnt = PyInt_AsLong(_tmp_h) - 1;            
                if(cnt == 0) { 
                    PyDict_DelItem(_HandleDict,key);
                    pyvpi_verbose("pyvpi.Handle->_vpi_handle is release,ptr is "FADDR_MARCO".\n",
                    self->_vpi_handle);
                    vpi_free_object(self->_vpi_handle);
                }
                else {                
                    _tmp_h = PyInt_FromLong(cnt);
                    PyDict_SetItem(_HandleDict,key,_tmp_h); //TBD How about the reference of item before set?
                    Py_DECREF(_tmp_h);
                }
            }
        }
        Py_DECREF(key);
    }
    self->ob_type->tp_free((PyObject*)self);
}

int  pyvpi_handle_Init(s_pyvpi_handle *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {NULL};
    
    PyErr_SetString(VpiError,  "Error, you can't new a Handle object.");
#ifdef PYVPI_DEBUG
    pyvpi_verbose("pyvpi.Handle is Initial.\n");
#endif
    Py_DECREF(self);
    return -1;
}

PyObject * pyvpi_handle_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{   
    p_pyvpi_handle self = (p_pyvpi_handle)type->tp_alloc(type, 0);
    self->_vpi_handle   = NULL;
    self->is_free       = 0;
    pyvpi_verbose("pyvpi.Handle is allocate,ptr is "FADDR_MARCO", type ptr is "FADDR_MARCO".\n",self,type);
    return (PyObject *) self;
}

PyObject *_pyvpi_handle_New(vpiHandle handle){
    //New HandleDict if no allocate;
    s_pyvpi_handle * self;
    PyObject * _tmp_h;
    PyObject * key;
    int         cnt = 1;
    if(handle == NULL) 
        return NULL;
    if(_HandleDict == NULL) {
        _HandleDict = PyDict_New() ;
    }
        self = (p_pyvpi_handle) pyvpi_handle_Type.tp_alloc(&pyvpi_handle_Type, 0);
        self->_vpi_handle = handle;
        self->is_free       = 0;
        pyvpi_verbose("pyvpi.Handle is allocate,ptr is "FADDR_MARCO", type ptr is "FADDR_MARCO".\n",self,&pyvpi_handle_Type);
#ifdef __LP64__        
        key = Py_BuildValue("k",handle);
#else
        key = Py_BuildValue("I",handle);
#endif
        _tmp_h = PyDict_GetItem(_HandleDict,key);

        if(_tmp_h != NULL) {
            cnt = PyInt_AsLong(_tmp_h) + 1;
        }
        else {
            pyvpi_verbose("pyvpi.Handle._vpi_handle is allocate,ptr is "FADDR_MARCO".\n",handle);
        }
        _tmp_h = PyInt_FromLong(cnt);
        PyDict_SetItem(_HandleDict,key,_tmp_h);   //TBD when the key value is same, how to dealwith refer        
        Py_DECREF(_tmp_h);
        Py_DECREF(key);
        return (PyObject *) self;
}

