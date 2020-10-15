#include "project.h"

int main(void)
{    
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 
    
    int i=0;
    for(;;)
    {
        i++;
    }
}

/* [] END OF FILE */
