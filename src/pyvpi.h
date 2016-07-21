//
#ifndef __PYVPI_H__
#define __PYVPI_H__

#include "Python.h"
#include "vpi_user.h"
#include "sv_vpi_user.h"

#ifdef MS_WINDOWS
#include <io.h>
#else
#include <sys/io.h>
#endif

#include "pyconfig.h"

#include "pyvector.h"
#include "pytime.h"
#include "pystrength.h"
#include "pyhandle.h"
#include "pyvalue.h"
#include "pycbdata.h"
#include "pysystfdata.h"

//static PyObject *SysTfDict;         //This is a dict used to store all system task and function in register.
//static PyObject *CallbackDict;      //This is a dict used to store all register callback function.
/* ------------------------------------------------------------------------- */
extern int pyvpi_CheckError( void );
/* ------------------------------------------------------------------------- */

#endif

