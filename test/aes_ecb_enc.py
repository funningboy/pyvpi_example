import pyvpi
import pyvpi_cons as cons
#import Crypto.Cipher.AES
#from blockcipher import *
from Crypto.Cipher import AES
import mmap
import ctypes
import traceback


def aes_ecb_enc():
    """
    >>> cipher = AES.new('2b7e151628aed2a6abf7158809cf4f3c'.decode('hex'))
    >>> crypted = cipher.encrypt('6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51'.decode('hex'))
    >>> crypted.encode('hex')
    '3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf'
    """

    text = pyvpi.handleByName("top.u_pyAESECBenc.text")
    key = pyvpi.handleByName("top.u_pyAESECBenc.key")
    enc = pyvpi.handleByName("top.u_pyAESECBenc.enc")

    # if val > 32 use HexStr as string type
    val_text = pyvpi.Value(cons.vpiHexStrVal)
    val_key  = pyvpi.Value(cons.vpiHexStrVal)
    val_enc  = pyvpi.Value(cons.vpiHexStrVal)

    pyvpi.getValue(text,val_text)
    pyvpi.getValue(key, val_key)
    pyvpi.getValue(enc, val_enc)

    try:
        cipher = AES.new(str(val_key.value).decode('hex'))
        crypted = cipher.encrypt(str(val_text.value).decode('hex'))
        val_enc.value = str(crypted.encode('hex'))
        pyvpi.putValue(enc, val_enc)
        pyvpi.printf("py key:{} + text:{} = enc:{}\n".format(val_key.value, val_text.value, val_enc.value))
    except:
        print traceback.print_exc()
        # skip if value is unknown
        pass

if __name__ == '__main__':
    aes_ecb_enc()

