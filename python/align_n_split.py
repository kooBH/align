import librosa
import soundfile as sf
import os
from tqdm.auto import tqdm
import argparse

import numpy as np
from numpy.fft import fft, ifft, fft2, ifft2, fftshift

def cross_correlation_using_fft(x, y):
    f1 = fft(x)
    f2 = fft(np.flipud(y))
    cc = np.real(ifft(f1 * f2))
    return fftshift(cc)

# shift < 0 means that y starts 'shift' time steps before x # shift > 0 means that y starts 'shift' time steps after x
def compute_shift(x, y):
    assert len(x) == len(y)
    c = cross_correlation_using_fft(x, y)
    assert len(c) == len(x)
    zero_index = int(len(x) / 2) - 1
    shift = zero_index - np.argmax(c)
    return shift

sr=16000
idx_align_start = sr

if __name__ == "__main__" : 
    parser = argparse.ArgumentParser()
    parser.add_argument('--path_raw',type=str,required=True)
    parser.add_argument('--txt_ref',type=str,required=True)
    parser.add_argument('--dir_ref',type=str,required=True)
    parser.add_argument('--dir_out',type=str,required=True)
    
    args = parser.parse_args()

    name_data = (args.path_raw).split('/')[-1]
    id_data = name_data.split('.')[0]
    os.makedirs(os.path.join(args.dir_out,id_data),exist_ok=True)

    raw,_ = librosa.load(args.path_raw,sr=sr,mono=False)
     
    ## reference info parsing
    f = open(args.txt_ref,"r")
    list_ref = f.readlines()
    f.close()
    list_ref = [x.strip("\n") for x in list_ref]
    
    idx_data = 0
    # run for references
    for name_ref in tqdm(list_ref) : 
    #for name_ref in list_ref : 
        id_ref = name_ref.split('.')[0]
        
        ref,_ = librosa.load(os.path.join(args.dir_ref,name_ref),sr=sr)
        len_ref = len(ref)
        
        if idx_align_start < 0 :
            idx_align_start = 0
        len_sample = int(len_ref+sr*6.5)
        
        idx_align_end = idx_align_start+len_sample
        data = raw[:,idx_align_start:idx_align_end]
        
        if idx_align_end >= raw.shape[1] : 
            print("ERROR")
            exit(-1)
        
        len_data = data.shape[1]
        
        # padding & get time delay     
        if len_data > len_ref : 
            padded_ref = np.pad(ref,(0,len_data-len_ref))
            tau = compute_shift(data[0,:],padded_ref)
        elif len_data < len_ref :
            data = np.pad(data,(0,len_ref-len_data))
            tau = compute_shift(data[0,:],ref)
              
        #print("{} | {} | {} {} {}".format(idx_data,idx_align_start,tau,len_ref,len_data))
        
        if tau <= 0 :
            data_synced = data[:,-tau:-tau+len_ref]
            idx_align_start = idx_align_start -tau + len_ref
        else : 
            data_synced = data[:,-tau:-tau+len_ref]
            idx_align_start=idx_align_start+(len_data - tau)
            
        
        
        if idx_align_start < 0 :
            idx_align_start = 0
        
        
       
        # output
        #sf.write(os.path.join(args.dir_out,id_data, str(idx_data) +"_tmp.wav"),data[0,:],sr)
        sf.write(os.path.join(args.dir_out,id_data, str(idx_data) +"_data.wav"),data_synced.T,sr)
        sf.write(os.path.join(args.dir_out,id_data, str(idx_data) +"_ref.wav"),ref,sr)
        
        # iter
        idx_data +=1
    