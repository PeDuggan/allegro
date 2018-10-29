#include <stdint.h>
#include <stdio.h>
#include "newfile.h"

typedef struct {
  uint8_t   a;
  uint8_t   b;
} byte_t;
typedef union {
  uint16_t   d;
  byte_t    ab;
} acum_t;
//Check info verifica que el bit este denro del margen posible para el puerto
//Devuelve la cantidad de bits del puerto seleccionado
static int checkinfo(char puerto,int bit);
static acum_t registro; //Registro que usan todas las funciones para trabajar
/*******************************************************************************
* Las funciones bitSet,bitClr y bitToggle tienen la misma logica pero se
* diferencian en sus operandos entre puerto y la mascara establecida.
*******************************************************************************/
void bitSet(char puerto,int bit)
{ //Necesitamos hacer un OR entre la mascara y el reistro
    if(checkinfo(puerto,bit))
    {
      if(puerto=='b' || puerto=='B')
      {
        registro.ab.b|=(1<<bit);
      }
      else if(puerto=='a' || puerto=='A')
      {
        registro.ab.a|=(1<<bit);
      }
      else if(puerto=='d' || puerto=='D')
      {
        registro.d|=(1<<bit);
      }
    }
}

void bitClr(char puerto,int bit)
{//Necesitamos hacer un AND entre el registro y el complemento a uno de ma mascara
    if(checkinfo(puerto,bit))
    {
      if(puerto=='b' || puerto=='B')
      {
        registro.ab.b&= ~(1<<bit);
      }
      else if(puerto=='a' || puerto=='A')
      {
        registro.ab.a&= ~(1<<bit);
      }
      else if(puerto=='d' || puerto=='D')
      {
        registro.d&= ~(1<<bit);
      }
    }
}

void bitToggle(char puerto,int bit)
{ //Necesitamos hacer un XOR entre el registro y la mascara
    if(checkinfo(puerto,bit))
    {
      if(puerto=='b' || puerto=='B')
      {
        registro.ab.b^=(1<<bit);
      }
      else if(puerto=='a' || puerto=='A')
      {
        registro.ab.a^=(1<<bit);
      }
      else if(puerto=='d' || puerto=='D')
      {
        registro.d^=(1<<bit);
      }
    }
}

int bitGet(char puerto,int bit)
{//Necesitamos aplicar la mascara para saber el valor del bit deseado
      int copy;    
      if(puerto=='b' || puerto=='B')
      {
        copy=registro.ab.b&(1<<bit);
      }
      else if(puerto=='a' || puerto=='A')
      {
        copy=registro.ab.a&(1<<bit);
      }
      else if(puerto=='d' || puerto=='D')
      {
        copy=registro.d&(1<<bit);
      }
     return copy;
      //Con !!copy obtenemos si su valor es 0 o 1
    

}
/*******************************************************************************
* Las funciones maskOn, maskOff, maskToggle llevan la misma logica pero,
* se diferencian en la funcion que utilizan, es por eso que necesitamos una
* para cada una. Su logica es hacer una mascara bit a bit, en vez de aplicar,
* toda la mascara de una.
* Importante: Para la validez, ponemos 0 ya que la mascara varia dependiendo
* del puerto seleccionado.
*******************************************************************************/
void maskOn(char puerto,int mask)
{
    int largo=checkinfo(puerto,0);
    int count;
    for(count=0;count<=largo;++count)
    {
        if(mask%2)
        {
            bitSet(puerto,count);
        }
        mask/=2;
    }
}

void maskOff(char puerto,int mask)
{ //Hacemos bit a bit la mascara, utilizando las otras funciones
    int largo=checkinfo(puerto,0);
    int count;
    for(count=0;count<=largo;++count)
    {
        if(mask%2)
        {
            bitClr(puerto,count);
        }
        mask/=2;
    }
}

void maskToggle(char puerto,int mask)
{
    int largo=checkinfo(puerto,0);
    int count;
    for(count=0;count<=largo;++count)
    {
        if(mask%2)
        {
            bitToggle(puerto,count);
        }
        mask/=2;
    }
}

static int checkinfo(char puerto,int bit)
{
  int cantidad;
  if(puerto=='b' || puerto=='B' || puerto=='a' || puerto=='A')
  { //Cargamos el largo del registro y verificamos su validez
    cantidad=LUINT8;
    if(bit>LUINT8)
    {
      cantidad=0;
      printf("El numero seleccionado es incorrecto.\n");
    }

  }
  else if(puerto=='d' || puerto=='D')
  { //Cargamos el largo del registro y verificamos su validez
    cantidad=LUINT16;
    if(bit>LUINT16)
    {
      cantidad=0;
      printf("El numero seleccionado es incorrecto.\n");
    }
  }
  else //En este caso el registro seleccionado no existe
  {
    cantidad=0;
    printf("El puerto seleccionado no existe.\n");
  }
  return cantidad;
}

