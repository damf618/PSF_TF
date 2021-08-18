





import numpy as np
import scipy.signal as sc
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Configuracion de las Seniales 
#--------------------------------------
fs         = 8000
N          = 8000
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


time_signals1 = np.zeros(N).astype("complex")
time_signals2 = np.zeros(N).astype("complex")
time_signals3 = np.zeros(N).astype("complex")
time_signals4 = np.zeros(N).astype("complex")
time_signals5 = np.zeros(N).astype("complex")
time_signals6 = np.zeros(N).astype("complex")

env_signals1 = np.zeros(N).astype("complex")
env_signals2 = np.zeros(N).astype("complex")
env_signals3 = np.zeros(N).astype("complex")
env_signals4 = np.zeros(N).astype("complex")
env_signals5 = np.zeros(N).astype("complex")
env_signals6 = np.zeros(N).astype("complex")

#--------------------------------------

#------------Escpectro--------------------------
#--------------------------------------
#NO GRAPH
fftData =   np.fft.fft(signal)
print(len(fftData))

#GRAPH
fftDataGraph =   np.abs(1/N*fftData)**2
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


"""
MODULO 2 ENVELOPE
"""

# TRANSFORMADA INVERSA DE FOURIER
time_signal   = np.fft.ifft(fftData)

time_signals1 = np.fft.ifft(segmented_signals1)
time_signals2 = np.fft.ifft(segmented_signals2)
time_signals3 = np.fft.ifft(segmented_signals3)
time_signals4 = np.fft.ifft(segmented_signals4)
time_signals5 = np.fft.ifft(segmented_signals5)
time_signals6 = np.fft.ifft(segmented_signals6)
    
# FILTRO PASABAJO QUE SEA LA MITAD DE UN HANNING
# EL filtro se especifica como la mitad de una ventana de Hanning con 0.4 segundos...
# fs = 8000Hz, por lo que mi filtro debe ser 0.4 * 8000 = 3200 muestras...
M = 3200
filter_h = np.hanning(M)
filter_h = filter_h[int(M/2):]
M=int(M/2)

#Teorema de Convolucion...
#Zero_Padding
#Nuevo Largo: N+M-1 -> 8000+1600-1 = 3599
time_signal_ext   = np.concatenate((time_signal,np.zeros(M-1)))

time_signals1_ext = np.concatenate((time_signals1,np.zeros(M-1)))
time_signals2_ext = np.concatenate((time_signals2,np.zeros(M-1)))
time_signals3_ext = np.concatenate((time_signals3,np.zeros(M-1)))
time_signals4_ext = np.concatenate((time_signals4,np.zeros(M-1)))
time_signals5_ext = np.concatenate((time_signals5,np.zeros(M-1)))
time_signals6_ext = np.concatenate((time_signals6,np.zeros(M-1)))

filter_h     = np.concatenate((filter_h,np.zeros(N-1)))

extra_t = np.arange(N,N+M-1,1)
extra_t = extra_t / fs
copyData = np.concatenate((tData,extra_t))

figw    = plt.figure ( 10 )
figwt      = figw.add_subplot ( 1,1,1  )
origfftLn,  = plt.plot( copyData,filter_h,'b-o',linewidth=4,alpha=0.5,label="original") 
figwt.grid( True )

#
# RECTIFIACION DE ONDA COMPLETA

for i in range (N+M-1):
    if(time_signal_ext[i]<0):
        time_signal_ext[i] = -1*time_signal_ext[i]

    if(time_signals1_ext[i]<0):
        time_signals1_ext[i] = -1*time_signals1_ext[i]

    if(time_signals2_ext[i]<0):
        time_signals2_ext[i] = -1*time_signals2_ext[i]

    if(time_signals3_ext[i]<0):
        time_signals3_ext[i] = -1*time_signals3_ext[i]

    if(time_signals4_ext[i]<0):
        time_signals4_ext[i] = -1*time_signals4_ext[i]

    if(time_signals5_ext[i]<0):
        time_signals5_ext[i] = -1*time_signals5_ext[i]
        
    if(time_signals6_ext[i]<0):
        time_signals6_ext[i] = -1*time_signals6_ext[i]
        

# FFT
env_signal = np.fft.fft(time_signal_ext)

env_signals1 = np.fft.fft(time_signals1_ext)
env_signals2 = np.fft.fft(time_signals2_ext)
env_signals3 = np.fft.fft(time_signals3_ext)
env_signals4 = np.fft.fft(time_signals4_ext)
env_signals5 = np.fft.fft(time_signals5_ext)
env_signals6 = np.fft.fft(time_signals6_ext)

filter_h     = np.fft.fft(filter_h)

#Teorema de Convolucion
K=1/480*(0.515/0.55)
env_signal   = np.fft.ifft(K*env_signal*filter_h)

