/*
   Distantzia-sentsoarekin lan egiteko funtzioak.

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021 
*/

#include "i2c_funtzioak.h"

//unsigned char ds_helb = 0xE4;	//distantzia sentsorearen helbidea

unsigned int Neurketa_berria_hasi(unsigned char sents)
{
    // Neurketa berria martxan jarri.
    // "sents" parametroak sentsorearen I2C helbidea adierazten du
    return LDByteWriteI2C_1(sents, 0x00, 0x51);
}

unsigned int Irakurri_neurria(unsigned char sents, unsigned char *dis)
{
    // Neurtutako distantzia irakurtzeko funtzioa.
    // "sents" parametroak sentsorearen I2C helbidea adierazten du
    // "dis" parametroa taula baten helbidea da, bertan itzultzeko neurtutakoa
    // Neurri bakoitzak bi byte ditu
    return LDByteReadI2C_1(sents, 0x02, dis, 2); //azken parametroa, irakurriko diren byte kopurua dira.
}

void Aldatu_helbidea(unsigned char sents, unsigned char sentsberr)
{
    // I2C helbidea aldatzeko funtzioa.
    // "sents" parametroak sentsorearen I2C helbidea adierazten du
    // "sentsberr" parametroak emango zaion helbide berria adierazten du
    LDByteWriteI2C_1(sents, 0x00, 0xA0);
    LDByteWriteI2C_1(sents, 0x00, 0xAA);
    LDByteWriteI2C_1(sents, 0x00, 0xA5);
    LDByteWriteI2C_1(sents, 0x00, sentsberr);
}

unsigned int Helbide_zehaztu(unsigned char *helbide)
{
    // Konektatuta dagoen sentsoarearen I2C helbidea detektatzeko funtzioa.
    // Sentsoreak erantzuten badu "helbide" parametroa gaurkotzen du
    // helbidearekin eta funtzioak 0 itzultzen du.
    // Ez badu sentsorerik aurkitzen, 1 itzultzen du.

    //irakurri E0 FF bueltatzen badu ez da helbidea
    unsigned int i;
    unsigned int em = 1;
    unsigned char helb_balioa;

    for (i = 0xE0; i < 0xFE; i += 2)
    {
        LDByteReadI2C_1(i, 0x00, &helb_balioa, 1);
        if (helb_balioa != 0xFF)
        {
            *helbide = i;
            em = 0;
            break;
        }
    }
    return em;
}
