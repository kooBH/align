#pragma once

#include <STFT.h>

namespace align {

  int getDelay(short* target,short * ref, int length) {
    int delay;
    int p, pp;
    int real, imag;
    double tmp_re, tmp_im;
    int max_val = 0;
    int max_idx = 0;
    int center;





    // get leq largest power of 2
    p = std::log2l(length);
    pp = std::pow(2, p);

    // create temporal buffer
    
    double* target_double = new double[pp+2];
    double* ref_double    = new double[pp+2];
    memset(target_double, 0, sizeof(double) * (pp+2));
    memset(ref_double, 0, sizeof(double) * (pp+2));

    // copy, copy in reverse order
    for (int i = 0; i < pp; i++) {
      target_double[i] = target[i];
      ref_double[i] = ref[length - i];
    }

    // fft
    Ooura_FFT fft(pp, 1);
    fft.SingleFFT(target_double);
    fft.SingleFFT(ref_double);

    // mul
    for (int i = 0; i < (pp+2)/2; i++) {
      real = i + i;
      imag = real + 1;

      tmp_re = target_double[real];
      tmp_im = target_double[imag];

      target_double[real] = tmp_re * ref_double[real] - tmp_im * ref_double[imag];
      target_double[imag] = tmp_re * ref_double[imag] + tmp_im * ref_double[real];
    }
    // inverse
    fft.SingleiFFT(target_double);

    // arg max
    for (int i = 0; i < pp; i++) {
      if (target_double[i] > max_val) {
        max_val = target_double[i];
        max_idx = i;
      }
    }

    // adjust center
    center = length / 2 - 1;
    delay = center - max_idx;

    delete[] target_double;
    delete[] ref_double;

    return delay;
  }
};