env_signals1 = np.fft.ifft(K*env_signals1*filter_h)
env_signals2 = np.fft.ifft(K*env_signals2*filter_h)
env_signals3 = np.fft.ifft(K*env_signals3*filter_h)
env_signals4 = np.fft.ifft(K*env_signals4*filter_h)
env_signals5 = np.fft.ifft(K*env_signals5*filter_h)
env_signals6 = np.fft.ifft(K*env_signals6*filter_h)

#GRAFICAS

fig    = plt.figure ( 1 )
fig.suptitle('Paso 1: Segmentacion de la senial', fontsize=16)

# FFT ORIGINAL DE LA SIGNAL
origfft     = fig.add_subplot ( 2,1,1 )
origfftLn,  = plt.plot( fData,fftDataGraph,'b-o',linewidth=4,alpha=0.5,label="original") 
origfft.grid( True )

n_data = int(N/2)
print(n_data)
print(fData[:n_data])

# FFT SEGMENTADA DE LA SIGNAL
fftAxe      = fig.add_subplot ( 2,1,2  )
ffts1Ln,    = plt.plot( fData[:n_data],np.abs(1/N*segmented_signals1[:n_data])**2,'r',linewidth = 5 , label="cutoff 1" )
ffts2Ln,    = plt.plot( fData[:n_data],np.abs(1/N*segmented_signals2[:n_data])**2,'g',linewidth = 5 , label="cutoff 2" )
ffts3Ln,    = plt.plot( fData[:n_data],np.abs(1/N*segmented_signals3[:n_data])**2,'b',linewidth = 5 , label="cutoff 3" )
ffts4Ln,    = plt.plot( fData[:n_data],np.abs(1/N*segmented_signals4[:n_data])**2,'c',linewidth = 5 , label="cutoff 4" )
ffts5Ln,    = plt.plot( fData[:n_data],np.abs(1/N*segmented_signals5[:n_data])**2,'m',linewidth = 5 , label="cutoff 5" )
ffts6Ln,    = plt.plot( fData[:n_data],np.abs(1/N*segmented_signals6[:n_data])**2,'k',linewidth = 5 , label="cutoff 6" )
fftAxe.grid( True )

#FIGURA 2 
fig2    = plt.figure ( 2 )
fig2.suptitle('Paso 2: Descomposicion de la senial', fontsize=16)

# IFFT ORIGINAL DE LA SIGNAL
origifft     = fig2.add_subplot ( 3,1,1 )
origifft.set_title("Senial Original IFFT",rotation=0,fontsize=10,va="center")
origifftLn,  = plt.plot( tData,time_signal,'b-o',linewidth=4,alpha=0.5,label="time inverse") 
origifftLn1, = plt.plot( tData,time_signals6,'k',linewidth=1,label="time_s 6" )
origifft.set_xlim ( 0    ,0.06 )
origifft.grid( True )


# IFFT SEGMENTADA DE LA SIGNAL
ifftAxe1      = fig2.add_subplot ( 3,3,4 )
ifftAxe1.set_title("Senial Segmentada1 IFFT",rotation=0,fontsize=10,va="center")
iffts1Ln,    = plt.plot( tData,time_signals1,'r',linewidth = 5 , label="time_s 1" )
ifftAxe1.set_xlim ( 0    ,0.02 )
ifftAxe1.grid( True )

ifftAxe2      = fig2.add_subplot ( 3,3,5 )
ifftAxe2.set_title("Senial Segmentada2 IFFT",rotation=0,fontsize=10,va="center")
iffts2Ln,    = plt.plot( tData,time_signals2,'g',linewidth = 5 , label="time_s 2" )
ifftAxe2.set_xlim ( 0    ,0.02 )
ifftAxe2.grid( True )

ifftAxe3      = fig2.add_subplot ( 3,3,6 )
ifftAxe3.set_title("Senial Segmentada3 IFFT",rotation=0,fontsize=10,va="center")
iffts3Ln,    = plt.plot( tData,time_signals3,'b',linewidth = 5 , label="time_s 3" )
ifftAxe3.set_xlim ( 0    ,0.02 )
ifftAxe3.grid( True )

ifftAxe4      = fig2.add_subplot ( 3,3,7 )
ifftAxe4.set_title("Senial Segmentada4 IFFT",rotation=0,fontsize=10,va="center")
iffts4Ln,    = plt.plot( tData,time_signals4,'c',linewidth = 5 , label="time_s 4" )
ifftAxe4.set_xlim ( 0    ,0.02 )
ifftAxe4.grid( True )

