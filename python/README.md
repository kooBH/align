# Align & Split  

+ Task  
To split input based on the list of references.  

+ Required  
1. input wav file  
2. reference list text file   
3. reference wav file   


+ arguments  
  + path_raw  : path of input wav file  
  + rext_ref  : path of reference list text file  
  + dir_ref   : directory path of reference wav files    
  + dir_out   : directory path for output files   
  
+ example bat file  
```
chcp 65001
python align_n_splut.py --path_raw D:/Data/20220824_glassbreak_backleft_65dB_50seg_13.wav --txt_ref D:/Data/22_08_24_17_59_49_glassbreak_13.txt  --dir_ref "D:/Data/DB/glassbreak"  --dir_out D:/Data/split/parsing/

```