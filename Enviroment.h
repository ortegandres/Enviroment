
#ifndef ENVIROMENT_H_
#define ENVIROMENT_H_

    #include <stdbool.h>
    #include <xdc/std.h>
    #include <ti/sysbios/BIOS.h>
    #include <ti/sysbios/knl/Task.h>
    #include <ti/sysbios/knl/Semaphore.h>
    #include <ti/drivers/GPIO.h>
    #include <ti/drivers/PIN.h>
    #include <ti/sysbios/family/arm/lm4/Timer.h>
    #include <xdc/cfg/global.h>
    #include "Board.h"

    typedef unsigned int Cliff_Pin, SizeC, Bum_Pin, SizeB;
    typedef uint32_t stateC, stateB;
    typedef void (*EnviromentEvent)(stateC, stateB);

    /*                  Función de incio
     * Se establecen los buffers que contendrán
     * los puertos de cada tipo de sensor (Cliff o Bumper)
     * y se declarará el nombre la función que se activará
     * cuando se activen los sensores
     * */
    extern void Enviroment_init(Cliff_Pin Cliff[], SizeC,
                                Bum_Pin Bum[], SizeB,
                                EnviromentEvent);
    /*                  Habilita los sensores */
    extern void Enviroment_Enable();
    /*                  Deshabilita los sensores */
    extern void Enviroment_Disable();

#endif
