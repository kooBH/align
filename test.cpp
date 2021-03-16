#include "WAV.h"
#include "align.h"

int main() {

  //std::string target_path = "../test_AEC_1.wav";
  //std::string ref_path = "../noise_IU_1ch.wav";
  std::string target_path = "../clap_1.wav";
  std::string ref_path = "../clap.wav";

  WAV target_wav;
  WAV ref_wav;

  target_wav.OpenFile(target_path);
  ref_wav.OpenFile(ref_path);

  int target_len = target_wav.GetNumOfSamples();
  int ref_len   = ref_wav.GetNumOfSamples();

  int length = std::min(target_len, ref_len);

  printf("length : %\d\n",length);

  int target_ch = target_wav.GetChannels();
  int ref_ch = ref_wav.GetChannels();

  short* target_buf = new short[length*target_ch];
  short* ref_buf = new short[length*ref_ch];

  //target_wav.ReadUnit(target_buf, length * target_ch);
  target_wav.ReadUnit(target_buf, length * 1);
  //ref_wav.ReadUnit(ref_buf, length * ref_ch);
  ref_wav.ReadUnit(ref_buf, length * 1);

  int delay = align::getDelay(target_buf, ref_buf, length);
  printf("Delay : %d\n",delay);

  WAV output(1, 16000);
  output.NewFile("../output.wav");
  short* out_buf = new short[target_len + delay];
  memset(out_buf, 0, sizeof(short) * (target_len + delay));

  // target is early
  if (delay > 0) {
    for (int i = 0; i < target_len; i++) {
      out_buf[ delay+i] = target_buf[i];
    }
  }
  // target is late
  else {
    for (int i = 0; i < target_len; i++) {
      out_buf[delay+i] = target_buf[i];
    }
  }

  output.Append(out_buf, target_len - delay);
  output.Finish();

  delete[] target_buf;
  delete[] ref_buf;
  delete[] out_buf;
  

  return 0;
}