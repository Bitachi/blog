#include<stdio.h>
#include<math.h>
void key_set(int);
void cipher();
int key_get();
void inv_cipher();
void sb();
void sr();
void mc();
unsigned char gmul(unsigned char a, unsigned char b);
void ark();
void ke();
void inv_sb();
void inv_sr();
void inv_mc();

unsigned char state11,state12,state13,state14,state21,state22,state23,state24,state31,state32,state33,state34,state41,state42,state43,state44;
unsigned char Cstate11,Cstate12,Cstate13,Cstate14,Cstate21,Cstate22,Cstate23,Cstate24,Cstate31,Cstate32,Cstate33,Cstate34,Cstate41,Cstate42,Cstate43,Cstate44;
unsigned char CFstate11,CFstate12,CFstate13,CFstate14,CFstate21,CFstate22,CFstate23,CFstate24,CFstate31,CFstate32,CFstate33,CFstate34,CFstate41,CFstate42,CFstate43,CFstate44;
unsigned char temp11,temp12,temp13,temp14,temp21,temp22,temp23,temp24,temp31,temp32,temp33,temp34,temp41,temp42,temp43,temp44;


unsigned char key11,key12,key13,key14,key21,key22,key23,key24,key31,key32,key33,key34,key41,key42,key43,key44;
unsigned char w[176];

unsigned char keys11[1024],keys12[1024],keys13[1024],keys14[1024],keys21[1024],keys22[1024],keys23[1024],keys24[1024],keys31[1024],keys32[1024],keys33[1024],keys34[1024],keys41[1024],keys42[1024],keys43[1024],keys44[1024];



int main(){
	//initial
  Cstate11=0xda;
  Cstate21=0x70;
  Cstate31=0x38;
  Cstate41=0xe2;
  Cstate12=0x0f;
  Cstate22=0x59;
  Cstate32=0xf5;
  Cstate42=0x44;
  Cstate13=0xb4;
  Cstate23=0x25;
  Cstate33=0xf8;
  Cstate43=0x76;
  Cstate14=0xd0;
  Cstate24=0x4c;
  Cstate34=0xda;
  Cstate44=0x8c;
  CFstate11=0xdc;
  CFstate21=0x70;
  CFstate31=0x38;
  CFstate41=0xe2;
  CFstate12=0x0f;
  CFstate22=0x59;
  CFstate32=0xf5;
  CFstate42=0x14;
  CFstate13=0xb4;
  CFstate23=0x25;
  CFstate33=0xb4;
  CFstate43=0x76;
  CFstate14=0xd0;
  CFstate24=0xe1;
  CFstate34=0xda;
  CFstate44=0x8c;
	key11 = 0x00;
	key21 = 0x01;
	key31 = 0x02;
	key41 = 0x03;
	key12 = 0x04;
	key22 = 0x05;
	key32 = 0x06;
	key42 = 0x07;
	key13 = 0x08;
	key23 = 0x09;
	key33 = 0x0a;
	key43 = 0x0b;
	key14 = 0x0c;
	key24 = 0x0d;
	key34 = 0x0e;
	key44 = 0x0f;

	printf("鍵候補の個数 = %d\n", key_get());

	//printf("PT = 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", state11 ,state21 ,state31 ,state41 ,state12 ,state22 ,state32 ,state42 ,state13 ,state23 ,state33 ,state43 ,state14 ,state24 ,state34 ,state44);
	//printf("IK = 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", key11 ,key21 ,key31 ,key41 ,key12 ,key22 ,key32 ,key42 ,key13 ,key23 ,key33 ,key43 ,key14 ,key24 ,key34 ,key44);

	//暗号化の実行
	//cipher();
	//printf("CT = 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", state11 ,state21 ,state31 ,state41 ,state12 ,state22 ,state32 ,state42 ,state13 ,state23 ,state33 ,state43 ,state14 ,state24 ,state34 ,state44);
}

//keyの値をセット
void key_set(int i){
  	key11 = w[i*16+0];
	key21 = w[i*16+1];
	key31 = w[i*16+2];
	key41 = w[i*16+3];
	key12 = w[i*16+4];
	key22 = w[i*16+5];
	key32 = w[i*16+6];
	key42 = w[i*16+7];
	key13 = w[i*16+8];
	key23 = w[i*16+9];
	key33 = w[i*16+10];
	key43 = w[i*16+11];
	key14 = w[i*16+12];
	key24 = w[i*16+13];
	key34 = w[i*16+14];
	key44 = w[i*16+15];
}

