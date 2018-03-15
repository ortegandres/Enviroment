
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

#include "Board.h"
#include "Enviroment.h"

unsigned int bum[] = {Board_BUTTON0,Board_BUTTON1};
unsigned int cliff[] = {Board_PA2};

void gggg(stateC cc, stateB bb);

Int main()
{
    Board_initGeneral();
    Board_initGPIO();

    Enviroment_init(cliff, 1, bum, 2, gggg);
    Enviroment_Enable();

    BIOS_start();

    return(0);
}

void gggg(stateC cc, stateB bb){
    if(bb == 0x02){
        GPIO_toggle(Board_LED0);
    }
    else{
        GPIO_toggle(Board_LED1);
    }
}
