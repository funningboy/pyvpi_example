/*
 * =====================================================================================
 *
 *       Filename:  pysystfdata.c
 *
 *    Description:  This is systfdata structure.
 *
 *        Version:  1.0
 *        Created:  12/22/2011 09:54:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gene Kong (), genekong.sw@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "pysystfdata.h"

static PyMethodDef  pyvpi_systfdata_methods[] = {
    {NULL}
};
static PyMemberDef pyvpi_systfdata_members[]  = {
    {NULL}
};
static PyGetSetDef pyvpi_systfdata_getsets[]  = {
    {   "type", 
        (getter)s_pyvpi_systfdata_gettype,             
        (setter)s_pyvpi_systfdata_settype,
        "get/set function/task type.",
        NULL},
    {   "sysfunctype", 
        (getter)s_pyvpi_systfdata_getsysfunctype, 
        (setter)s_pyvpi_systfdata_setsysfunctype,
        "get/set function/task return type.",
        NULL},
    {   "tfname", 
        (getter)s_pyvpi_systfdata_gettfname, 
        (setter)s_pyvpi_systfdata_settfname,
        "get/set function/task name(start with $).",
        NULL},
    {   "calltf", 
        (getter)s_pyvpi_systfdata_getcalltf, 
        (setter)s_pyvpi_systfdata_setcalltf,
        "get/set call function/task.",
        NULL},
    {   "compiletf", 
        (getter)s_pyvpi_systfdata_getcompiletf, 
        (setter)s_pyvpi_systfdata_setcompiletf,
        "get/set  compile function/task.",
        NULL},
    {   "sizetf", 
        (getter)s_pyvpi_systfdata_getsizetf, 
        (setter)s_pyvpi_systfdata_setsizetf,
        "get/set  compile function/task.",
        NULL},
    {NULL}
};

PyTypeObject pyvpi_systfdata_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "pyvpi._SysTfData",        /*tp_name*/
    sizeof(s_pyvpi_systfdata), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyvpi_systfdata_Dealloc, /*tp_dealloc*/    
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
    "pyvpi system task func data objects",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pyvpi_systfdata_methods,       /* tp_methods */
    pyvpi_systfdata_members,       /* tp_members */
    pyvpi_systfdata_getsets,       /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) pyvpi_systfdata_Init, /* tp_init */
    0,                         /* tp_alloc */
    pyvpi_systfdata_New,           /* tp_new */
};


void pyvpi_systfdata_Dealloc(p_pyvpi_systfdata self)
{
    //Free self.
    Py_XDECREF(self->tfname);
    Py_XDECREF(self->calltf);
    Py_XDECREF(self->compiletf);
    Py_XDECREF(self->sizetf);
    pyvpi_verbose("pyvpi._SysTfData is release, "
                                      "ptr is "FADDR_MARCO".\n",self);
    self->ob_type->tp_free((PyObject*)self);
}

int  pyvpi_systfdata_Init(s_pyvpi_systfdata *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"type","sysfunctype","tfname","calltf","compiletf","sizetf", NULL};
    self->tfname    =   NULL;
    self->calltf    =   NULL;
    self->compiletf =   NULL;
    self->sizetf    =   NULL;
    self->_vpi_systfdata.type           = vpiSysTask; //Default value.
    self->_vpi_systfdata.sysfunctype    = vpiSysTask; //Default value.
    self->_vpi_systfdata.user_data      = (void *)self;
    self->_vpi_systfdata.calltf         = _calltf;
    self->_vpi_systfdata.compiletf      = _compiletf;
    self->_vpi_systfdata.sizetf         = _sizetf;
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|iiOOOO", kwlist,                                      
                                      &self->_vpi_systfdata.type,
                                      &self->_vpi_systfdata.sysfunctype,
                                      &self->tfname,
                                      &self->calltf,
                                      &self->compiletf,
                                      &self->sizetf
                                      ))
        return -1;
    //Check type and sysfunctype.
    if(self->_vpi_systfdata.type == vpiSysTask) {
        if(self->_vpi_systfdata.sysfunctype != vpiSysTask) {
            PyErr_SetString(VpiError, "Can't set sysfunctype to other values"
                    "except vpiSysTask for vpiSysTask.");
            return -1;
        }
    }
    else if(self->_vpi_systfdata.sysfunctype == vpiSysFunc){
        if( self->_vpi_systfdata.sysfunctype != vpiIntFunc   &&
            self->_vpi_systfdata.sysfunctype != vpiRealFunc  &&
            self->_vpi_systfdata.sysfunctype != vpiTimeFunc  &&
            self->_vpi_systfdata.sysfunctype != vpiSizedFunc &&
            self->_vpi_systfdata.sysfunctype != vpiSizedSignedFunc ) {
            PyErr_SetString(VpiError, "Can't set sysfunctype to other values "
                    "except vpi[Int,Real,Time,Sized,SizedSigned]Func for vpiSysFunc.");
            return -1;
        }
    }
    else {
        PyErr_SetString(VpiError, "Can't set type to other values except vpiSysTask,vpiSysFunc.");
        return -1;
    }


    //Check tfname
    if(self->tfname != NULL){
        if(s_pyvpi_systfdata_settfname(self,self->tfname,NULL) == -1)
            return -1;
    }

    //Check calltf
    if(self->calltf != NULL){
        if(s_pyvpi_systfdata_setcalltf(self,self->calltf,NULL) == -1) 
            return -1;
    } 
    //Check compiletf
    if(self->compiletf != NULL){
        if(s_pyvpi_systfdata_setcompiletf(self,self->compiletf,NULL) == -1) 
            return -1;
    } 
    //Check sizetf
    if(self->sizetf != NULL){
        if(s_pyvpi_systfdata_setsizetf(self,self->sizetf,NULL) == -1) 
            return -1;
    } 
    pyvpi_verbose("pyvpi._SysTfData is Initial, "
        "type is "FADDR_MARCO".\n",self->_vpi_systfdata.type);
    return 0;
}

