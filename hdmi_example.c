/**
555 * Example of using the Digilent display drivers for Zybo Z7 HDMI output, with animation
 * Russell Joyce, 11/03/2019
 */

#include <stdio.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xgpiops.h"
#include "zybo_z7_hdmi/display_ctrl.h"

// Frame size (based on 1440x900 resolution, 32 bits per pixel)
#define MAX_FRAME (1440*900)
#define FRAME_STRIDE (1440*4)

DisplayCtrl dispCtrl; // Display driver struct
u32 frameBuf[DISPLAY_NUM_FRAMES][MAX_FRAME] __attribute__((aligned(0x20))); // Frame buffers for video data
void *pFrames[DISPLAY_NUM_FRAMES]; // Array of pointers to the frame buffers

int main(void) {
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
	int x, y;
	u32 stride = dispCtrl.stride / 4;
	u32 width = dispCtrl.vMode.width;
	u32 height = dispCtrl.vMode.height;

	u32 *frame = (u32 *)dispCtrl.framePtr[dispCtrl.curFrame];
	u32 blue;

	int right = 1;
	int down = 1;
	int xpos = 0;
	int ypos = 0;
	u32 buff = dispCtrl.curFrame;


	// Get parameters from display controller struct
		int x_aux, y_aux;


		u32 *frame_aux = (u32 *)dispCtrl.framePtr[dispCtrl.curFrame];
		u32 red;

		int right_aux = 1;
		int down_aux = 1;
		int xpos_aux = 0;
		int ypos_aux = 0;

		
		XGpioPs_Config *GPIO_Config;
		XGpioPs my_Gpio;
		int status;
		
		GPIO_Config =XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
		status = XGpioPs_CfgInitialize(&my_Gpio,GPIO_Config,GPIO_Config->BaseAddr);
		
		//XGpioPs_SetDirectionPin(XGpioPS*InstancePtr,int pin, int Direction);
		
		//XGpioPs_SetOutputEnable(XGpioPS*InstancePtr,u32 Pin,u32 opEnable);
		
		//XGpioPs_WritePin(XGpioPS*InstancePtr, int pin, int Data);
		
		
		
		
	while (1) {
			// Switch the frame we're modifying to be the back buffer (1 to 0, or 0 to 1)
			buff = !buff;
			frame = (u32 *)dispCtrl.framePtr[buff];

			frame_aux = (u32 *)dispCtrl.framePtr[buff];

			// Clear the entire frame to white (inefficient, but it works)
			memset(frame, 0xFF, MAX_FRAME*4);
			memset(frame_aux, 0x70, MAX_FRAME*4);

			// Adjust the position of the square
			if (right) {
				xpos++;
				xpos++;

				if (xpos == width-64){
					right = 0;
			}
				
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

				if (ypos == height-64) {
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


									for (x = xpos; x < xpos+64; x++) {
										for (y = ypos; y < ypos+64; y++) {
											blue = ((x*0xFF) / xpos+64);
											frame[y*stride + x] = (blue << BIT_DISPLAY_BLUE);




										}
									}

			// Adjust the position of the square

					xpos_aux=80;
											ypos_aux=80;

			// Draw RED square on the screen
			for (x_aux = xpos_aux; x_aux < xpos_aux+32; x_aux++) {
				for (y_aux = ypos_aux; y_aux < ypos_aux+20; y_aux++) {
					red = (y_aux*0xFF) / ypos_aux+64;
					frame_aux[y_aux*stride + x_aux] = (red << BIT_DISPLAY_RED);
				}
			}


			// Draw RED square on the screen




			// Flush everything out to DDR from the cache
			Xil_DCacheFlush();

			// Switch active frame to the back buffer (will take place during next vertical blanking period)
			DisplayChangeFrame(&dispCtrl, buff);

			// Wait for the frame to switch before continuing (so after current frame has been drawn)
			DisplayWaitForSync(&dispCtrl);
	}

	return 0;

}
