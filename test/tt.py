import pyvpi
import pyvpi_cons as cons
import Crypto.Cipher.AES
import mmap
import ctypes
import traceback

def getAllHandles(handle,type) :
    ans = []
    iter = pyvpi.iterate(type,handle)
    while True :
        h = pyvpi.scan(iter)
        if not h :
            break
        ans.append(h)
    return ans

def test():
    """
    c = a+b;
    """

    a = pyvpi.handleByName("top.u_pyadaptor.a")
    b = pyvpi.handleByName("top.u_pyadaptor.b")
    c = pyvpi.handleByName("top.u_pyadaptor.c")

    val_a = pyvpi.Value(cons.vpiIntVal)
    val_b = pyvpi.Value(cons.vpiIntVal)
    val_c = pyvpi.Value(cons.vpiIntVal)

    pyvpi.getValue(a,val_a)
    pyvpi.getValue(b,val_b)
    pyvpi.getValue(c,val_c)

    try:
        val_c.value = val_a.value + val_b.value
        pyvpi.putValue(c, val_c)
        pyvpi.printf("py a:{} + b:{} = c:{}\n".format(val_a.value, val_b.value, val_c.value))
    except:
        # skip if value is unknown
        pass

if __name__ == '__main__':
    test()
