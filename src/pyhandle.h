#ifndef __PYHANDLE_H__
#define __PYHANDLE_H__
#include "Python.h"
#include "structmember.h"

#include "vpi_user.h"
#include "sv_vpi_user.h"

#include "pyconfig.h"
/* ------------------------------------------------------------------------- */
//PyObject vpi time struct data.
//PyObject vpi time struct data.
typedef struct t_pyvpi_handle
{
    PyObject_HEAD
    vpiHandle   _vpi_handle;      //vpiHandle
    PLI_INT32   is_free;
} s_pyvpi_handle, *p_pyvpi_handle;

extern void pyvpi_handle_Dealloc(p_pyvpi_handle self);
extern int  pyvpi_handle_Init(s_pyvpi_handle *self, PyObject *args, PyObject *kwds);
extern PyObject * pyvpi_handle_New(PyTypeObject *type, PyObject *args, PyObject *kwds);

extern PyObject * _pyvpi_handle_New(vpiHandle handle);


#endif

