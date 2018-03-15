
#include <xdc/runtime/System.h>
#include "Enviroment.h"

#define STACKSIZE           512

Task_Struct         EnvTaskStruct;
Char                EnvTaskStack[STACKSIZE];
Task_Handle         EnvTaskHandle;

Semaphore_Struct    Measure_sem;
Semaphore_Handle    Measure_sem_Handle;

Cliff_Pin   _cliff[]    = {0};
Bum_Pin     _bum[]      = {0};
SizeC       _sizeC      =  0;
SizeB       _sizeB      =  0;
stateC      state_C     =  0;
stateB      state_B     =  0;
bool        TaskFlag    =  false;
void (*Enviroment_fxn)(stateC, stateB);

//                  Interrupción del proceso
//    Interrupción del buffer de sensores Cliff
void EnviromentIntC(unsigned int index)
{
    GPIO_clearInt(index);
//    int l,k;
//    for(l=0, k=0; l<100; l++){
//        if(index){
//            ++k;
//            if(k == 75){
//                int checkC;
//                for(checkC=0; checkC<_sizeC; checkC++){
//                    state_C <<= 1;
//                    state_C += !GPIO_read(_cliff[checkC]);
//                }
//                k = 0;
//                l = 0;
//                break;
//            }
//        }
//        else{
//            k = 0;
//        }
//    }

    /*
     * Checa cual o cuales sensores Cliff se
     * activaron y guarda el estado de todos
     * */
    int checkC;
    for(checkC=0; checkC<_sizeC; checkC++){
        // corre un bit hacia la izquierda
        state_C <<= 1;
        // guarda la lectura del puerto en state_C
        state_C += !GPIO_read(_cliff[checkC]);
    }
    Semaphore_post(Measure_sem_Handle);
}
//    Interrupción del buffer de sensores Bumpers
void EnviromentIntB(unsigned int index)
{
    GPIO_clearInt(index);

    /*
     * Checa cual o cuales sensores Bumpers se
     * activaron y guarda el estado de todos
     * */
    int checkB;
    for(checkB=0; checkB<_sizeB; checkB++){
        // corre un bit hacia la izquierda
        state_B <<= 1;
        // guarda la lectura del puerto en state_B
        state_B += !GPIO_read(_bum[checkB]);
    }
    Semaphore_post(Measure_sem_Handle);
}

// Task
Void EnviroTask()
{
    TASKLOOP:
        while(TaskFlag)
        {
            // Espera a que haya una interrupción
            Semaphore_pend(Measure_sem_Handle, BIOS_WAIT_FOREVER);
                /* Envia el estado de los sensores a
                 * la función declarada por el usuario*/
                Enviroment_fxn(state_C, state_B);
                // Reinicia los estados
                state_C = 0; state_B = 0;
        }
    goto TASKLOOP;
}

//                              Funciones públicas
//  Función de inicio
void Enviroment_init(Cliff_Pin Cliff[], SizeC Size_C, Bum_Pin Bum[], SizeB Size_B,
                     EnviromentEvent Enviroment_Event){

    Task_Params         taskParams;
    Semaphore_Params    semParams;

    _sizeC          = Size_C;
    _sizeB          = Size_B;
    Enviroment_fxn  = Enviroment_Event;

    // Regristra la interrupción del buffer de sensores Cliff
    int i;
    for(i=0; i<Size_C; i++){
        _cliff[i] = Cliff[i];
        GPIO_setCallback(Cliff[i], EnviromentIntC);
        GPIO_enableInt(Cliff[i]);
    }

    // Regristra la interrupción del buffer de sensores Bumpers
    int p;
    for(p=0; p<Size_B; p++){
        _bum[p] = Bum[p];
        GPIO_setCallback(Bum[p], EnviromentIntB);
        GPIO_enableInt(Bum[p]);
    }

    //Configura el Task
    Task_Params_init(&taskParams);
    taskParams.stackSize = STACKSIZE;
    taskParams.stack = &EnvTaskStack;
    Task_construct(&EnvTaskStruct, (Task_FuncPtr)EnviroTask,
                   &taskParams, NULL);

    //Configurar el semáforo
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&Measure_sem, 0, &semParams);
    Measure_sem_Handle = Semaphore_handle(&Measure_sem);
}
//  Activa el Task
void Enviroment_Enable(){
    TaskFlag = true;
}
//  Desactiva el Task
void Enviroment_Disable(){
    TaskFlag = false;
}
