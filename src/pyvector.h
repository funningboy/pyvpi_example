#ifndef __PYVECTOR_H__
#define __PYVECTOR_H__
#include "Python.h"
#include "structmember.h"

#include "vpi_user.h"
#include "sv_vpi_user.h"

#include "pyconfig.h"
//typedef struct t_vpi_vecval
//{
//  /* following fields are repeated enough times to contain vector */
//  PLI_UINT32 aval, bval;          /* bit encoding: ab: 00=0, 10=1, 11=X, 01=Z */
//} s_vpi_vecval, *p_vpi_vecval;
/* ------------------------------------------------------------------------- */
//PyObject vpi vector struct data.
typedef struct t_pyvpi_vector
{
    PyObject_HEAD
    s_vpi_vecval  *_vpi_vector; //struct vpi vector point.
    PLI_UINT32  size;
    PLI_UINT32  cache_size;      //Cache size, used to store s_vpi_vecval vector,unit is sizeof(s_vpi_vecval)-> 64bit
    p_vpi_vecval cache_ptr;    
} s_pyvpi_vector, *p_pyvpi_vector;

extern void pyvpi_vector_Dealloc(p_pyvpi_vector self);
extern int  pyvpi_vector_Init(s_pyvpi_vector *self, PyObject *args, PyObject *kwds);
extern PyObject * pyvpi_vector_New(PyTypeObject *type, PyObject *args, PyObject *kwds);

//Misc Functions ... ...
//Update cache if size is changed, return 0 successful, -1 failed.
extern PLI_INT32 pyvpi_vector_update_cache(s_pyvpi_vector *self);

//Get/Set Functions ... ...
//size
PyObject * s_pyvpi_vector_getsize(s_pyvpi_vector *self, void *closure);
int        s_pyvpi_vector_setsize(s_pyvpi_vector *self, PyObject *value, void *closure);
PyObject * s_pyvpi_vector_getvalue(s_pyvpi_vector *self, void *closure);
int        s_pyvpi_vector_setvalue(s_pyvpi_vector *self, PyObject *value, void *closure);


#endif
