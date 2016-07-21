//The 
#include "pyvalue.h"

static PyMethodDef  pyvpi_value_methods[] = {
    {NULL}
};
static PyMemberDef pyvpi_value_members[]  = {
    {"format",  T_UINT, offsetof(s_pyvpi_value, _vpi_value.format), READONLY, " format"},
    {NULL}
};
static PyGetSetDef pyvpi_value_getsets[]  = {
    {"value", (getter)s_pyvpi_value_getvalue, 
    (setter)s_pyvpi_value_setvalue,"get/set value.",NULL},
    {NULL}
};

//Type define here
PyTypeObject pyvpi_value_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyvpi.Value",             /*tp_name*/
    sizeof(s_pyvpi_value),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyvpi_value_Dealloc, /*tp_dealloc*/    
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
    "pyvpi value objects",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pyvpi_value_methods,       /* tp_methods */
    pyvpi_value_members,       /* tp_members */
    pyvpi_value_getsets,       /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) pyvpi_value_Init, /* tp_init */
    0,                         /* tp_alloc */
    pyvpi_value_New,           /* tp_new */
};

void pyvpi_value_Dealloc(p_pyvpi_value self)
{
    //Free self.
    pyvpi_verbose("pyvpi.Value is release, "
        "ptr is "FADDR_MARCO".\n",self);
    if(self->obj != NULL) 
        Py_XDECREF(self->obj);
    if(self->fixed_handle !=NULL) 
        Py_XDECREF(self->fixed_handle);
    self->ob_type->tp_free((PyObject*)self);
}

int  pyvpi_value_Init(s_pyvpi_value *self, PyObject *args, PyObject *kwds)
{    
    static char *kwlist[] = {"format", NULL};
    
    self->_vpi_value.format = vpiHexStrVal; //Default value.
    Py_DECREF(self->obj);
    self->obj    = PyString_FromString("");
    self->_vpi_value.value.str = PyString_AsString(self->obj);
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist,
                                      &self->_vpi_value.format))
    {
        PyErr_SetString(VpiError, "The pyvpi.Value initial args must be "
            "(format = int).");
        return -1;
    }
    Py_DECREF(self->obj);       //For inital, we need no object...
    pyvpi_verbose("pyvpi.Value is Initial, "
                                      "format is "FADDR_MARCO".\n",self->_vpi_value.format);
    return update_format(self,self->_vpi_value.format,NULL);
}

PyObject * pyvpi_value_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    p_pyvpi_value   self;
    int i = 0;
    self = (p_pyvpi_value)type->tp_alloc(type, 0);
    if(self == NULL) 
        return NULL;

    Py_INCREF(Py_None);
    self-> obj = Py_None;
    self->fixed_handle = NULL;
    pyvpi_verbose("pyvpi.Value is allocate, "
                                      "ptr is "FADDR_MARCO", type ptr is "FADDR_MARCO".\n",self,type);
    return (PyObject *) self;
}

//Get/Set
PyObject * s_pyvpi_value_getvalue(s_pyvpi_value *self, void *closure)
{
    if(self->obj == NULL) {
        if(self->_vpi_value.format == vpiBinStrVal ||
           self->_vpi_value.format == vpiOctStrVal ||
           self->_vpi_value.format == vpiDecStrVal ||
           self->_vpi_value.format == vpiHexStrVal ||
           self->_vpi_value.format == vpiStringVal) {
            self->obj = PyString_FromString(self->_vpi_value.value.str);
        }
        /*
        else if(self->_vpi_value.format == vpiScalarVal) {
            self->obj = PyInt_FromLong(self->_vpi_value.value.scalar);
        }
        else if(self->_vpi_value.format == vpiIntVal) {
            self->obj    = PyInt_FromLong(self->_vpi_value.value.integer);
        }
        else if(self->_vpi_value.format == vpiRealVal){
             self->obj    = PyFloat_FromDouble(self->_vpi_value.value.real);
        }*/
        else {
            PyErr_SetString(VpiError, "pyvpi inner error occur when string value return!");
            return NULL;
        }
    }
    Py_INCREF(self->obj);
    return self->obj;
}
int        s_pyvpi_value_setvalue(s_pyvpi_value *self, PyObject *value, void *closure)
{
    PyObject * tmp;
    if (value == NULL) {
        PyErr_SetString(VpiError, "Can't set index to NULL.");
        return -1;
    }
    Py_INCREF(self->obj);
    tmp = self->obj;
    Py_INCREF(value);
    Py_DECREF(self->obj);
    if(update_format(self,self->_vpi_value.format,value) == -1){
        Py_DECREF(value);
        self->obj = tmp;
        return -1;
    }
    Py_DECREF(tmp);
    return 0;
}