ifftAxe5      = fig2.add_subplot ( 3,3,8 )
ifftAxe5.set_title("Senial Segmentada5 IFFT",rotation=0,fontsize=10,va="center")
iffts5Ln,    = plt.plot( tData,time_signals5,'m',linewidth = 5 , label="time_s 5" )
ifftAxe5.set_xlim ( 0    ,0.02 )
ifftAxe5.grid( True )

ifftAxe6      = fig2.add_subplot ( 3,3,9 )
ifftAxe6.set_title("Senial Segmentada6 IFFT",rotation=0,fontsize=10,va="center")
iffts6Ln,    = plt.plot( tData,time_signals6,'k',linewidth = 5 , label="time_s 6" )
ifftAxe6.set_xlim ( 0    ,0.02 )
ifftAxe6.grid( True )

extra_t = np.arange(N,N+M-1,1)
extra_t = extra_t/fs
tData = np.concatenate((tData,extra_t)) 

#FIGURA 3 
fig3    = plt.figure ( 3 )
fig3.suptitle('Paso 2: Envolvente de la senial', fontsize=16)

# ENV ORIGINAL DE LA SIGNAL
origenv     = fig3.add_subplot ( 3,1,1 )
origenv.set_title("Envolvente Senial Original",rotation=0,fontsize=10,va="center")
difLn,  = plt.plot( tData,time_signal_ext,'y',linewidth=4,alpha=0.2,label="time inverse")
origenvLn,  = plt.plot( tData,np.real(env_signal),'b-o',linewidth=4,alpha=0.5,label="time inverse") 
origenv.set_xlim ( ((M-1)/2)/fs    ,(N+(M-1)/2)/fs )
origenv.grid( True )


# ENV SEGMENTADA DE LA SIGNAL
envAxe1      = fig3.add_subplot ( 3,3,4 )
envAxe1.set_title("Envolvente Senial Segmentada1",rotation=0,fontsize=10,va="center")
envs1Ln,    = plt.plot( tData,np.real(env_signals1),'r',linewidth = 5 , label="time_s 1" )
dif1Ln,    = plt.plot( tData,time_signals1_ext,'b',linewidth = 1 ,alpha=0.2, label="time_s 1" )
envAxe1.set_xlim ( ((M-1)/2)/fs    ,(N+(M-1)/2)/fs )
#envAxe1.set_xlim ( 0    ,0.02 )
envAxe1.grid( True )

envAxe2      = fig3.add_subplot ( 3,3,5 )
envAxe2.set_title("Envolvente Senial Segmentada2",rotation=0,fontsize=10,va="center")
envs2Ln,    = plt.plot( tData,np.real(env_signals2),'g',linewidth = 5 , label="time_s 2" )
#envAxe2.set_xlim ( 0    ,0.02 )
envAxe2.grid( True )

envAxe3      = fig3.add_subplot ( 3,3,6 )
envAxe3.set_title("Envolvente Senial Segmentada3",rotation=0,fontsize=10,va="center")
envs3Ln,    = plt.plot( tData,np.real(env_signals3),'b',linewidth = 5 , label="time_s 3" )
#envAxe3.set_xlim ( 0    ,0.02 )
envAxe3.grid( True )

envAxe4      = fig3.add_subplot ( 3,3,7 )
origenv.set_title("Envolvente Senial Segmentada4",rotation=0,fontsize=10,va="center")
envs4Ln,    = plt.plot( tData,np.real(env_signals4),'c',linewidth = 5 , label="time_s 4" )
#envAxe4.set_xlim ( 0    ,0.02 )
envAxe4.grid( True )

envAxe5      = fig3.add_subplot ( 3,3,8 )
origenv.set_title("Envolvente Senial Segmentada5",rotation=0,fontsize=10,va="center")
envs5Ln,    = plt.plot( tData,np.real(env_signals5),'m',linewidth = 5 , label="time_s 5" )
#envAxe5.set_xlim ( 0    ,0.02 )
envAxe5.grid( True )

envAxe6      = fig3.add_subplot ( 3,3,9 )
origenv.set_title("Envolvente Senial Segmentada6",rotation=0,fontsize=10,va="center")
envs6Ln,    = plt.plot( tData,np.real(env_signals6),'k',linewidth = 5 , label="time_s 6" )
#envAxe6.set_xlim ( 0    ,0.02 )
envAxe6.grid( True )


#FIGURA 4 
fig4    = plt.figure ( 4 )
fig4.suptitle('Paso 2: Rectificacion de Onda Completa de la senial', fontsize=16)

# IFFT ORIGINAL DE LA SIGNAL
dif     = fig4.add_subplot ( 3,1,1 )
dif.set_title("Senial Original IFFT",rotation=0,fontsize=10,va="center")
difLn,  = plt.plot( tData,time_signal_ext,'b-o',linewidth=4,alpha=0.5,label="time inverse") 
dif.set_xlim ( 0    ,0.06 )
dif.grid( True )


