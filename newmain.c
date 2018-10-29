/*******************************************************************************
 * Liberías utilizadas
 ******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> 
#include "newfile.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

/*******************************************************************************
 * Definiciones
 ******************************************************************************/
#define FPS 60.0               //frames p/second
#define LARGO_DIS 951          //ancho de imagen base
#define ALTO_DIS 310           //alto de imagen base
#define MASK  0xFF             //mascara para toggle
#define LED_ALT 86             //altura cada imagen led 
#define LED_LARG 93            //ancho led 
#define ONCOLOR al_color_name("gold") //mascara color encendido
#define OFFCOLOR al_color_name("white") //mascara color apagado

enum MYKEYS                     //mascaras arreglo teclado
	{
		KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_T,KEY_C,KEY_S,KEY_B,KEY_Q
	};	


// Posicion inicial en x de cada led
int posx [8] = {33, 146, 261, 374, 485, 598, 712, 825};
// Posicion inicial en y de cada led
int posy = 97;

/*******************************************************************************
 * función principal, analiza el input y responde segun corresponda
 ******************************************************************************/
int main ()
{
    /***************************************************************************
    * Definiciones estructuras Allegro
    ***************************************************************************/
    ALLEGRO_DISPLAY *display  = NULL;
    ALLEGRO_BITMAP  *port_a   = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    
    //areglo de status de cada tecla
    bool key_pressed[13] = { false, false, false, false,false,false,false,false,false,false,false,false,false };
    
    bool redraw = false;    //flag actualizar display
    bool do_exit = false;   //flag exit
    bool oneclick = false;  //flag para evitar muchas lecturas apretando 1 vez
    bool party = false;     //flag boton B "parpadear"
    unsigned int party_num=0;   //counter para parpadear
    int ledn, i2;               //contadores para moverse por bits 
                                //(diferentes para diferentes funciones)
    
    //ERROR 1, falla Allegro
    if(!al_init()) 
    {
            fprintf(stderr, "failed to initialize allegro!\n");
            return -1;
    }
    //ERROR 2, falla teclado
    if(!al_install_keyboard())
    {
	fprintf(stderr,"failed to initialize the keyboard\n");
	return -1;
    }
    //ERROR 3 falla addon
    if(!al_init_image_addon()) 
    { // ADDON necesario para manejo(no olvidar el freno de mano) de imagenes 
            fprintf(stderr, "failed to initialize image addon !\n");
            return -1;
    }
    //ERROR 4 falla primitivas
    if(!al_init_primitives_addon())
    {
      fprintf(stderr, "failed to initialize primitives!\n");
      return -1;
    }

    //TIMER
    timer=al_create_timer(1.0/FPS);
    
    //ERROR 5 falla timer
    if(!timer)	
    {
            fprintf(stderr, "failed to create timer!\n");
            return -1;
    }
    
    //BITMAP (port_a)
    port_a = al_load_bitmap("PORTA.jpg");
    
    //ERROR 6 falla bitmap
    if(!port_a) 
    {
			al_destroy_timer(timer);
            fprintf(stderr, "failed to load image !\n");
            return 0;
    }
    
    //EVENT QUEUE
    event_queue = al_create_event_queue();
    
    //ERROR 7 falla ev. queue
    if(!event_queue) 
    {
            fprintf(stderr, "failed to create event_queue!\n");
            al_destroy_bitmap(port_a);
            al_destroy_timer(timer);
            return -1;
    }
    
    //DISPLAY
    display = al_create_display(LARGO_DIS,ALTO_DIS);
    
    //ERROR 8 falla display
    if(!display) 
    {
        al_destroy_timer(timer);
        al_destroy_bitmap(port_a);
	al_destroy_event_queue(event_queue);
      fprintf(stderr, "failed to create display!\n");
            return -1;
    }
    //void al_draw_bitmap(ALLEGRO_BITMAP *bitmap, float dx, float dy, int flags) 
    al_draw_bitmap(port_a, 0, 0, 0);//flags(normalmente en cero, ver doc. para rotar etc)
    al_flip_display();
    
    //Inicializa eventos (teclado,timer,display)
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source()); 
    
    //Inicia timer
    al_start_timer(timer);
	while(!do_exit)
		{
			ALLEGRO_EVENT ev;
			if( al_get_next_event(event_queue, &ev) ) //Toma un evento de la cola
			{ 
				if(ev.type == ALLEGRO_EVENT_TIMER)  //entra cada 1/FPS segundos
				{
					int i;
					for(i=0;i<8;i++)        //loop revisa todas las teclas de 0-7
					{
						if(key_pressed[i] && (!oneclick))   //setea bit
						{
							oneclick=true;
							bitSet('a',i);
						}
					}
					if(key_pressed[KEY_T] && (!oneclick)) 
					{
						oneclick=true;
                                                
						for(i=0;i<8;i++)    //loop toggle todos los bits
						{
						bitToggle('a',i);
						}
					}
					else if(key_pressed[KEY_C] && (!oneclick)) 
					{
						oneclick=true;
						maskOff('a',MASK);  //todos bits off
					}
					else if(key_pressed[KEY_S] && (!oneclick)) 
					{
						oneclick=true;
						maskOn('a',MASK);   //todos bits on
					}
					else if(key_pressed[KEY_B] && (!oneclick)) 
					{
						oneclick=true;
						party=!party;       //flag on, resetea counter
                                                party_num=0;
					}
					else if(key_pressed[KEY_Q] && (!oneclick)) 
					{
						oneclick=true;
						do_exit=true;       //fin programa flag
					}
					redraw = true;
				}

				else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
					do_exit = true;             //fin programa flag

				else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)   //apretar tecla   
				{
					switch(ev.keyboard.keycode)     //activa indicador de tecla correspondiente
					{
						case ALLEGRO_KEY_0:
							key_pressed[KEY_0] = true;
							break;

						case ALLEGRO_KEY_1:
							key_pressed[KEY_1] = true;
							break;

						case ALLEGRO_KEY_2: 
							key_pressed[KEY_2] = true;
							break;

						case ALLEGRO_KEY_3:
							key_pressed[KEY_3] = true;
							break;
					
						case ALLEGRO_KEY_4:
							key_pressed[KEY_4] = true;
							break;

						case ALLEGRO_KEY_5:
							key_pressed[KEY_5] = true;
							break;

						case ALLEGRO_KEY_6: 
							key_pressed[KEY_6] = true;
							break;

						case ALLEGRO_KEY_7:
							key_pressed[KEY_7] = true;
							break;

						case ALLEGRO_KEY_B:
							key_pressed[KEY_B] = true;
							break;

						case ALLEGRO_KEY_T:
							key_pressed[KEY_T] = true;
							break;

						case ALLEGRO_KEY_C: 
							key_pressed[KEY_C] = true;
							break;

						case ALLEGRO_KEY_S:
							key_pressed[KEY_S] = true;
							break;

						case ALLEGRO_KEY_Q:
							key_pressed[KEY_Q] = true;
							break;
					}
				}

				else if(ev.type == ALLEGRO_EVENT_KEY_UP)    //soltar tecla
				{
					switch(ev.keyboard.keycode)     //desactiva indicador
					{
						case ALLEGRO_KEY_0:
							key_pressed[KEY_0] = false;
							break;

						case ALLEGRO_KEY_1:
							key_pressed[KEY_1] = false;
							break;

						case ALLEGRO_KEY_2: 
							key_pressed[KEY_2] = false;
							break;

						case ALLEGRO_KEY_3:
							key_pressed[KEY_3] = false;
							break;
					
						case ALLEGRO_KEY_4:
							key_pressed[KEY_4] = false;
							break;

						case ALLEGRO_KEY_5:
							key_pressed[KEY_5] = false;
							break;

						case ALLEGRO_KEY_6: 
							key_pressed[KEY_6] = false;
							break;

						case ALLEGRO_KEY_7:
							key_pressed[KEY_7] = false;
							break;

						case ALLEGRO_KEY_B:
							key_pressed[KEY_B] = false;
							break;

						case ALLEGRO_KEY_T:
							key_pressed[KEY_T] = false;
							break;

						case ALLEGRO_KEY_C: 
							key_pressed[KEY_C] = false;
							break;

						case ALLEGRO_KEY_S:
							key_pressed[KEY_S] = false;
							break;

						case ALLEGRO_KEY_Q:
							key_pressed[KEY_Q] = false;
							break;
					}
                                        oneclick=false; //habilita nuevos imputs
				}
			}
	 
			if(redraw && al_is_event_queue_empty(event_queue)) //actualizar
			{  
				redraw = false; //resetea flag	
                                
                                //PARTY (parpadear)
				if(party)       
                                {                               
                                    party_num++;            //incrementa contador
                                    if(party_num>0xfffff)   
                                    {
                                        party_num=0;        //evitar overflow (+10 dias)
                                    }    
                                    if(!(party_num%45))     //parpadear cada 3/4 segundo
                                    {
                                        
                                        for(i2=0;i2<8;i2++)
                                        {
                                            if(bitGet('a',i2))         //solo leds prendidos
                                            {
                                                if(!(party_num%90))     //invertir color cada 2 ciclos
                                                {
                                                    al_draw_filled_rectangle(posx[i2],posy,posx[i2]+LED_LARG,posy+LED_ALT,ONCOLOR);
                                                }   
                                                else
                                                {
                                                    al_draw_filled_rectangle(posx[i2],posy,posx[i2]+LED_LARG,posy+LED_ALT,OFFCOLOR);
                                                }   
                                                al_flip_display();
                                            }    
                                        } 
                                    }    
                                } 
                                else
                                {    
                                    for(ledn=0;ledn<8;ledn++)   //actualizador display NO party
                                    {
                                            if(bitGet('a',ledn))
                                            {
                                                al_draw_filled_rectangle(posx[ledn],posy,posx[ledn]+LED_LARG,posy+LED_ALT,ONCOLOR);
                                            }
                                            else
                                            {
                                                al_draw_filled_rectangle(posx[ledn],posy,posx[ledn]+LED_LARG,posy+LED_ALT,OFFCOLOR);
                                            }	

                                            al_flip_display();
                                    }
                                }   
			}
		}

  	al_destroy_bitmap(port_a);      
	al_destroy_timer(timer);
	al_destroy_display(display);
	return 0;
}   //FIN

