#include "WAV.h"

int main() {

  std::string target_path = "";
  std::string ref_path = "";

  WAV target_wav;
  WAV ref_wav;

  short* target_buf;
  short* ref_buf;

  target_wav.OpenFile(target_path);
  target_wav.OpenFile(ref_path);

  return 0;
}