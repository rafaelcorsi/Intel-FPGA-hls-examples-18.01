#include "HLS/hls.h"
#include "HLS/ac_fixed.h"
#include "stdio.h"

// Not recommended Usage example:
// Convert dynamic double/float value inside component
component
ac_fixed<20, 10, true, AC_RND, AC_SAT> ctor_from_double(double d){
  // calling double constructor
  ac_fixed<20, 10, true, AC_RND, AC_SAT> t(d);
  //literal double value for constructor
  //is optimized in HLS i++ compiler
  ac_fixed<20, 10, true, AC_RND, AC_SAT> offset(0.5);
  return t + offset;
}

// Recommended Usage example:
// Convert dynamic double/float value outside component
component
ac_fixed<20, 10, true, AC_RND, AC_SAT> ctor_from_ac_fixed(ac_fixed<20, 10, true, AC_RND, AC_SAT> d){
  // calling ac_fixed constructor
  ac_fixed<20, 10, true, AC_RND, AC_SAT> t(d);
  //literal double value for constructor
  //is optimized in HLS i++ compiler
  ac_fixed<20, 10, true, AC_RND, AC_SAT> offset(0.5);
  return t + offset;
}



// Test bench for program
int main() {
  ac_fixed<20, 10, true, AC_RND, AC_SAT> r;

  ac_fixed<20, 10, true, AC_RND, AC_SAT> arg = 3.1415;
  r = ctor_from_ac_fixed(arg);
  printf("%s\n", r.to_string(AC_DEC).c_str());

  r = ctor_from_double(3.1415);
  printf("%s\n", r.to_string(AC_DEC).c_str());

  return 0;
}