/* Update vpi value by vpi value structure.
 * If this format if vpiVectorVal, use must support the vector size.
 * This function must deal with the reference, this is different with
 * update_format.
 */
void pyvip_value_update_value(s_pyvpi_value *self, s_vpi_value *ovalp, PLI_INT32 blen)
{
    p_pyvpi_vector pvector;
    p_pyvpi_time   ptime;
    p_pyvpi_strengthval   pstrength;
    PLI_INT32   numvals,i;
    PyObject*   tmpobj;
    PyObject*   dictobj;

    Py_INCREF(self->obj);
    tmpobj = self->obj;
    if(self->fixed_handle == NULL) {
        Py_DECREF(self->obj);
        self->obj = NULL;   /* If value is not fixed, always create new object for value object point.*/
        self->_vpi_value.format    = ovalp->format;
    }
    else {
        if(self->_vpi_value.format != ovalp->format)
            pyvpi_error("Found error when update fixed handle pyvpi.Value!");
    }
    switch (ovalp->format) {
    /* all string values */
    case vpiBinStrVal:
    case vpiOctStrVal:
    case vpiDecStrVal:
    case vpiHexStrVal:
    case vpiStringVal:
        if(self->fixed_handle == NULL) {
            if(self->obj == NULL)
                self->obj    = PyString_FromString(ovalp->value.str);
            self->_vpi_value.value.str = PyString_AsString(self->obj);
        } 
        else {
            char *ptr = ovalp->value.str;
            int  i  = 0;
            while(ptr[i] != '\0') {
                self->_vpi_value.value.str[i] = ptr[i];
                i++;
            }
            self->_vpi_value.value.str[i] = '\0';
            Py_XDECREF(self->obj);           
            self->obj = NULL; /* Simple string operate, we will create obj at value return. */
        }
        break;
    case vpiScalarVal:
        if(self->fixed_handle == NULL) {
            self->obj    = PyInt_FromLong(ovalp->value.scalar);
            self->_vpi_value.value.scalar = PyInt_AsLong(self->obj);
        }
        else {            
            self->_vpi_value.value.scalar = ovalp->value.scalar;
            Py_XDECREF(self->obj);
            self->obj    = NULL;
        }
        break;
    case vpiIntVal:
        if(self->fixed_handle == NULL) {
            self->obj    = PyInt_FromLong(ovalp->value.integer);
            self->_vpi_value.value.integer = PyInt_AsLong(self->obj);
        }
        else {
            self->_vpi_value.value.integer = ovalp->value.integer;
            Py_XDECREF(self->obj);
            self->obj    = NULL;
        }
        break;
    case vpiRealVal:
        if(self->fixed_handle == NULL) {
            self->obj    = PyFloat_FromDouble(ovalp->value.real);
            self->_vpi_value.value.real = PyFloat_AsDouble(self->obj);
        }
        else {
            self->_vpi_value.value.real = ovalp->value.real;
            Py_XDECREF(self->obj);
            self->obj    = NULL;
        }
        break;
    case vpiVectorVal:
        if(self->fixed_handle == NULL) {
            self->obj    = pyvpi_vector_New(&pyvpi_vector_Type,DumbTuple,DumbDict);
            dictobj = PyDict_New();
            PyDict_SetItem(dictobj,PyString_FromString("size"),PyInt_FromLong(blen));
            pyvpi_vector_Init((p_pyvpi_vector)self->obj,DumbTuple,dictobj);    //TBD check error.
            pvector = (p_pyvpi_vector) self->obj;
            numvals = (blen + 31) >> 5;
            for(i=0; i <numvals; i++)
            {
                pvector->_vpi_vector[i].aval = ovalp->value.vector[i].aval;
                pvector->_vpi_vector[i].bval = ovalp->value.vector[i].bval;
            }
            self->_vpi_value.value.vector = pvector->_vpi_vector;
        }
        else {
            numvals = (blen + 31) >> 5;
            for(i=0; i <numvals; i++)
            {
                self->_vpi_value.value.vector[i] = ovalp->value.vector[i];
            }
        }
        break;
    case vpiStrengthVal:
        if(self->fixed_handle == NULL) {
            self->obj    = pyvpi_strengthval_New(&pyvpi_strengthval_Type,DumbTuple,DumbDict);
            pyvpi_strengthval_Init((p_pyvpi_strengthval)self->obj,DumbTuple,DumbDict);        
            pstrength = (p_pyvpi_strengthval) self->obj;
            self->_vpi_value.value.strength = &pstrength->_vpi_strengthval;
        
            pstrength->_vpi_strengthval.logic = ovalp->value.strength->logic;
            pstrength->_vpi_strengthval.s0 = ovalp->value.strength->s0;
            pstrength->_vpi_strengthval.s1 = ovalp->value.strength->s1;
        }
        else {
            *self->_vpi_value.value.strength = *ovalp->value.strength;  //Memory copy.
        }
        break;
    case vpiTimeVal:
        if(self->fixed_handle == NULL) {
            self->obj    = pyvpi_time_New(&pyvpi_time_Type,DumbTuple,DumbDict);
            pyvpi_time_Init((p_pyvpi_time)self->obj,DumbTuple,DumbDict);
            ptime = (p_pyvpi_time) self->obj;
            self->_vpi_value.value.time = &ptime->_vpi_time;
        
            ptime->_vpi_time.type = ovalp->value.time->type;
            ptime->_vpi_time.high = ovalp->value.time->high;
            ptime->_vpi_time.low = ovalp->value.time->low;
            ptime->_vpi_time.real = ovalp->value.time->real;
        }
        else {
            *self->_vpi_value.value.time = *ovalp->value.time;
        }
        break;
        /* not sure what to do here? */
    case vpiObjTypeVal: case vpiSuppressVal:
        break;
    }
    Py_DECREF(tmpobj);
    return ;
}

