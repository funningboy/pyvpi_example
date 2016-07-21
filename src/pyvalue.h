#ifndef __PYVALUE_H__
#define __PYVALUE_H__
#include "Python.h"
#include "structmember.h"
#include "string.h"
#include <stdarg.h>

#include "vpi_user.h"
#include "sv_vpi_user.h"

#include "pyconfig.h"
#include "pyvector.h"
#include "pytime.h"
#include "pystrength.h"
#include "pyhandle.h"

//typedef struct t_vpi_value
//{
//  PLI_INT32 format; /* vpi[[Bin,Oct,Dec,Hex]Str,Scalar,Int,Real,String,
//                           Vector,Strength,Suppress,Time,ObjType]Val */
//  union
//    {
//      PLI_BYTE8                *str;       /* string value */
//      PLI_INT32                 scalar;    /* vpi[0,1,X,Z] */
//      PLI_INT32                 integer;   /* integer value */
//      double                    real;      /* real value */
//      struct t_vpi_time        *time;      /* time value */
//      struct t_vpi_vecval      *vector;    /* vector value */
//      struct t_vpi_strengthval *strength;  /* strength value */
//      PLI_BYTE8                *misc;      /* ...other */
//    } value;
//} s_vpi_value, *p_vpi_value;
/* ------------------------------------------------------------------------- */
//PyObject vpi time struct data.
typedef struct t_pyvpi_value
{
    PyObject_HEAD
    s_vpi_value  _vpi_value;        //struct vpi time.
    PyObject*    obj;               //maybe time,vector,strength.
    p_pyvpi_handle fixed_handle;    /*  If this value is used to fixed handle,
                                        keep this handle. Only used for mmap.
                                     */
    PLI_INT32   cache_size;         // per byte.
} s_pyvpi_value, *p_pyvpi_value;

extern void pyvpi_value_Dealloc(p_pyvpi_value self);
extern int  pyvpi_value_Init(s_pyvpi_value *self, PyObject *args, PyObject *kwds);
extern PyObject * pyvpi_value_New(PyTypeObject *type, PyObject *args, PyObject *kwds);

//misc
extern void pyvip_value_update_value(s_pyvpi_value *self, s_vpi_value *ovalp, PLI_INT32 blen);
extern PLI_INT32 update_format(p_pyvpi_value self, PLI_INT32 nformat, PyObject* nobj);

//Get/Set Functions ......
//value
PyObject * s_pyvpi_value_getvalue(s_pyvpi_value *self, void *closure);
int        s_pyvpi_value_setvalue(s_pyvpi_value *self, PyObject *value, void *closure);

#endif
