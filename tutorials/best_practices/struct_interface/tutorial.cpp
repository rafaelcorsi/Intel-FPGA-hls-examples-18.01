#include <stdio.h>
#include "IPPacketHeader.h"
#include "HLS/hls.h"
#include "HLS/ac_int.h"

component unsigned long long get_hash_padding(IPPacketHeader_padding packet){
  return packet.hash_val();
}

component unsigned long long get_hash_no_padding(IPPacketHeader_no_padding packet){
  return packet.hash_val();
}

int main(){
  const int SIZE = 32;
  std::string arr[SIZE] = {
    "db762005a5d5415a90f7c2b57c51cf784c7cfe61",
    "bb051e5b4534c265d051904c916e121fb7be6c09",
    "8b77a99640dbe0c10d97af7aba50502cd9f9fc4d",
    "f24cc09d2f60ffbe258fbaf4408c9ec26da5060b",
    "992a3059c5a9ddcd88227c1e66cc2f3e0d66b908",
    "dcb16827c1f443326f02e37dcdd7c4b3ecdc702b",
    "8d83f3eaaf3b5ae1b73454328394180fa1716201",
    "8d729b6a92964c0573010e165748eab9e86df3c3",
    "a97adb77fa35a50ccc0b52675c478ff51dd89e8d",
    "c28d0748f11cc9df172e8de48db126661173f4d4",
    "fea77ecf656534594101eaf11c47bcd3e0c11fa6",
    "c4a42dac964f78e5be9ec03ed09226219b2a57ce",
    "fd7c38038d7f0d4571de9eb35d53279283f8ed8b",
    "e57ca3f56d67083a93de8ff3d87d4590e7d38f33",
    "89ba433dbab14747990777e4af2ff2bb29ba461d",
    "bbbace6c828c57965b9109040ec8fe24424027c5",
    "d89c2d65dec52f62069cea4e1fee9286a9f19349",
    "a8a2a20dc2eea5489297e80b5b3f16ecd9a4e609",
    "f32dd65c6669d0556a22c64a6103b0697f7c243d",
    "ccbd206ccde62351080083e682ae18ef6c3b85c7",
    "982213bf12309b37971dc81093280590ea5af864",
    "b0d26d7b56058274a8967401a40a53537a7fec09",
    "9f65cb4f01d8dcd8bcf80f478fd8b431db0db6c6",
    "9d554b6062aaa4444c6944ddd55ef9ae4e26d939",
    "b4412efec8bcdae3465b7fa9148d7661415afa49",
    "e0b53202a1c9c5a180b3600bb25c2306abba4824",
    "980b084bbff4487e3f54903feadb03915862b52b",
    "d2f61a3aa474597f3b269e628740e5ceb46f4fcf",
    "e59eed0f3ff5b5da1898d2bfc5f6fd48aca1b759",
    "c729eff5c079e342ead55de401274c139c52d4ef",
    "93aa8336ad5556b7a4d288e1b00f25fa4a742867",
    "a6da6b859686e0e3cf3c85a6350cf40a813c4717"
  };

  for(int i = 0; i < SIZE; i++){
    IPPacketHeader_no_padding h1(arr[i].c_str());
    IPPacketHeader_padding h2(arr[i].c_str());

    unsigned long long hash1 = get_hash_no_padding(h1);
    unsigned long long hash2 = get_hash_padding(h2);

    if(hash1 != hash2){
      printf("ERROR\n");
      return 1;
    }
  }
  printf("PASSED\n");
  return 0;
}