PyObject * pyvpi_systfdata_New(PyTypeObject *type, PyObject *args, PyObject *kwds)
{   
    p_pyvpi_systfdata self = (p_pyvpi_systfdata)type->tp_alloc(type, 0);
    pyvpi_verbose("pyvpi._SysTfData is allocate,ptr is "FADDR_MARCO", "
        "type ptr is "FADDR_MARCO".\n",self,type);
    return (PyObject *) self;
}

PyObject * s_pyvpi_systfdata_gettype(s_pyvpi_systfdata *self, void *closure)
{
    return Py_BuildValue("i",self->_vpi_systfdata.type);
}
int        s_pyvpi_systfdata_settype(s_pyvpi_systfdata *self, PyObject *value, void *closure)
{
    //Check type, it must be vpiSysTask,vpiSysFunc.
    int tmp;
    if(!PyInt_Check(value)){
        PyErr_SetString(VpiError,
                        "The value must be vpiSys[Task,Func].");
        return -1;
    }
    tmp = PyInt_AS_LONG(value);
    if(tmp != vpiSysTask &&
       tmp != vpiSysFunc) {
        PyErr_SetString(VpiError,
                        "The value must be vpiSys[Task,Func].");
        return -1;
    }
    self->_vpi_systfdata.type = tmp;
    return 0;
}

PyObject * s_pyvpi_systfdata_getsysfunctype(s_pyvpi_systfdata *self, void *closure)
{
    //Get SysTask/Func return type.
     return Py_BuildValue("i",self->_vpi_systfdata.sysfunctype);
}
int        s_pyvpi_systfdata_setsysfunctype(s_pyvpi_systfdata *self, PyObject *value, void *closure)
{
    //Set SysTask/Func return type.
    //Check type, it must be vpiSysTask,vpiSysFunc.
    int tmp;
    if(!PyInt_Check(value)){
        PyErr_SetString(VpiError,
                        "The value must be vpiSysTask, vpi[Int,Real,Time,Sized,SizedSigned]Func.");
        return -1;
    }
    tmp = PyInt_AS_LONG(value);
    if(self->_vpi_systfdata.type == vpiSysTask){
        //Task
        if(tmp != vpiSysTask){
            PyErr_SetString(VpiError,
                            "The value must be vpiSysTask when type is vpiSysTask.");
            return -1;
        }
    }
    else {
        //Function return...
        if( tmp == vpiIntFunc   ||
            tmp == vpiRealFunc  ||
            tmp == vpiTimeFunc  ||
            tmp == vpiSizedFunc ||
            tmp == vpiSizedSignedFunc )
        {
            //dummy code here...
        }
        else {
            PyErr_SetString(VpiError,
                            "The value must be vpi[Int,Real,Time,Sized,"
                            "SizedSigned]Func when type is vpiSysFunc.");
            return -1;
        }
    }
    self->_vpi_systfdata.sysfunctype = tmp;
    return 0;    
}    

PyObject * s_pyvpi_systfdata_gettfname(s_pyvpi_systfdata *self, void *closure)
{
    Py_INCREF(self->tfname);
    return self->tfname;
}
int        s_pyvpi_systfdata_settfname(s_pyvpi_systfdata *self, PyObject *value, void *closure)
{
    char * name;
    if(value == NULL){
        PyErr_SetString(VpiError, "Can't set tfname to None.");
        return -1;
    }

    name = PyString_AsString(value);
    if(strlen(name)<2) {
        PyErr_SetString(VpiError, "The tfname len must big than 1.");
        return -1;
    }
    if(name[0] != '$') {
        PyErr_SetString(VpiError, "The tfname must start with $");
        return -1;
    }
    Py_INCREF(value);
    self->_vpi_systfdata.tfname = name; 
    Py_XDECREF(self->tfname);
    self->tfname = value;
    return 0;
}

