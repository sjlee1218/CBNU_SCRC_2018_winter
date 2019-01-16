#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned char BYTE;

long long get_reverse(BYTE temp[96], int offset, int length) {
	unsigned long ref = 0;
	//longitude = temp[31] << 24 | temp[30] << 16 | temp[29] << 8 | temp[28]

	for (int i = 0; i < length; i++) {
		ref |= temp[offset + 4 + i] << 8*i;
	}
	return ref;
}


int main(){
	ifstream in("180108.ubx");
	ofstream out("log.txt");
	if (!in.is_open())
		return 0;
    if (!out.is_open())
        return 0;

    bool successFlag;
	BYTE rx;
	BYTE temp[96]; // RelPosNED
	char ch;

	while(!in.eof()){
		in.get(ch);
		rx= (BYTE)ch;
		if (rx != 0xB5)
			continue;

        in.get(ch);
        rx= (BYTE)ch;
        if (rx != 0x62)
			continue;

		unsigned int check_A = 0;
		unsigned int check_B = 0;

		for (int i = 0; i < 96; i++) {
            in.get(ch);
            temp[i] = (BYTE) ch;
		    check_A = check_A + temp[i];
		    check_A &= 0xff;
		    check_B = check_B + check_A;
		    check_B &= 0xff;
		}

		if (temp[0] != 0x01)
			continue;

		if (temp[1] !=0x07)
			continue;

        BYTE valid = temp[15];
		if ((valid & 0b111) != 0b111)
			return 0;

        in.get(ch);
        rx= (BYTE)ch;
		if (rx != check_A)
			return 0;
		
        in.get(ch);
        rx= (BYTE)ch;
		if (rx != check_B)
			return 0;

		//cout<<fixed;
		//cout.precision(6);
		//cout<< "lon: "<<(double) get_reverse(temp, 24,4)*1e-7<<" lon: "<<(double) get_reverse(temp, 28,4)*1e-7<<endl;
		//cout<<"velN(cm/s): "<<(double) get_reverse(temp, 48,4)/1000<<" velE: "<<(double)get_reverse(temp, 52,4)/1000<<" velD: "<<(double)get_reverse(temp,56,4)/1000<<endl;
		//cout<<"headMot: "<<(double)get_reverse(temp, 64,4)*1e-5<<  " hAcc: "<<(double) get_reverse(temp, 72,4)*1e-5<<endl;

        out<<fixed;
        out.precision(6);
        out<<(double) get_reverse(temp, 24,4)*1e-7<<"\t"<<(double) get_reverse(temp, 28,4)*1e-7<<endl;
        out<<(double) get_reverse(temp, 48,4)/1000<<"\t"<<(double)get_reverse(temp, 52,4)/1000<<"\t"<<(double)get_reverse(temp,56,4)/1000<<endl;
        out<<(double) get_reverse(temp, 60,4)/1000<<"\t"<<(double)get_reverse(temp, 64,4)*1e-5<<  "\t"<<(double) get_reverse(temp, 72,4)*1e-5<<endl;
	}

	in.close();
}
