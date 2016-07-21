#ifndef __PYSTRENGTH_H__
#define __PYSTRENGTH_H__
#include "Python.h"
#include "structmember.h"

#include "vpi_user.h"
#include "sv_vpi_user.h"

#include "pyconfig.h"
//typedef struct t_vpi_strengthval
//{
//  PLI_INT32 logic;               /* vpi[0,1,X,Z] */
//  PLI_INT32 s0, s1;              /* refer to strength coding below */
//} s_vpi_strengthval, *p_vpi_strengthval;

/* ------------------------------------------------------------------------- */
//PyObject vpi strengthval struct data.
//PyObject vpi strengthval struct data.
typedef struct t_pyvpi_strengthval
{
    PyObject_HEAD
    s_vpi_strengthval  _vpi_strengthval;      //struct vpi strengthval.
} s_pyvpi_strengthval, *p_pyvpi_strengthval;

extern void pyvpi_strengthval_Dealloc(p_pyvpi_strengthval self);
extern int  pyvpi_strengthval_Init(s_pyvpi_strengthval *self, PyObject *args, PyObject *kwds);
extern PyObject * pyvpi_strengthval_New(PyTypeObject *type, PyObject *args, PyObject *kwds);


#endif
