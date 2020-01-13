******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xgpio.h" //botones,switches,leds y demas
#include "xstatus.h"
#include "sleep.h"
#include "zybo_z7_hdmi/display_ctrl.h" //controladores diligent

// Frame size (based on 1440x900 resolution, 32 bits per pixel)
#define MAX_FRAME (1440*900)
#define FRAME_STRIDE (1440*4)// resolution

DisplayCtrl dispCtrl; // Display driver struct
u32 frameBuf[DISPLAY_NUM_FRAMES][MAX_FRAME] __attribute__((aligned(0x20))); // Frame buffers for video data
void *pFrames[DISPLAY_NUM_FRAMES]; // Array of pointers to the frame buffers


int main()
{


    init_platform();//inicia hardware gpio y uart

    print("Hello World\n\r");


    // Initialise an array of pointers to the 2 frame buffers
    	int i;
    	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
    		pFrames[i] = frameBuf[i];

    	// Initialise the display controller
    	DisplayInitialize(&dispCtrl, XPAR_AXIVDMA_0_DEVICE_ID, XPAR_VTC_0_DEVICE_ID, XPAR_HDMI_AXI_DYNCLK_0_BASEADDR, pFrames, FRAME_STRIDE);

    	// Start with the first frame buffer (of two)
    	DisplayChangeFrame(&dispCtrl, 0);

    	// Set the display resolution
    	DisplaySetMode(&dispCtrl, &VMODE_1440x900);

    	// Enable video output
    	DisplayStart(&dispCtrl);

    	printf("\n\r");
    	printf("HDMI output enabled\n\r");
    	printf("Current Resolution: %s\n\r", dispCtrl.vMode.label);
    	printf("Pixel Clock Frequency: %.3fMHz\n\r", dispCtrl.pxlFreq);
    	printf("Starting animation loop...\n\r");

    	// Get parameters from display controller struct
    //player 1
    	int x, y; //bullet 1 player 1
    	int x_aux, y_aux; //player1

    	int x_cubo,y_cubo; // es el target donde debe caer la bullet

        int p1_b1; // bullet player 1





    	u32 stride = dispCtrl.stride / 4;
    	u32 width = dispCtrl.vMode.width;
    	u32 height = dispCtrl.vMode.height;

	
	//son los frames de cada imagen


    	u32 *frame;
    	//u32 *frame = (u32 *)dispCtrl.framePtr[dispCtrl.curFrame]; //player 1
        u32 *frame_aux;// = (u32 *)dispCtrl.framePtr[dispCtrl.curFrame]; // bullet 1

        u32*cubo;

        u32 buff = dispCtrl.curFrame;


        // player 1 bullet 1 posiciones iniciales
    	int right = 1;
    	int down = 1;
    	int xpos = 0;
    	int ypos = 0;















    		u32 red,green,blue;

// initial player's positions

    	    		//player 1
    	    		int xpos_aux = 700;
    	    	    int ypos_aux = 800;

    	    	    //cubo

    	    	    int xpos_cubo = 1200;
    	    	    int ypos_cubo = 500;

    	    	    int cubo_up;
    	    	    int cubo_right=1;



      //botones,switches(dip),leds uso gneral

    	    		XGpio led,btn,dip;
    	    		int btn_check,dip_check;//dip_check;

    	    		XGpio_Initialize(&btn,XPAR_AXI_GPIO_0_DEVICE_ID); /// se inicializan como puertos de input o output
    	    		XGpio_SetDataDirection(&btn,1,0xffffffff);

    	    		XGpio_Initialize(&dip,XPAR_AXI_GPIO_1_DEVICE_ID);
    	    		    	    		XGpio_SetDataDirection(&dip,1,0xffffffff);



    	    		XGpio_Initialize(&led,XPAR_AXI_GPIO_2_DEVICE_ID);
    	    		    	    		XGpio_SetDataDirection(&led,1,0x00000000);











    	    	while (1) {




    	    		btn_check =XGpio_DiscreteRead(&btn,1); // esta variable es para leer
    	    		dip_check =XGpio_DiscreteRead(&dip,1);

    	    		//xil_printf("btn switch status %x\r\n",btn_check);

    	    			// Switch the frame we're modifying to be the back buffer (1 to 0, or 0 to 1)
    	    			buff = !buff;

    	    			frame = (u32 *)dispCtrl.framePtr[buff]; //player 1 bullet 1


    			 frame_aux = (u32 *)dispCtrl.framePtr[buff]; //player 1


    			 cubo = (u32 *)dispCtrl.framePtr[buff]; //cubo




    			// Clear the entire frame to white (inefficient, but it works)
    			memset(frame, 0xEF, MAX_FRAME*4); // esto es en memoria, si lo quitan no hay imagen
    			memset(frame_aux, 0x70, MAX_FRAME*4);
    			memset(cubo,0x70,MAX_FRAME*4);




///---------------------player 1---------------------------------------///


    			// Adjust the position of bullets

    			if (btn_check==2 ||  p1_b1==1 ){ //solo cuando se presione el boton 2 aparecera la bullet y se quedara asi

    				p1_b1=1;


    				if (dip_check==1){ //con este dip, el cubo rebota a la izquierda

    				if (right) {// describir como se mueve
    								xpos++;
    								xpos++;
    								if ((xpos == width-64)  || (ypos==ypos_aux))
    									right = 0;
    							}
    							else {
    								xpos--;
    								xpos--;
    								if (xpos == 0)
    									right = 1;
    							}
    							if (down) {
    								ypos++;
    								ypos++;
    								if (  (ypos == height-64) || (xpos==xpos_aux) ) {
    									down = 0;
    								}
    							}
    							else {
    								ypos--;
    								ypos--;
    								if (ypos == 0) {
    									down = 1;
    								}
    							}






    				}// end dip 1

    				if (dip_check==8){//con ese dip switch rebota derecha

    				    				if (right) {
    				    								xpos++;
    				    								//xpos++;
    				    								if ((xpos == width-64)  )
    				    									right = 0;
    				    							}
    				    							else {
    				    								//xpos--;
    				    								xpos--;
    				    								if (xpos == 0  || (ypos==ypos_aux))
    				    									right = 1;
    				    							}
    				    							if (down) {
    				    								//ypos++;
    				    								ypos++;
    				    								if (  (ypos == height-64)  ) {
    				    									down = 0;
    				    								}
    				    							}
    				    							else {
    				    								//ypos--;
    				    								ypos--;
    				    								if (ypos == 0  || (xpos==xpos_aux)) {
    				    									down = 1;
    				    								}
    				    							}

    				    							if(right==2){

    				    							}

    				    							if(down==2){

    				    							}






    				    				}// end dip 2


    							// Draw black square on the screen // se dibuja el tamaño y color de la figura,
    							for (x = xpos; x < xpos+64; x++) {
    								for (y = ypos; y < ypos+64; y++) {
    									frame[y*stride + x] = 0; //color negro
    								}
    							}

             // el cubo es el target donde debe llegar la bullet de color negra, este cubo es verde de mismo tamaño y se mueve en diagonal en sentido hacia la izquiera
    							//movimiento cubo
    							if(cubo_right){
    								xpos_cubo++;

    								if(xpos_cubo==1300){

    									cubo_right=0;
    								}

    							}

    							else{
    								xpos_cubo--;

    								if(xpos_cubo==1100){
    									cubo_right=1;
    								}
    							}


    							if(cubo_up){
    								ypos_cubo++;

									if(ypos_cubo==600){
										cubo_up=0;
									}
    							}

    							else{
    								ypos_cubo--;

									if(ypos_cubo==400){
										                cubo_up=1;
																		}
    							}
// cuando la posicion de la bullet y el cubo target son iguales empieza a desaparecer  y se queda en la bullet en la posiicon del cubo

	if (  ( (xpos==xpos_cubo) && (ypos==ypos_cubo) )  || ( (ypos==400)&&(xpos==1200) )  || ( (ypos==600)&&(xpos==1200) ) || ( (ypos==300)&&(xpos==1200) ) || ( (ypos==700)&&(xpos==1200) )||( (ypos==500)&&(xpos==1100) )  ||( (ypos==500)&&(xpos==1300) ) ||( (ypos==500)&&(xpos==1000) )||( (ypos==500)&&(xpos==1400) )    ) {
    			    								xpos=xpos_cubo;
    			    								ypos=xpos_cubo;
    			    								right=2;
    			    								down=2;

    			    							}


    			}



    								    // player's  red rectangule , es lo que mueve el usuario

    									if ( ( btn_check==1)  ) { // describe movimiento
    								    				xpos_aux++;
    								    				xpos_aux++;
    								    				xpos_aux++;//velocidad



    								    				if (xpos_aux == width-64){// cuando alcanza borde regresa
    								    					xpos_aux--;


    								    			}

    								    			}
    								    			else if( btn_check==8  ) {
    								    				xpos_aux--;
    								    				xpos_aux--;
    								    				xpos_aux--;



    								    				if (xpos_aux == 0)
    								    					xpos_aux++;

    								    			}



    			// Draw RED square on the screen

    			for (x_aux = xpos_aux; x_aux < xpos_aux+64; x_aux++) {
    				for (y_aux = ypos_aux; y_aux < ypos_aux+64; y_aux++) {
    					red = (y_aux*0xFF) / ypos_aux+64;
    					frame_aux[y_aux*stride + x_aux] = (red << BIT_DISPLAY_RED); //dibuja el cuadro del usuario, color rojo
    				}
    			}



    			for (x_cubo = xpos_cubo; x_cubo < xpos_cubo+64; x_cubo++) {
    			    				for (y_cubo = ypos_cubo; y_cubo < ypos_cubo+64; y_cubo++) {
    			    					green = ((x*0xFF) / xpos+94);
    			    					cubo[y_cubo*stride + x_cubo] = (green << BIT_DISPLAY_GREEN);//dibuja cuadro target color verde
    			    				}
    			    			}




    			// Flush everything out to DDR from the cache
    			Xil_DCacheFlush();

    			// Switch active frame to the back buffer (will take place during next vertical blanking period)
    			DisplayChangeFrame(&dispCtrl, buff);

    			// Wait for the frame to switch before continuing (so after current frame has been drawn)
    			DisplayWaitForSync(&dispCtrl);





    	    	}
    cleanup_platform();
    return 0;
}


