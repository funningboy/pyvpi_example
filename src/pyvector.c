#include "pyvector.h"

static PyMethodDef  pyvpi_vector_methods[] = {
    {NULL}
};
static PyMemberDef pyvpi_vector_members[]  = {    
    {NULL}
};
static PyGetSetDef pyvpi_vector_getsets[]  = {
    {"size", (getter)s_pyvpi_vector_getsize, 
    (setter)s_pyvpi_vector_setsize,"get/set size.",NULL},
    {"vec", (getter)s_pyvpi_vector_getvalue, 
    (setter)s_pyvpi_vector_setvalue,"get/set value.",NULL},
    {NULL}
};

PyTypeObject pyvpi_vector_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyvpi.Vector",              /*tp_name*/
    sizeof(s_pyvpi_vector),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyvpi_vector_Dealloc, /*tp_dealloc*/    
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
    "pyvpi time objects",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pyvpi_vector_methods,       /* tp_methods */
    pyvpi_vector_members,       /* tp_members */
    pyvpi_vector_getsets,       /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) pyvpi_vector_Init, /* tp_init */
    0,                         /* tp_alloc */
    pyvpi_vector_New,           /* tp_new */
};


void pyvpi_vector_Dealloc(p_pyvpi_vector self)
{
    //Free self.
    pyvpi_verbose("pyvpi.Vector is release,ptr is "FADDR_MARCO".\n",self);
    self->ob_type->tp_free((PyObject*)self);
}

int  pyvpi_vector_Init(s_pyvpi_vector *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"size","vec", NULL};
    PyObject * tmpval = NULL;
    self->size = 4*32;     //Default value.
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|iO", kwlist,                                      
                                      &self->size,&tmpval))
        return -1;    
    pyvpi_verbose("pyvpi.Vector is Initial,size is "FADDR_MARCO".\n",self->size);
    /*1. update vector size.*/    
    if(pyvpi_vector_update_cache(self) != 0){
        return -1;
    }    
    if(tmpval != NULL){
        return s_pyvpi_vector_setvalue(self,tmpval,Py_None);
    }
    return 0;
}

PyObject * pyvpi_vector_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{   
    p_pyvpi_vector self = (p_pyvpi_vector)type->tp_alloc(type, 0);
    if(self == NULL) 
        return NULL;
    self->cache_size = 4;     //Default cache value is 4*32 bits.
    self->cache_ptr  = (p_vpi_vecval) malloc(self->cache_size * sizeof(s_vpi_vecval));    
    self->_vpi_vector= self->cache_ptr;
    if(self->cache_ptr == NULL) {
        PyErr_SetString(PyExc_MemoryError,
                        "There is no enouth memory for vector assign.");    
        Py_DECREF(self);
        return NULL;
    }
    pyvpi_verbose("pyvpi.Vector is allocate,ptr is "FADDR_MARCO", "
        "type ptr is "FADDR_MARCO".\n", self, type);
    return (PyObject *) self;
}

//Misc functions ... ....
PLI_INT32 pyvpi_vector_update_cache(s_pyvpi_vector *self)
{
    PLI_UINT32 numvals ;
    numvals = (self->size + 31) >> 5;
    if(numvals > self->cache_size){
        p_vpi_vecval tmp = (p_vpi_vecval) malloc(self->cache_size * sizeof(s_vpi_vecval) *2);
        if(tmp == NULL) {
            PyErr_SetString(PyExc_MemoryError,
                            "There is no enouth memory for vector assign.");
            return -1;
        }
        free(self->cache_ptr);
        self->cache_size = self->cache_size * 2;
        self->cache_ptr  = tmp;
    } 
    else if(numvals < self->cache_size/2 && numvals > 4) {
        p_vpi_vecval tmp = (p_vpi_vecval) malloc(self->cache_size * sizeof(s_vpi_vecval) /2);
        if(tmp == NULL) {
            PyErr_SetString(PyExc_MemoryError,
                            "There is no enouth memory for vector assign.");
            return -1;
        }
        free(self->cache_ptr);
        self->cache_size = self->cache_size / 2;
        self->cache_ptr  = tmp;
    }
    self->_vpi_vector = self->cache_ptr;
    return 0;
}
/* \brief Getter/Setter 
*/
PyObject * s_pyvpi_vector_getsize(s_pyvpi_vector *self, void *closure)
{
    return Py_BuildValue("I", self->size);
}
int        s_pyvpi_vector_setsize(s_pyvpi_vector *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set size to NULL.");
        return -1;
    }

    if (! PyInt_Check(value)) {
        PyErr_SetString(VpiError,
                        "The reason must be an int.");
        return -1;
    }
    self->size = PyInt_AS_LONG(value);
    return pyvpi_vector_update_cache(self);
}

/* Get value attrib.
 */
PyObject * s_pyvpi_vector_getvalue(s_pyvpi_vector *self, void *closure)
{
    PLI_UINT32      numvals,i;
    PyObject*       list;
    list = PyList_New(0);
    if(list == NULL) return NULL;
    numvals = (self->size + 31) >> 5;
    for(i = 0; i < numvals; i++){
        PyObject *tpl = Py_BuildValue("(II)",
                                     self->_vpi_vector[i].aval,
                                     self->_vpi_vector[i].bval
                                     );
        if(tpl == NULL) return NULL;        
        if(PyList_Append(list,tpl) == -1)
            return NULL;
    }
    return list;
}

int        s_pyvpi_vector_setvalue(s_pyvpi_vector *self, PyObject *value, void *closure)
{
    PLI_UINT32      numvals,i,vsize;
    PLI_INT32       aval,bval;
    PyObject*       tpl;
    //First get value(Must be list) size
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set value to NULL.");
        return -1;
    }

    if (! PyList_Check(value)) {
        PyErr_SetString(VpiError,
                        "The reason must be an List.");
        return -1;
    }
    vsize = PyList_Size(value);
    numvals = (self->size + 31) >> 5;
    for(i = 0; i< numvals; i++){
        aval = 0;
        bval = 0;
        if(i < vsize) { //Keep value didn't assgin zero.
            tpl = PyList_GetItem(value,i);
            if(PyTuple_Check(tpl)){
                //If this is a tuple.
                if(!PyArg_ParseTuple(tpl,"|II",&aval,&bval)){
                    PyErr_SetString(VpiError, "Can't set None (|int) to value.");
                    return -1;
                }
            }
            else if(PyLong_Check(tpl)) {
                //If this is a long.
                aval = PyLong_AsLong(tpl);
            }
            else if(PyInt_Check(tpl)){
                //If this is a integer.
                aval = PyInt_AS_LONG(tpl);
            }
            else {
                PyErr_SetString(VpiError, "Can't set None (|int) to value.");
                return -1;
            }
        }
        self->_vpi_vector[i].aval = aval;
        self->_vpi_vector[i].bval = bval;
    }
    return 0;
}