/* Update format 
 * User can update format and object here, before call this function, all reference must
 * be care processed.
 */
PLI_INT32 update_format(p_pyvpi_value self, PLI_INT32 nformat, PyObject* nobj)
{
    p_pyvpi_vector pvector;
    p_pyvpi_time   ptime;
    p_pyvpi_strengthval   pstrength;    
    switch (nformat) {
    /* all string values */
    case vpiBinStrVal:
    case vpiOctStrVal:
    case vpiDecStrVal:
    case vpiHexStrVal:
    case vpiStringVal:
        if(nobj == NULL)
            nobj    = PyString_FromString("");
        if (! PyString_Check(nobj)) {
                PyErr_SetString(VpiError,
                                "The value must be a string<Current format is string>.");
                return -1;
            }
        if(self->fixed_handle == NULL) {
            self->_vpi_value.value.str = PyString_AsString(nobj);
        }
        else {
            char *ptr = PyString_AsString(nobj);
            int  i = 0;
            if(PyString_Size(nobj) + 1 > self->cache_size) {
                PyErr_SetString(VpiError,
                                "The string size is overflow.");
                return -1;
            }
            while(ptr[i] != '\0') {
                self->_vpi_value.value.str[i] = ptr[i];
                i++;
            }
            self->_vpi_value.value.str[i] = '\0';
        }
        break;
    case vpiScalarVal:
        if(nobj == NULL)
            nobj = PyInt_FromLong(vpi0);
        if (! PyInt_Check(nobj) || PyInt_AS_LONG(nobj) < 0 || PyInt_AS_LONG(nobj) > 3) {
            PyErr_SetString(VpiError,
                            "The value must be vpi[0,1,X,Z]<Current format is vpiScalarVal>.");
            return -1;
        }
        self->_vpi_value.value.scalar = PyInt_AsLong(nobj);
        break;
    case vpiIntVal:
        if(nobj == NULL)
            nobj    = PyInt_FromLong(0);
        if (! PyInt_Check(nobj)) {
                PyErr_SetString(VpiError,
                                "The value must be an int<Current format is vpiIntVal>.");
                return -1;
            }
        self->_vpi_value.value.integer = PyInt_AsLong(nobj);
        break;
    case vpiRealVal:
        if(nobj == NULL)
            nobj    = PyFloat_FromDouble(0.0);
        if (! PyFloat_Check(nobj)) {
            PyErr_SetString(VpiError,
                            "The value must be an float<Current format is vpiRealVal>.");
            return -1;
        }
        self->_vpi_value.value.real = PyFloat_AsDouble(nobj);
        break;
    case vpiVectorVal:
        if(nobj == NULL) {
            nobj    = pyvpi_vector_New(&pyvpi_vector_Type,DumbTuple,DumbDict);
            pyvpi_vector_Init((p_pyvpi_vector)nobj,DumbTuple,DumbDict);   //TBD check error
        }
        if (! PyObject_TypeCheck(nobj,&pyvpi_vector_Type)) {
            PyErr_SetString(VpiError,
                            "The value object be a pyvpi.Vector<Current format is vpiVectorVal>.");
            return -1;
        }
        pvector = (p_pyvpi_vector) nobj;
        if(self->fixed_handle == NULL) {
            self->_vpi_value.value.vector = pvector->_vpi_vector;
        }
        else {
            int numvals = (pvector->size + 31) >> 5;
            int i = 0;
            if(((p_pyvpi_vector)self->obj)->size != pvector->size) {
                PyErr_SetString(VpiError,
                            "For fixed handle value, vector size must equal.");
                return -1;
            }            
            for(i=0; i <numvals; i++)
            {
                self->_vpi_value.value.vector[i] = pvector->_vpi_vector[i];
            }
        }
        break;
    case vpiStrengthVal:
        if(nobj == NULL) {
            nobj    = pyvpi_strengthval_New(&pyvpi_strengthval_Type,DumbTuple,DumbDict);
            pyvpi_strengthval_Init((p_pyvpi_strengthval)nobj,DumbTuple,DumbDict);
        }
        if (! PyObject_TypeCheck(nobj,&pyvpi_strengthval_Type)) {
            PyErr_SetString(VpiError,
                            "The value object be a pyvpi._Strength<Current format is vpiStrengthVal>.");
            return -1;
        }
        pstrength = (p_pyvpi_strengthval) nobj;
        if(self->fixed_handle == NULL) {
            self->_vpi_value.value.strength = &pstrength->_vpi_strengthval;
        }
        else {
            *self->_vpi_value.value.strength = pstrength->_vpi_strengthval;
        }
        break;
    case vpiTimeVal:
        if(nobj == NULL) {
            nobj    = pyvpi_time_New(&pyvpi_time_Type,DumbTuple,DumbDict);
            pyvpi_time_Init((p_pyvpi_time)nobj,DumbTuple,DumbDict);
        }
        if (! PyObject_TypeCheck(nobj,&pyvpi_time_Type)) {
            PyErr_SetString(VpiError,
                            "The value object be a pyvpi.Time<Current format is vpiTimeVal>.");
            return -1;
        }
        ptime = (p_pyvpi_time) nobj;
        if(self->fixed_handle == NULL) {
            self->_vpi_value.value.time = &ptime->_vpi_time;
        }
        else {
            *self->_vpi_value.value.time = ptime->_vpi_time;
        }
        break;
        /* not sure what to do here? */
    case vpiObjTypeVal: case vpiSuppressVal:
        PyErr_SetString(VpiError,"The format of pyvpi.Value not "
            "support vpiObjTypeVal and vpiSuppressVal."
            );
        return -1;
    default :
        PyErr_SetString(VpiError,"The format of pyvpi.Value must be "
            "vpi[[Bin,Oct,Dec,Hex]Str,Scalar,Int,Real,String,Vector,"
            "Strength,Suppress,Time,ObjType]Val.");
        return -1;
    }
    Py_INCREF(nobj);
    Py_XDECREF(self->obj);
    self->obj = nobj;
    self->_vpi_value.format    = nformat;
    return 0;
}

