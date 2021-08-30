from fxpmath import Fxp
import numpy as np

fs = 2793   # Hz
t = 0.4     # segundos

M      = 1
N      = 15
SIGNED = True

POINTS = fs*t

Hanning_filter = np.hanning(POINTS)

#Mitad derecha de un filtro de un filtro Hanning
Hanning_filter = Hanning_filter[int(POINTS/2):]
print(Hanning_filter)


if(SIGNED):
    MIN    = -2**(M-1)
    MAX    = 2**(M-1)-1/2**N
else:
    MIN    = 0
    MAX    = 2**(M)-1/2**N

Q = Fxp(Hanning_filter, signed = SIGNED, n_word = M+N, n_frac  = N,rounding  = "trunc")    # create fixed-point object (3 bit for intefer, 2 for fractional)

#np.savetxt('hanning_window.h', Fxp.bin(Q), newline=',\n',header = 'q15_t hanning[]={',footer='}')   # X is an array

print(len(Q))
#for i in range(len(Hanning_filter)):
#    print("0b{0:}".format(Fxp.hex(Q)[i]))
