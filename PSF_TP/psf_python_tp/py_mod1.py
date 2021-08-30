import numpy as np
import scipy.signal as sc
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Configuracion de las Seniales 
#--------------------------------------
fs         = 8000
N          = 16000
M          = 550
cutFrec    = [200,400,800,1600,3200,fs]
f1         = 100
f2         = 300
f3         = 700
f4         = 1200
f5         = 2000
f6         = 3000
f7         = 3600
f8         = 100
f9         = 100
f10        = 100
#--------------------------------------

# Vectores de tiempo 
#--------------------------------------
nData      = np.arange(0,N,1) #arranco con numeros enteros para evitar errores de float
#fData      = nData*(fs/((N)-(N)%2))-fs/2
fData      = nData*(fs/(N))
tData      = nData/fs
#--------------------------------------

# Seniales Generadas
#--------------------------------------
signal_1  = np.sin(2*np.pi*f1*tData)
signal_2  = np.sin(2*np.pi*f2*tData)
signal_3  = np.sin(2*np.pi*f3*tData)
signal_4  = np.sin(2*np.pi*f4*tData)
signal_5  = np.sin(2*np.pi*f5*tData)
signal_6  = np.sin(2*np.pi*f6*tData)
signal_7  = np.sin(2*np.pi*f7*tData)
signal_8  = 0.01*np.sin(2*np.pi*f8*tData)
signal_9  = 0.01*np.sin(2*np.pi*f9*tData)
signal_10 = 0.01*np.sin(2*np.pi*f10*tData)

signal = signal_1 + signal_2 + signal_3 + signal_4 + signal_5 +signal_6 + signal_7 + signal_8 + signal_9 + signal_10

segmented_signals1 = np.zeros(N).astype("complex")
segmented_signals2 = np.zeros(N).astype("complex")
segmented_signals3 = np.zeros(N).astype("complex")
segmented_signals4 = np.zeros(N).astype("complex")
segmented_signals5 = np.zeros(N).astype("complex")
segmented_signals6 = np.zeros(N).astype("complex")
#--------------------------------------

#------------Escpectro--------------------------
#--------------------------------------
#NO GRAPH
fftData =   1/N*np.fft.fft(signal)
#fftData = 1/N*np.fft.fftshift(signal)
print(len(fftData))

#GRAPH
fftDataGraph =   np.abs(fftData)**2
print(len(fftData))


for i in range(len(fData)):
    if(np.abs(fData[i])>cutFrec[0]):
        segmented_signals1[i] = 0
    else:
        segmented_signals1[i] = fftData[i] 

    if((np.abs(fData[i])>cutFrec[1])or(np.abs(fData[i])<cutFrec[0])):
        segmented_signals2[i] = 0
    else:
        segmented_signals2[i] = fftData[i] 

    if((np.abs(fData[i])>cutFrec[2])or(np.abs(fData[i])<cutFrec[1])):
        segmented_signals3[i] = 0
    else:
        segmented_signals3[i] = fftData[i] 
    
    if((np.abs(fData[i])>cutFrec[3])or(np.abs(fData[i])<cutFrec[2])):
        segmented_signals4[i] = 0
    else:
        segmented_signals4[i] = fftData[i] 

    if((np.abs(fData[i])>cutFrec[4])or(np.abs(fData[i])<cutFrec[3])):
        segmented_signals5[i] = 0
    else:
        segmented_signals5[i] = fftData[i] 

    if((np.abs(fData[i])>cutFrec[5])or(np.abs(fData[i])<cutFrec[4])):
        segmented_signals6[i] = 0
    else:
        segmented_signals6[i] = fftData[i] 


fig    = plt.figure ( 1 )
fig.suptitle('Paso 1: Segmentacion de la senial', fontsize=16)

# FFT ORIGINAL DE LA SIGNAL
origfft     = fig.add_subplot ( 2,1,1 )
origfftLn,  = plt.plot( fData,fftDataGraph,'b-o',linewidth=4,alpha=0.5,label="original") 
origfft.grid( True )

n_data = int(N/2)

# FFT SEGMENTADA DE LA SIGNAL
fftAxe      = fig.add_subplot ( 2,1,2  )
ffts1Ln,    = plt.plot( fData[:n_data],np.abs(segmented_signals1[:n_data])**2,'r',linewidth = 5 , label="cutoff 1" )
ffts2Ln,    = plt.plot( fData[:n_data],np.abs(segmented_signals2[:n_data])**2,'g',linewidth = 5 , label="cutoff 2" )
ffts3Ln,    = plt.plot( fData[:n_data],np.abs(segmented_signals3[:n_data])**2,'b',linewidth = 5 , label="cutoff 3" )
ffts4Ln,    = plt.plot( fData[:n_data],np.abs(segmented_signals4[:n_data])**2,'c',linewidth = 5 , label="cutoff 4" )
ffts5Ln,    = plt.plot( fData[:n_data],np.abs(segmented_signals5[:n_data])**2,'m',linewidth = 5 , label="cutoff 5" )
ffts6Ln,    = plt.plot( fData[:n_data],np.abs(segmented_signals6[:n_data])**2,'k',linewidth = 5 , label="cutoff 6" )
fftAxe.grid( True )


plt.show()
