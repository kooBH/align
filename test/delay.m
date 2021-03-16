close all;

noisy_file_1 = '../test_AEC_1.wav';  
noise_file = '../noise_IU_1ch.wav';    

noisy_1 = audioread(noisy_file_1);
noise = audioread(noise_file);

[c_1,lags_1] = xcorr(noise(:,1),noisy_1(:,1));   
zero_index_1 = int32(length(c_1)/2) - 1;

figure;plot(c_1);title(noisy_file_1);

[max_val_1,max_idx_1] = max(c_1);
disp(max_idx_1-zero_index_1);