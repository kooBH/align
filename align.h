#pragma once

#include <STFT.h>

namespace align {

  int getDelay(short* target,short * ref, int length) {
    int delay;
    int p, pp;
    int real, imag;
    double tmp_re, tmp_im;
    double max_val = 0;
    int max_idx = 0;
    int center;

    // get leq largest power of 2
    p = std::log2l(length);
    printf("p : %d\n",p);
    pp = std::pow(2, p);
    printf("pp : %d\n",pp);

    // create temporal buffer
    double* target_double = new double[pp+2];
    double* ref_double    = new double[pp+2];
    double* shift_double    = new double[pp+2];
    memset(target_double, 0, sizeof(double) * (pp+2));
    memset(ref_double, 0, sizeof(double) * (pp+2));
    memset(shift_double, 0, sizeof(double) * (pp+2));

    // copy, copy in reverse order
    for (int i = 0; i < pp; i++) {
      target_double[i] = target[i];
      ref_double[i] = ref[i];
    }
    // ?? Normalization ?

    // fft
    Ooura_FFT fft(pp, 1);
    fft.SingleFFT(target_double);
    fft.SingleFFT(ref_double);

    // conjugate ref
    for (int i = 0; i < pp / 2 + 1; i++) {
      real = i + i;
      imag = real + 1;
      ref_double[imag] = -ref_double[imag];
    }

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

    // fft shift
    for (int i = 0; i < pp/2; i++) {
      shift_double[i] = target_double[i + pp / 2];
    }
    for (int i = 0; i < pp/2; i++) {
      shift_double[i+pp/2] = target_double[i];
    }

    // arg max
    for(int i = 0; i < pp; i++) {
      if (shift_double[i] > max_val) {
        max_val = std::abs(shift_double[i]);
        max_idx = i;
      }
    }

    // adjust center
    center = pp / 2 - 1;
    printf("max_val : %lf | max_idx : %d | center : %d\n",max_val,max_idx,center);
    delay = center - max_idx;
    //delay = -max_idx;

    delete[] target_double;
    delete[] ref_double;

    return delay;
  }
};