//Figure5 Nr: ラウンド数の最大値
void cipher(){
  int round; //ラウンド数
  int Nr = 10;

  ke(); //KeyExpansion関数、ark()の実行前に鍵の生成を行う必要がある。

  key_set(0); //初回ラウンドのkeyの設定
  ark(); //AddRoundKey関数

  for( round=1; round<=Nr-1; round++){
	if(round==9) state11 += 0x01;
    sb(); //SubByte関数
    sr(); //ShiftRows関数
    mc(); //MixColumns関数
    key_set(round);
    ark();
  }

  sb();
  sr();
  key_set(Nr);
  ark();
}

//鍵を取得する関数
//正しい暗号文と誤り暗号文により、鍵を取得する。
int key_get(){
	int i, j, temp, Cnt=0;
	unsigned char a,b,c,d,e;

	for(i=0; i<0xff; i++){
		key11=(i&0xff);
		for(j=0; j<0xff; j++){
			key21=j&0xff;

			ke(); //KeyExpansion関数、ark()の実行前に鍵の生成を行う必要がある。

			//正しい暗号文の逆計算を行う。
			state11=Cstate11;
			state21=Cstate21;
			state31=Cstate31;
			state41=Cstate41;
			state12=Cstate12;
			state22=Cstate22;
			state32=Cstate32;
			state42=Cstate42;
			state13=Cstate13;
			state23=Cstate23;
			state33=Cstate33;
			state43=Cstate43;
			state14=Cstate14;
			state24=Cstate24;
			state34=Cstate34;
			state44=Cstate44;

			inv_cipher();

			temp11=state11;
			temp21=state21;
			temp31=state31;
			temp41=state41;
			temp12=state12;
			temp22=state22;
			temp32=state32;
			temp42=state42;
			temp13=state13;
			temp23=state23;
			temp33=state33;
			temp43=state43;
			temp14=state14;
			temp24=state24;
			temp34=state34;
			temp44=state44;

			//間違いの暗号文の逆計算を行う。
			state11=CFstate11;
			state21=CFstate21;
			state31=CFstate31;
			state41=CFstate41;
			state12=CFstate12;
			state22=CFstate22;
			state32=CFstate32;
			state42=CFstate42;
			state13=CFstate13;
			state23=CFstate23;
			state33=CFstate33;
			state43=CFstate43;
			state14=CFstate14;
			state24=CFstate24;
			state34=CFstate34;
			state44=CFstate44;

			inv_cipher();

			a = temp11^state11;
			b = temp21^state21;
			c = temp31^state31;
			d = temp41^state41;
			e = temp12^state12;

			//a = temp11;
			//b = temp21;
			c = temp31;
			d = temp41;

			if( (gmul(a,1)==gmul(b,2))){
				keys11[Cnt] = key11;
				keys21[Cnt] = key21;
				keys31[Cnt] = key31;
				keys41[Cnt] = key41;
				keys12[Cnt] = key12;
				keys22[Cnt] = key22;
				keys32[Cnt] = key32;
				keys42[Cnt] = key42;
				keys13[Cnt] = key13;
				keys23[Cnt] = key23;
				keys33[Cnt] = key33;
				keys43[Cnt] = key43;
				keys14[Cnt] = key14;
				keys24[Cnt] = key24;
				keys34[Cnt] = key34;
				keys44[Cnt] = key44;
				Cnt++;
			}
		}
	}

	for(i=0; i<0xff; i++){
		key21=(i&0xff);
		for(j=0; j<0xff; j++){
			key31=j&0xff;

			ke(); //KeyExpansion関数、ark()の実行前に鍵の生成を行う必要がある。

			//正しい暗号文の逆計算を行う。
			state11=Cstate11;
			state21=Cstate21;
			state31=Cstate31;
			state41=Cstate41;
			state12=Cstate12;
			state22=Cstate22;
			state32=Cstate32;
			state42=Cstate42;
			state13=Cstate13;
			state23=Cstate23;
			state33=Cstate33;
			state43=Cstate43;
			state14=Cstate14;
			state24=Cstate24;
			state34=Cstate34;
			state44=Cstate44;

			inv_cipher();

			temp11=state11;
			temp21=state21;
			temp31=state31;
			temp41=state41;
			temp12=state12;
			temp22=state22;
			temp32=state32;
			temp42=state42;
			temp13=state13;
			temp23=state23;
			temp33=state33;
			temp43=state43;
			temp14=state14;
			temp24=state24;
			temp34=state34;
			temp44=state44;

			//間違いの暗号文の逆計算を行う。
			state11=CFstate11;
			state21=CFstate21;
			state31=CFstate31;
			state41=CFstate41;
			state12=CFstate12;
			state22=CFstate22;
			state32=CFstate32;
			state42=CFstate42;
			state13=CFstate13;
			state23=CFstate23;
			state33=CFstate33;
			state43=CFstate43;
			state14=CFstate14;
			state24=CFstate24;
			state34=CFstate34;
			state44=CFstate44;

			inv_cipher();

			a = temp11^state11;
			b = temp21^state21;
			c = temp31^state31;
			d = temp41^state41;
			e = temp12^state12;

			a = temp11;
			//b = temp21;
			//c = temp31;
			d = temp41;

			if( (gmul(b,1)==gmul(c,1))){
				keys11[Cnt] = key11;
				keys21[Cnt] = key21;
				keys31[Cnt] = key31;
				keys41[Cnt] = key41;
				keys12[Cnt] = key12;
				keys22[Cnt] = key22;
				keys32[Cnt] = key32;
				keys42[Cnt] = key42;
				keys13[Cnt] = key13;
				keys23[Cnt] = key23;
				keys33[Cnt] = key33;
				keys43[Cnt] = key43;
				keys14[Cnt] = key14;
				keys24[Cnt] = key24;
				keys34[Cnt] = key34;
				keys44[Cnt] = key44;
				Cnt++;
			}
		}
	}

	for(i=0; i<0xff; i++){
		key31=(i&0xff);
		for(j=0; j<0xff; j++){
			key41=j&0xff;

			ke(); //KeyExpansion関数、ark()の実行前に鍵の生成を行う必要がある。

			//正しい暗号文の逆計算を行う。
			state11=Cstate11;
			state21=Cstate21;
			state31=Cstate31;
			state41=Cstate41;
			state12=Cstate12;
			state22=Cstate22;
			state32=Cstate32;
			state42=Cstate42;
			state13=Cstate13;
			state23=Cstate23;
			state33=Cstate33;
			state43=Cstate43;
			state14=Cstate14;
			state24=Cstate24;
			state34=Cstate34;
			state44=Cstate44;

			inv_cipher();

			temp11=state11;
			temp21=state21;
			temp31=state31;
			temp41=state41;
			temp12=state12;
			temp22=state22;
			temp32=state32;
			temp42=state42;
			temp13=state13;
			temp23=state23;
			temp33=state33;
			temp43=state43;
			temp14=state14;
			temp24=state24;
			temp34=state34;
			temp44=state44;

			//間違いの暗号文の逆計算を行う。
			state11=CFstate11;
			state21=CFstate21;
			state31=CFstate31;
			state41=CFstate41;
			state12=CFstate12;
			state22=CFstate22;
			state32=CFstate32;
			state42=CFstate42;
			state13=CFstate13;
			state23=CFstate23;
			state33=CFstate33;
			state43=CFstate43;
			state14=CFstate14;
			state24=CFstate24;
			state34=CFstate34;
			state44=CFstate44;

			inv_cipher();

			a = temp11^state11;
			b = temp21^state21;
			c = temp31^state31;
			d = temp41^state41;
			e = temp12^state12;

			a = temp11;
			b = temp21;
			//c = temp31;
			//d = temp41;

			if( (gmul(c,3)==gmul(d,1))){
				keys11[Cnt] = key11;
				keys21[Cnt] = key21;
				keys31[Cnt] = key31;
				keys41[Cnt] = key41;
				keys12[Cnt] = key12;
				keys22[Cnt] = key22;
				keys32[Cnt] = key32;
				keys42[Cnt] = key42;
				keys13[Cnt] = key13;
				keys23[Cnt] = key23;
				keys33[Cnt] = key33;
				keys43[Cnt] = key43;
				keys14[Cnt] = key14;
				keys24[Cnt] = key24;
				keys34[Cnt] = key34;
				keys44[Cnt] = key44;
				Cnt++;
			}
		}

	}
  CFstate11=0x1f;
  CFstate21=0x70;
  CFstate31=0x38;
  CFstate41=0xe2;
  CFstate12=0x0f;
  CFstate22=0x59;
  CFstate32=0xf5;
  CFstate42=0xb5;
  CFstate13=0xb4;
  CFstate23=0x25;
  CFstate33=0xec;
  CFstate43=0x76;
  CFstate14=0xd0;
  CFstate24=0x7b;
  CFstate34=0xda;
  CFstate44=0x8c;


	temp = Cnt;
	for(i=0; i<Cnt; i++){
		key11 = keys11[i];
		key21 = keys21[i];
		key31 = keys31[i];
		key41 = keys41[i];
		key12 = keys12[i];
		key22 = keys22[i];
		key32 = keys32[i];
		key42 = keys42[i];
		key13 = keys13[i];
		key23 = keys23[i];
		key33 = keys33[i];
		key43 = keys43[i];
		key14 = keys14[i];
		key24 = keys24[i];
		key34 = keys34[i];
		key44 = keys44[i];

		ke();

		//正しい暗号文の逆計算を行う。
		state11=Cstate11;
		state21=Cstate21;
		state31=Cstate31;
		state41=Cstate41;
		state12=Cstate12;
		state22=Cstate22;
		state32=Cstate32;
		state42=Cstate42;
		state13=Cstate13;
		state23=Cstate23;
		state33=Cstate33;
		state43=Cstate43;
		state14=Cstate14;
		state24=Cstate24;
		state34=Cstate34;
		state44=Cstate44;

		inv_cipher();

		temp11=state11;
		temp21=state21;
		temp31=state31;
		temp41=state41;
		temp12=state12;
		temp22=state22;
		temp32=state32;
		temp42=state42;
		temp13=state13;
		temp23=state23;
		temp33=state33;
		temp43=state43;
		temp14=state14;
		temp24=state24;
		temp34=state34;
		temp44=state44;

		//間違いの暗号文の逆計算を行う。
		state11=CFstate11;
		state21=CFstate21;
		state31=CFstate31;
		state41=CFstate41;
		state12=CFstate12;
		state22=CFstate22;
		state32=CFstate32;
		state42=CFstate42;
		state13=CFstate13;
		state23=CFstate23;
		state33=CFstate33;
		state43=CFstate43;
		state14=CFstate14;
		state24=CFstate24;
		state34=CFstate34;
		state44=CFstate44;

		inv_cipher();

		a = temp11^state11;
		b = temp21^state21;
		c = temp31^state31;
		d = temp41^state41;

		if( (gmul(a,1)==gmul(b,2))
			|| (gmul(b,1)==gmul(c,1))
			|| (gmul(c,3)==gmul(d,1))){
			printf("IK = 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", key11 ,key21 ,key31 ,key41 ,key12 ,key22 ,key32 ,key42 ,key13 ,key23 ,key33 ,key43 ,key14 ,key24 ,key34 ,key44);
			temp++;
		}
		temp--;
	}

	return temp;
}

