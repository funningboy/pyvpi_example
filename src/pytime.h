#ifndef __PYTIME_H__
#define __PYTIME_H__
#include "Python.h"
#include "structmember.h"

#include "vpi_user.h"
#include "sv_vpi_user.h"

#include "pyconfig.h"
//typedef struct t_vpi_time
//{
//  PLI_INT32  type;               /* [vpiScaledRealTime, vpiSimTime,
//                                     vpiSuppressTime] */
//  PLI_UINT32 high, low;          /* for vpiSimTime */
//  double     real;               /* for vpiScaledRealTime */
//} s_vpi_time, *p_vpi_time;

/* ------------------------------------------------------------------------- */
//PyObject vpi time struct data.
typedef struct t_pyvpi_time
{
    PyObject_HEAD
    s_vpi_time  _vpi_time;      //struct vpi time.
} s_pyvpi_time, *p_pyvpi_time;

extern void pyvpi_time_Dealloc(p_pyvpi_time self);
extern int  pyvpi_time_Init(s_pyvpi_time *self, PyObject *args, PyObject *kwds);
extern PyObject * pyvpi_time_New(PyTypeObject *type, PyObject *args, PyObject *kwds);

//Get/Set Functions ......
//value
PyObject * s_pyvpi_time_gettime(s_pyvpi_time *self, void *closure);


#endif
