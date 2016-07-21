#ifndef __PYCBDATA_H__
#define __PYCBDATA_H__
#include "Python.h"
#include "structmember.h"

#include "vpi_user.h"
#include "sv_vpi_user.h"

#include "pyconfig.h"
#include "pytime.h"
#include "pyhandle.h"
#include "pyvalue.h"

//typedef struct t_cb_data
//{
//  PLI_INT32    reason;                        /* callback reason */
//  PLI_INT32    (*cb_rtn)(struct t_cb_data *); /* call routine */
//  vpiHandle    obj;                           /* trigger object */
//  p_vpi_time   time;                          /* callback time */
//  p_vpi_value  value;                         /* trigger object value */
//  PLI_INT32    index;                         /* index of the memory word or
//                                                 var select that changed */
//  PLI_BYTE8   *user_data;
//} s_cb_data, *p_cb_data;
/* ------------------------------------------------------------------------- */
//PyObject vpi cbdata struct data.
typedef struct t_pyvpi_cbdata
{
    PyObject_HEAD
    s_cb_data  _vpi_cbdata;         //Struct vpi_cbdata.
    PyObject *callback;             //Callback object
    PyObject *obj_h;                //Object handle
    PyObject *cb_h;                 //Callback return handle
    PyObject *user_data;            //User Object handle
    PyObject *dict;
} s_pyvpi_cbdata, *p_pyvpi_cbdata;

//Method ...
extern void pyvpi_cbdata_Dealloc(p_pyvpi_cbdata self);
extern int  pyvpi_cbdata_Init(p_pyvpi_cbdata self, PyObject *args, PyObject *kwds);
extern PyObject * pyvpi_cbdata_New(PyTypeObject *type, PyObject *args, PyObject *kwds);

//Get/Set Functions ......
//reason
PyObject * s_pyvpi_cbdata_getreason(s_pyvpi_cbdata *self, void *closure);
int        s_pyvpi_cbdata_setreason(s_pyvpi_cbdata *self, PyObject *value, void *closure);
//trigger object
PyObject * s_pyvpi_cbdata_gettrgobj(s_pyvpi_cbdata *self, void *closure);
int        s_pyvpi_cbdata_settrgobj(s_pyvpi_cbdata *self, PyObject *value, void *closure);
//trigger time
PyObject * s_pyvpi_cbdata_gettime(s_pyvpi_cbdata *self, void *closure);
int        s_pyvpi_cbdata_settime(s_pyvpi_cbdata *self, PyObject *value, void *closure);
//trigger value
PyObject * s_pyvpi_cbdata_getvalue(s_pyvpi_cbdata *self, void *closure);
int        s_pyvpi_cbdata_setvalue(s_pyvpi_cbdata *self, PyObject *value, void *closure);
//index
PyObject * s_pyvpi_cbdata_getindex(s_pyvpi_cbdata *self, void *closure);
int        s_pyvpi_cbdata_setindex(s_pyvpi_cbdata *self, PyObject *value, void *closure);
//callback
PyObject * s_pyvpi_cbdata_getcallback(s_pyvpi_cbdata *self, void *closure);
int        s_pyvpi_cbdata_setcallback(s_pyvpi_cbdata *self, PyObject *value, void *closure);

/* _pyvpi_cb_rtn(p_cb_data) :
 * Function for callback.
 */
extern PLI_INT32 _pyvpi_cb_rtn(p_cb_data data);


#endif