//復号
void inv_cipher(){
 	int Nr = 10;

  	key_set(Nr); //初回ラウンドのkeyの設定
  	ark(); //AddRoundKey関数
	inv_sr();
	inv_sb();
}

void sb(){
	unsigned char sbox[256] = {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
		0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
		0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
		0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
		0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
		0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
		0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
		0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};

	state11=sbox[state11];
	state12=sbox[state12];
	state13=sbox[state13];
	state14=sbox[state14];
	state21=sbox[state21];
	state22=sbox[state22];
	state23=sbox[state23];
	state24=sbox[state24];
	state31=sbox[state31];
	state32=sbox[state32];
	state33=sbox[state33];
	state34=sbox[state34];
	state41=sbox[state41];
	state42=sbox[state42];
	state43=sbox[state43];
	state44=sbox[state44];
}

void sr(){
	unsigned char tmp;

	state11=state11;
	state12=state12;
	state13=state13;
	state14=state14;
	tmp=state21;
	state21=state22;
	state22=state23;
	state23=state24;
	state24=tmp;
	tmp=state31;
	state31=state33;
	state33=tmp;
	tmp=state32;
	state32=state34;
	state34=tmp;
	tmp=state41;
	state41=state44;
	state44=state43;
	state43=state42;
	state42=tmp;
}

