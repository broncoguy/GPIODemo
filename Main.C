/* ECE 4510 Lab 1 Code-2 */
#include "stm32f4xx.h"
/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;

int main(void)
{
  
  uint8_t Value = 0xFF;
  const uint16_t Start = GPIO_Pin_6;
  const uint16_t ACK = GPIO_Pin_7;
  const uint16_t Data = GPIO_Pin_15 |
                        GPIO_Pin_14 | 
                        GPIO_Pin_13 |
                        GPIO_Pin_12 |
                        GPIO_Pin_11 |
                        GPIO_Pin_10 | 
                        GPIO_Pin_9 | 
                        GPIO_Pin_8;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s)
       before to branch to application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  

  /* GPIOB Peripheral clock enable */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   /* GPIOC Peripheral clock enable */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
   
  /* Configure PB6 in input mode*/
  GPIO_InitStructure.GPIO_Pin = Start;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure PB7 in output mode */
  GPIO_InitStructure.GPIO_Pin = ACK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure PC15-PC8 in output mode */
  GPIO_InitStructure.GPIO_Pin = Data;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PG6 or PG8 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.  */  
  int num;
  while (1)
  {
    if ((GPIOB ->IDR & Start) == 0x40){ //check to see if start is high
      num = 1; // if high go to case 1
    }
    else {
      num = 0; // if low go to case 0
    }
    switch(num){
      
    case 1:
    GPIOC -> ODR = Value << 8; // shift right 8 times
    
    Value = Value - 0x04; // subract value by 4
    
    GPIOB -> BSRRL = ACK; // set ack high
    
    num = 2; // go to case 2
    break;
    case 2:
      
      if (Start == 0x40){ // determine if start is still high
       GPIOB -> BSRRL = ACK; // if high ack stays high
       num = 2; // keep looping until start goes low
      }
      
      else{
        num = 0; // if start is low go to case 0
        
      }
    
    break;
    
    case 0:
      
    GPIOB -> BSRRH = ACK; // set ack low when start is low
      
       
    break;
    
    default: 
     
    GPIOB -> BSRRH = ACK; // defaut set ack low
    
  }
}
}
