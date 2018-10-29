#ifndef PUERTO_H
#define PUERTO_H
/*******************************************************************************
* CONSTANTES
*******************************************************************************/
#define LUINT16 sizeof(uint16_t)*8
#define LUINT8 sizeof(uint8_t)*8
/*******************************************************************************
* Estas 3 funciones se encargan de enmascar los puertos de distintas maneras,
* su logica entre ellas es muy similar pero defieren en la funcion que usan.
* Se les debe mandar por ASCII el puerto deseado ('a','b','d') y la mascara a
* utilizar.
*******************************************************************************/
void maskToggle(char puerto,int mask); //Cambia al estado opuesto los bits
void maskOff(char puerto,int mask); //Apaga los bits de la mascara
void maskOn(char puerto,int mask); //Prende los bits de la mascara
/*******************************************************************************
* Estas 3 funciones se encargan de enmascar de solo un bit los puertos de
* distinta manera, su logica es muy similar entre ellas pero difieren en la
* operacion que hacen.
*******************************************************************************/
void bitToggle(char puerto,int bit); //Cambia al estado opuesto un bit
void bitClr(char puerto,int bit); //Apaga un bit
void bitSet(char puerto,int bit); //Prende un bit
// bitGet imprime el valor de un bit de un puerto especifico
int bitGet(char puerto,int bit);
#endif