void mc() {
        unsigned char tmp11,tmp12,tmp13,tmp14,tmp21,tmp22,tmp23,tmp24,tmp31,tmp32,tmp33,tmp34,tmp41,tmp42,tmp43,tmp44;

	tmp11=gmul(state11,2)^gmul(state21,3)^gmul(state31,1)^gmul(state41,1);
	tmp21=gmul(state11,1)^gmul(state21,2)^gmul(state31,3)^gmul(state41,1);
	tmp31=gmul(state11,1)^gmul(state21,1)^gmul(state31,2)^gmul(state41,3);
	tmp41=gmul(state11,3)^gmul(state21,1)^gmul(state31,1)^gmul(state41,2);

	tmp12=gmul(state12,2)^gmul(state22,3)^gmul(state32,1)^gmul(state42,1);
	tmp22=gmul(state12,1)^gmul(state22,2)^gmul(state32,3)^gmul(state42,1);
	tmp32=gmul(state12,1)^gmul(state22,1)^gmul(state32,2)^gmul(state42,3);
	tmp42=gmul(state12,3)^gmul(state22,1)^gmul(state32,1)^gmul(state42,2);

	tmp13=gmul(state13,2)^gmul(state23,3)^gmul(state33,1)^gmul(state43,1);
	tmp23=gmul(state13,1)^gmul(state23,2)^gmul(state33,3)^gmul(state43,1);
	tmp33=gmul(state13,1)^gmul(state23,1)^gmul(state33,2)^gmul(state43,3);
	tmp43=gmul(state13,3)^gmul(state23,1)^gmul(state33,1)^gmul(state43,2);

	tmp14=gmul(state14,2)^gmul(state24,3)^gmul(state34,1)^gmul(state44,1);
	tmp24=gmul(state14,1)^gmul(state24,2)^gmul(state34,3)^gmul(state44,1);
	tmp34=gmul(state14,1)^gmul(state24,1)^gmul(state34,2)^gmul(state44,3);
	tmp44=gmul(state14,3)^gmul(state24,1)^gmul(state34,1)^gmul(state44,2);

	state11=tmp11;
	state12=tmp12;
	state13=tmp13;
	state14=tmp14;
	state21=tmp21;
	state22=tmp22;
	state23=tmp23;
	state24=tmp24;
	state31=tmp31;
	state32=tmp32;
	state33=tmp33;
	state34=tmp34;
	state41=tmp41;
	state42=tmp42;
	state43=tmp43;
	state44=tmp44;
}