PyObject * s_pyvpi_systfdata_getcalltf(s_pyvpi_systfdata *self, void *closure)
{
    Py_INCREF(self->calltf);
    return self->calltf;
}
int        s_pyvpi_systfdata_setcalltf(s_pyvpi_systfdata *self, PyObject *value, void *closure)
{
    if(value == NULL){
        PyErr_SetString(VpiError, "Can't set calltf to None.");
        return -1;
    }
    if (!PyCallable_Check(value)) {
        PyErr_SetString(VpiError, "calltf must be a callable.");
        return -1;
    }
    Py_INCREF(value);    
    Py_XDECREF(self->calltf);
    self->calltf = value;
    return 0; 
}

PyObject * s_pyvpi_systfdata_getcompiletf(s_pyvpi_systfdata *self, void *closure)
{
    Py_INCREF(self->compiletf);
    return self->compiletf;
}

int        s_pyvpi_systfdata_setcompiletf(s_pyvpi_systfdata *self, PyObject *value, void *closure)
{
    if(value == NULL){
        PyErr_SetString(VpiError, "Can't set compiletf to None.");
        return -1;
    }
    if (!PyCallable_Check(value)) {
        PyErr_SetString(VpiError, "calltf must be a callable.");
        return -1;
    }
    Py_INCREF(value);    
    Py_XDECREF(self->compiletf);
    self->compiletf = value;
    return 0;     
}

PyObject * s_pyvpi_systfdata_getsizetf(s_pyvpi_systfdata *self, void *closure)
{
    Py_INCREF(self->sizetf);
    return self->sizetf;
}

int        s_pyvpi_systfdata_setsizetf(s_pyvpi_systfdata *self, PyObject *value, void *closure)
{
    if(value == NULL){
        PyErr_SetString(VpiError, "Can't set sizetf to None.");
        return -1;
    }
    if (!PyCallable_Check(value)) {
        PyErr_SetString(VpiError, "calltf must be a callable.");
        return -1;
    }
    Py_INCREF(value);    
    Py_XDECREF(self->sizetf);
    self->sizetf = value;
    return 0;     
}

PLI_INT32 _calltf(PLI_BYTE8 *self)
{
    p_pyvpi_systfdata _self = (p_pyvpi_systfdata) self;
    PyObject * arglist = Py_BuildValue("(O)",_self);
    PyObject * ans;
    if (!PyCallable_Check(_self->calltf)) 
        return 0;
    ans = PyObject_CallObject(_self->calltf, arglist);
    Py_DECREF(arglist);
    if(ans == NULL) 
        return 0;
    if(PyInt_Check(ans)){   //work with int,size,sizesigned function type.
        return PyInt_AsLong(ans);
    }
    /* TDB
    if(PyFloat_Check(ans)){
        return PyFloat_AsDouble(ans);
    }
    */
    //What is time function???
    return 0;
}

PLI_INT32 _compiletf(PLI_BYTE8 *self)
{
    p_pyvpi_systfdata _self = (p_pyvpi_systfdata) self;
    PyObject * arglist = Py_BuildValue("(O)",_self);
    PyObject * ans;
    if (!PyCallable_Check(_self->compiletf)) 
        return 0;
    ans = PyObject_CallObject(_self->compiletf, arglist);
    Py_DECREF(arglist);
    if(ans == NULL) 
        return 0;
    if(PyInt_Check(ans)){
        return PyInt_AsLong(ans);
    }
    /* TBD
    if(PyFloat_Check(ans)){
        return PyFloat_AsDouble(ans);
    }
    */
    //What is time function???
    return 0;
}

PLI_INT32 _sizetf(PLI_BYTE8 *self)
{
    p_pyvpi_systfdata _self = (p_pyvpi_systfdata) self;
    PyObject * arglist = Py_BuildValue("(O)",_self);
    PyObject * ans;
    if (!PyCallable_Check(_self->sizetf)) 
        return 0;
    ans = PyObject_CallObject(_self->sizetf, arglist);
    Py_DECREF(arglist);
    if(ans == NULL) 
        return 0;
    if(PyInt_Check(ans)){
        return PyInt_AsLong(ans);
    }
    /* TBD
    if(PyFloat_Check(ans)){
        return PyFloat_AsDouble(ans);
    }
    */
    //What is time function???
    return 0;
}

