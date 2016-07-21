import pyvpi
import pyvpi_cons as cons
#import Crypto.Cipher.AES
#from blockcipher import *
from Crypto.Cipher import AES
import mmap
import ctypes
import traceback


def aes_ecb_dec():
    """
    ECB EXAMPLE:
    -------------
    NIST Special Publication 800-38A http://cryptome.org/bcm/sp800-38a.htm#F
    >>> decipher = AES.new('2b7e151628aed2a6abf7158809cf4f3c'.decode('hex'))
    >>> decipher.decrypt(crypted).encode('hex')
    '6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51'
    """

    dec = pyvpi.handleByName("top.u_pyAESECBdec.dec")
    key = pyvpi.handleByName("top.u_pyAESECBdec.key")
    text = pyvpi.handleByName("top.u_pyAESECBdec.text")

    # if val > 32 use HexStr as string type
    val_key  = pyvpi.Value(cons.vpiHexStrVal)
    val_dec  = pyvpi.Value(cons.vpiHexStrVal)
    val_text = pyvpi.Value(cons.vpiHexStrVal)

    pyvpi.getValue(text,val_text)
    pyvpi.getValue(key, val_key)
    pyvpi.getValue(dec, val_dec)

    try:
        decipher = AES.new(str(val_key.value).decode('hex'))
        decrypted = decipher.decrypt(str(val_dec.value).decode('hex'))
        val_text.value = str(decrypted.encode('hex'))
        pyvpi.putValue(text, val_text)
        pyvpi.printf("py key:{} + dec:{} = text:{}\n".format(val_key.value, val_dec.value, val_text.value))
    except:
        print traceback.print_exc()
        # skip if value is unknown
        pass

if __name__ == '__main__':
    aes_ecb_dec()