//gmul
unsigned char gmul(unsigned char a, unsigned char b) {
	unsigned char p = 0;
	unsigned char counter;
	unsigned char hi_bit_set;
	for(counter = 0; counter < 8; counter++) {
		if((b & 1) == 1)
			p ^= a;
		hi_bit_set = (a & 0x80);
		a <<= 1;
		if(hi_bit_set == 0x80)
			a ^= 0x1b;
		b >>= 1;
	}
	return p;
}

void ark() {
	state11 = state11 ^ key11;
	state12 = state12 ^ key12;
	state13 = state13 ^ key13;
	state14 = state14 ^ key14;
	state21 = state21 ^ key21;
	state22 = state22 ^ key22;
	state23 = state23 ^ key23;
	state24 = state24 ^ key24;
	state31 = state31 ^ key31;
	state32 = state32 ^ key32;
	state33 = state33 ^ key33;
	state34 = state34 ^ key34;
	state41 = state41 ^ key41;
	state42 = state42 ^ key42;
	state43 = state43 ^ key43;
	state44 = state44 ^ key44;
}

void ke(){
        unsigned char Rcon[16]={0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36,0x6c,0xd8,0xab,0x4d,0x9a};
	unsigned char sbox[256] = {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
		0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
		0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
		0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
		0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
		0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
		0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
		0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};
	unsigned char temp,temp1,temp2,temp3,temp4;
	int Nb = 4;
	int Nk = 4;
	int Nr = 10;
	int i = 0;

	w[0] = key11;
	w[1] = key21;
	w[2] = key31;
	w[3] = key41;
	w[4] = key12;
	w[5] = key22;
	w[6] = key32;
	w[7] = key42;
	w[8] = key13;
	w[9] = key23;
	w[10] = key33;
	w[11] = key43;
	w[12] = key14;
	w[13] = key24;
	w[14] = key34;
	w[15] = key44;

	i = 4*Nk;
	while(i < 4*Nb*(Nr+1)) {
		temp1 = w[i-4];
                temp2 = w[i-3];
                temp3 = w[i-2];
                temp4 = w[i-1];
		if (i % (4*Nk) == 0){
			//RotWord
			temp = temp1;
			temp1 = temp2;
			temp2 = temp3;
			temp3 = temp4;
			temp4 = temp;
			//SubWord
			temp1 = sbox[temp1];
			temp2 = sbox[temp2];
			temp3 = sbox[temp3];
			temp4 = sbox[temp4];
			//Rcon
			temp1 = temp1 ^ Rcon[i/(4*Nk)];
			temp2 = temp2 ^ 0;
		        temp3 = temp3 ^ 0;
			temp4 = temp4 ^ 0;
		}
		else if (Nk > 6 && (i%Nk) == 4){
			//SubWord
			temp1 = sbox[temp1];
			temp2 = sbox[temp2];
			temp3 = sbox[temp3];
			temp4 = sbox[temp4];
		}
		w[i]=w[i-4*Nk]^temp1;
		w[i+1]=w[i-4*Nk+1]^temp2;
		w[i+2]=w[i-4*Nk+2]^temp3;
		w[i+3]=w[i-4*Nk+3]^temp4;
		i=i+4;
	}
}

