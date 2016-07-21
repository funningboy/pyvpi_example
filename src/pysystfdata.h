#ifndef __PYSYSTFDATA_H__
#define __PYSYSTFDATA_H__
#include "Python.h"
#include "structmember.h"

#include "vpi_user.h"
#include "sv_vpi_user.h"

#include "pyconfig.h"
/* ------------------------------------------------------------------------- */
//typedef struct t_vpi_systf_data
//{
//  PLI_INT32 type;                       /* vpiSysTask, vpiSysFunc */
//  PLI_INT32 sysfunctype;                /* vpiSysTask, vpi[Int,Real,Time,Sized,
//                                                           SizedSigned]Func */
//  PLI_BYTE8 *tfname;                    /* first character must be '$' */
//  PLI_INT32 (*calltf)(PLI_BYTE8 *);
//  PLI_INT32 (*compiletf)(PLI_BYTE8 *);
//  PLI_INT32 (*sizetf)(PLI_BYTE8 *);     /* for sized function callbacks only */
//  PLI_BYTE8 *user_data;
//} s_vpi_systf_data, *p_vpi_systf_data;

//PyObject vpi time struct data.
typedef struct t_pyvpi_systfdata
{
    PyObject_HEAD
    s_vpi_systf_data  _vpi_systfdata;      //vpiHandle
    PyObject* tfname;
    PyObject* calltf;
    PyObject* compiletf;
    PyObject* sizetf;
} s_pyvpi_systfdata, *p_pyvpi_systfdata;

extern void pyvpi_systfdata_Dealloc(p_pyvpi_systfdata self);
extern int  pyvpi_systfdata_Init(s_pyvpi_systfdata *self, PyObject *args, PyObject *kwds);
extern PyObject * pyvpi_systfdata_New(PyTypeObject *type, PyObject *args, PyObject *kwds);

//Misc function
PLI_INT32 _calltf(PLI_BYTE8 *self);
PLI_INT32 _compiletf(PLI_BYTE8 *self);
PLI_INT32 _sizetf(PLI_BYTE8 *self);

PyObject * s_pyvpi_systfdata_gettype(s_pyvpi_systfdata *self, void *closure);
int        s_pyvpi_systfdata_settype(s_pyvpi_systfdata *self, PyObject *value, void *closure);
PyObject * s_pyvpi_systfdata_getsysfunctype(s_pyvpi_systfdata *self, void *closure);
int        s_pyvpi_systfdata_setsysfunctype(s_pyvpi_systfdata *self, PyObject *value, void *closure);
PyObject * s_pyvpi_systfdata_gettfname(s_pyvpi_systfdata *self, void *closure);
int        s_pyvpi_systfdata_settfname(s_pyvpi_systfdata *self, PyObject *value, void *closure);
PyObject * s_pyvpi_systfdata_getcalltf(s_pyvpi_systfdata *self, void *closure);
int        s_pyvpi_systfdata_setcalltf(s_pyvpi_systfdata *self, PyObject *value, void *closure);
PyObject * s_pyvpi_systfdata_getcompiletf(s_pyvpi_systfdata *self, void *closure);
int        s_pyvpi_systfdata_setcompiletf(s_pyvpi_systfdata *self, PyObject *value, void *closure);
PyObject * s_pyvpi_systfdata_getsizetf(s_pyvpi_systfdata *self, void *closure);
int        s_pyvpi_systfdata_setsizetf(s_pyvpi_systfdata *self, PyObject *value, void *closure);

#endif

