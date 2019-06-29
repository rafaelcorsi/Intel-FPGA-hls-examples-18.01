// See README for a description of this tutorial
#include "accelerate.h"

component void my_fma(ihc::stream_in<int> &a, ihc::stream_in<int> &b, ihc::stream_in<int> &c, ihc::stream_out<int> &res){
	res.write(a.read()*b.read()+c.read());
}