void inv_sb(){
    unsigned char inv_s[256] =
    {
        0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
        0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
        0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
        0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
        0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
        0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
        0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
        0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
        0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
        0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
        0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
        0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
        0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
        0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
        0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
    };

	state11=inv_s[state11];
	state12=inv_s[state12];
	state13=inv_s[state13];
	state14=inv_s[state14];
	state21=inv_s[state21];
	state22=inv_s[state22];
	state23=inv_s[state23];
	state24=inv_s[state24];
	state31=inv_s[state31];
	state32=inv_s[state32];
	state33=inv_s[state33];
	state34=inv_s[state34];
	state41=inv_s[state41];
	state42=inv_s[state42];
	state43=inv_s[state43];
	state44=inv_s[state44];
}

void inv_sr(){
	unsigned char tmp;

	state11=state11;
	state12=state12;
	state13=state13;
	state14=state14;
	tmp=state21;
	state21=state24;
	state24=state23;
	state23=state22;
	state22=tmp;
	tmp=state31;
	state31=state33;
	state33=tmp;
	tmp=state32;
	state32=state34;
	state34=tmp;
	tmp=state41;
	state41=state42;
	state42=state43;
	state43=state44;
	state44=tmp;
}

void inv_mc() {
        unsigned char tmp11,tmp12,tmp13,tmp14,tmp21,tmp22,tmp23,tmp24,tmp31,tmp32,tmp33,tmp34,tmp41,tmp42,tmp43,tmp44;

	tmp11=gmul(state11,14)^gmul(state21,11)^gmul(state31,13)^gmul(state41,9);
	tmp21=gmul(state11,9)^gmul(state21,14)^gmul(state31,11)^gmul(state41,13);
	tmp31=gmul(state11,13)^gmul(state21,9)^gmul(state31,14)^gmul(state41,11);
	tmp41=gmul(state11,11)^gmul(state21,13)^gmul(state31,9)^gmul(state41,14);
	tmp12=gmul(state12,14)^gmul(state22,11)^gmul(state32,13)^gmul(state42,9);
	tmp22=gmul(state12,9)^gmul(state22,14)^gmul(state32,11)^gmul(state42,13);
	tmp32=gmul(state12,13)^gmul(state22,9)^gmul(state32,14)^gmul(state42,11);
	tmp42=gmul(state12,11)^gmul(state22,13)^gmul(state32,9)^gmul(state42,14);
	tmp13=gmul(state13,14)^gmul(state23,11)^gmul(state33,13)^gmul(state43,9);
	tmp23=gmul(state13,9)^gmul(state23,14)^gmul(state33,11)^gmul(state43,13);
	tmp33=gmul(state13,13)^gmul(state23,9)^gmul(state33,14)^gmul(state43,11);
	tmp43=gmul(state13,11)^gmul(state23,13)^gmul(state33,9)^gmul(state43,14);
	tmp14=gmul(state14,14)^gmul(state24,11)^gmul(state34,13)^gmul(state44,9);
	tmp24=gmul(state14,9)^gmul(state24,14)^gmul(state34,11)^gmul(state44,13);
	tmp34=gmul(state14,13)^gmul(state24,9)^gmul(state34,14)^gmul(state44,11);
	tmp44=gmul(state14,11)^gmul(state24,13)^gmul(state34,9)^gmul(state44,14);

	state11=tmp11;
	state12=tmp12;
	state13=tmp13;
	state14=tmp14;
	state21=tmp21;
	state22=tmp22;
	state23=tmp23;
	state24=tmp24;
	state31=tmp31;
	state32=tmp32;
	state33=tmp33;
	state34=tmp34;
	state41=tmp41;
	state42=tmp42;
	state43=tmp43;
	state44=tmp44;
}
