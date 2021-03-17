#include "WAV.h"
#include "align.h"

int main() {

  std::string early_path = "../clap_early.wav";
  std::string late_path = "../clap_late.wav";
  std::string ref_path = "../clap.wav";

  WAV early_wav;
  WAV late_wav;
  WAV ref_wav;

  early_wav.OpenFile(early_path);
  late_wav.OpenFile(late_path);
  ref_wav.OpenFile(ref_path);

  int early_len = early_wav.GetNumOfSamples();
  int late_len = late_wav.GetNumOfSamples();
  int ref_len   = ref_wav.GetNumOfSamples();

  int length_early = std::min(early_len, ref_len);
  int length_late = std::min(late_len, ref_len);

  printf("length early: %\d\n",length_early);
  printf("length late: %\d\n",length_late);

  int early_ch = early_wav.GetChannels();
  int late_ch = late_wav.GetChannels();
  int ref_ch = ref_wav.GetChannels();

  short* early_buf = new short[early_len*early_ch];
  short* late_buf = new short[late_len*late_ch];
  short* ref_buf = new short[ref_len*ref_ch];

  early_wav.ReadUnit(early_buf, early_len*1);
  late_wav.ReadUnit(late_buf, late_len*1);
  ref_wav.ReadUnit(ref_buf, ref_len*1);

  int delay_early = align::getDelay(early_buf, ref_buf, length_early);
  int delay_late = align::getDelay(late_buf, ref_buf, length_late);

  printf("delay_early : %d\n",delay_early);
  printf("delay_late  : %d\n",delay_late);

  WAV output_early(1, 16000);
  WAV output_late(1, 16000);
  output_early.NewFile("../output_early.wav");
  output_late.NewFile("../output_late.wav");

  short* out_buf_late = new short[late_len + delay_late];
  memset(out_buf_late, 0, sizeof(short) * (late_len + delay_late));
  short* out_buf_early = new short[early_len + delay_early];
  memset(out_buf_early, 0, sizeof(short) * (early_len + delay_early));

  // target is early
  if (delay_late > 0)
    for (int i = 0; i < late_len; i++)
      out_buf_late[ delay_late+i] = late_buf[i];
  // target is late
  else
    for (int i = 0; i < late_len+delay_late; i++)
      out_buf_late[i] = late_buf[-delay_late+i];

  // target is early
  if (delay_early > 0)
    for (int i = 0; i < early_len; i++)
      out_buf_early[ delay_early+i] = early_buf[i];
  // target is late
  else
    for (int i = 0; i < early_len+delay_early; i++)
      out_buf_early[i] = early_buf[-delay_early+i];

  output_late.Append(out_buf_late, late_len + delay_late);
  output_late.Finish();
  output_early.Append(out_buf_early, early_len + delay_early);
  output_early.Finish();

  delete[] late_buf;
  delete[] early_buf;
  delete[] ref_buf;
  delete[] out_buf_late;
  delete[] out_buf_early;
  

  return 0;
}