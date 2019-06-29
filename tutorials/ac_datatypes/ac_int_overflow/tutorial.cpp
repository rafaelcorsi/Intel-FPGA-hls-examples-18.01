#include "HLS/hls.h"
#include "HLS/ac_int.h"

typedef ac_int<4, false> index_t; // 4 bits is not sufficient to contain the index, should use ac_int<5, false>
typedef ac_int<33, false> data_t; // 33 bits unsigned is not enough to contain all values

component int table_lookup(index_t index){
	int look_up_table[32] = {323 , 57 , 281 , 334 , 51 , 183 , 324 , 423 , 181 , 241 ,
		317 , 342 , 474 , 446 , 471 , 413 , 186 , 317 , 3 , 226 , 120 , 53 , 413 , 395 ,
		389 , 483 , 22 , 391 , 386 , 347 , 216 , 447};
	return look_up_table[index]++;
}

component data_t counter_add(unsigned val){
	static data_t s = 0;
	s += val;
	return s;
}


int main(){
	// test table_lookup
	for(int i = 0; i < 32; i++){
		table_lookup(i);
	}

	// test counter_add
	data_t vals[32] = {306276630 , 373387776 , 390544026 , 333115691 , 306593041 , 268854808 ,
		427315476 , 296801109 , 375771675, 227658630 , 371749428 , 2889928115 , 376592708 ,
		270348342 , 294783009 , 370753741 , 220792572 , 280505247 , 4294549464 , 352205506 ,
		216029378 , 268818169 , 324114303 , 266343869 , 291090973 , 3093869306 , 317867191 ,
		389202263 , 234008846 , 399966201 , 324184169 , 381343859 };
	for(int i = 0; i < 32; i++){
		data_t k = counter_add(vals[i]);
	}
	return 0;

}