# IFFT SEGMENTADA DE LA SIGNAL
dif1      = fig4.add_subplot ( 3,3,4 )
dif1.set_title("Senial Segmentada1 IFFT",rotation=0,fontsize=10,va="center")
dif1Ln,    = plt.plot( tData,time_signals1_ext,'r',linewidth = 5 , label="time_s 1" )
dif1.set_xlim ( 0    ,0.02 )
dif1.grid( True )

dif2      = fig4.add_subplot ( 3,3,5 )
dif2.set_title("Senial Segmentada2 IFFT",rotation=0,fontsize=10,va="center")
dif2Ln,    = plt.plot( tData,time_signals2_ext,'g',linewidth = 5 , label="time_s 2" )
dif2.set_xlim ( 0    ,0.02 )
dif2.grid( True )

dif3      = fig4.add_subplot ( 3,3,6 )
dif3.set_title("Senial Segmentada3 IFFT",rotation=0,fontsize=10,va="center")
dif3Ln,    = plt.plot( tData,time_signals3_ext,'b',linewidth = 5 , label="time_s 3" )
dif3.set_xlim ( 0    ,0.02 )
dif3.grid( True )

dif4      = fig4.add_subplot ( 3,3,7 )
dif4.set_title("Senial Segmentada4 IFFT",rotation=0,fontsize=10,va="center")
dif4Ln,    = plt.plot( tData,time_signals4_ext,'c',linewidth = 5 , label="time_s 4" )
dif4.set_xlim ( 0    ,0.02 )
dif4.grid( True )

dif5      = fig4.add_subplot ( 3,3,8 )
dif5.set_title("Senial Segmentada5 IFFT",rotation=0,fontsize=10,va="center")
dif5Ln,    = plt.plot( tData,time_signals5_ext,'m',linewidth = 5 , label="time_s 5" )
dif5.set_xlim ( 0    ,0.02 )
dif5.grid( True )

dif6      = fig4.add_subplot ( 3,3,9 )
dif6.set_title("Senial Segmentada6 IFFT",rotation=0,fontsize=10,va="center")
dif6Ln,    = plt.plot( tData,time_signals6_ext,'k',linewidth = 5 , label="time_s 6" )
dif6.set_xlim ( 0    ,0.02 )
dif6.grid( True )






plt.show()

"""

#----------BLACKMAN----------------------------

winData=np.blackman(M)
#winData=np.hamming(M) 
#winData=sc.gaussian(M,20)

winAxe       = fig.add_subplot(3,1,3)
winAxe.set_title("Window",rotation=0,fontsize=10,va="center")
winLn, = plt.plot(nData[:M],winData,'b-o',linewidth=4,alpha=0.5,label="Abs")
winAxe.grid(True)
##------------SIGNAL IFFT-----------------------
fftDataShifted = np.fft.fftshift(fftData)
ifftData       = np.fft.ifft(fftDataShifted)
ifftDataShifted= np.fft.fftshift(ifftData)
ifftDataShifted= ifftDataShifted[N//2-M//2:N//2+M//2]

#------aplico ventaneo ----------------------
#ifftDataShifted*=winData

ifftDataShifted = np.concatenate((ifftDataShifted,np.zeros(N-M)))

ifftAxe = fig.add_subplot(3,1,2)
ifftAxe.set_title("Potencia calculada en Tiempo: {0:.2f}".format(np.sum(np.abs(ifftData)**2)),rotation=0,fontsize=10,va="center")
ifftRLn, = plt.plot(tData,np.real(ifftDataShifted),'b-o' ,linewidth  = 5,alpha = 0.5,label="Signal real")
ifftILn, = plt.plot(tData,np.imag(ifftDataShifted),'r-o' ,linewidth  = 5,alpha = 0.2,label="Signal imag")
fftAxe.grid(True)


#----------FFT----------------------------
fftData        = np.fft.fft(ifftDataShifted)
fftDataShifted = np.fft.fftshift(fftData)

fftAxe       = fig.add_subplot(3,2,2)
fftAxe.set_title("Potencia calculada en fft: {0:.2f}".format(np.sum(np.abs(fftData)**2)/N),rotation=0,fontsize=10,va="center")
penAbsLn, = plt.plot(fData,np.abs(fftDataShifted)**2,'b-o',linewidth=4,alpha=0.5,label="Abs")
#penRLn, = plt.plot(fData,np.real(fftDataShifted),'b-o',linewidth=4,alpha=0.5,label="real")
#penILn  = plt.plot(fData,np.imag(fftDataShifted),'r-o',linewidth=4,alpha=0.2,label="imag")
fftAxe.grid(True)
###--------------------------------------
plt.get_current_fig_manager().window.showMaximized()
plt.show()
"""