/*
hasieratu_pultsadoreak: hasieratu pultsadoreen hankatxoak sarrera gisa
pultsadoreen tratamendua: S4 pultsadorearen inkesta etengabea
S4 sakatuta mantentzen den bitartean, D3 itzalita eta D4 (RA1) piztuta
S4 askatzean, D3 piztu eta D4 itzali

Egileak: Iker Galardi eta Unai Fernandez 
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"

void hasieratu_pultsadoreak()
{
    //Pultsadoreen hankatxoak sarrera gisa hasieratzeko:
    AD1PCFGH = 0xffff; //AN16 eta AN31 arteko sarrera analogikoak digital gisa definitu, arazoak saihesteko
    //sinplifikatzeagatik, denak jarri ditugu digital gisa
    //baina, kasu honetan, AN23 sarrera bakarrik erabiliko dugu (A portuko RA7 bitari dagokiona):
    //AD1PCFGH = 0b 0000 0000 1000 0000 = 0x0080 hau nahikoa litzateke
    //Hankatxo horretan S5 pultsadorea (sarrera) eta D10 LEDa (irteera) daude konektatuta,
    //eta arazoak ematen ditu pultsadoreak sarrera analogikoa baldin bada:
    //detektatzen du S5 pultsadorea beti dagoela sakatuta
    //hau da, tentsio baxua detektatzen du beti, nahiz eta pultsadorea ez sakatu.

    //S3(RD6), S6(RD7) eta S4(RD13) pultsadoreak sarrera gisa:
    TRISDbits.TRISD6 = 1; // 1 = In (sarrera); 0 = Out (irteera)
    TRISDbits.TRISD7 = 1;
    TRISDbits.TRISD13 = 1;
}
