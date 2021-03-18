#include "Sonar.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void Init_Sonar() 
{
	Input_Capture_Setup();
	Trigger_Setup();
}

void Input_Capture_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enabling GPIO clock

	GPIOA->MODER &= ~GPIO_MODER_MODE0;	// clearing GPIO mode bits
	GPIOA->MODER |= GPIO_MODER_MODE0_1; // setting GPIO mode to alternate functions
	GPIOA->AFR[0] = GPIO_AFRL_AFSEL0_0; // selecting TIM4_CH1 (AF2)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;	// selecting no PUPD

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // enable timer 4
	TIM2->PSC |= 0x0000000FU;			  // setting prescaler value
	TIM2->ARR |= 0xFFFFU;				  // setting ARR value

	TIM2->CCMR1 = TIM_CCMR1_CC1S_0; // configure as input matching timer input 1

	TIM2->CCER |= (TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC1E); // setting sensitivity to both rising and falling triggers

	TIM2->EGR |= TIM_EGR_UG; // setting update generation

	TIM2->DIER |= (TIM_DIER_CC1DE | TIM_DIER_CC1IE | TIM_DIER_UIE); // setting DMA interrupt requests

	TIM2->SR = 0;

	TIM2->CR1 = (TIM_CR1_CEN | TIM_CR1_ARPE); // enable counter

	NVIC_EnableIRQ(TIM2_IRQn);		// enable interrupts
	NVIC_SetPriority(TIM2_IRQn, 2); // setting interrupt priority
}

void TIM2_IRQHandler(void)
{
	// [TODO]
	float CounterClockPeriod = 0.000001;

	if (TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		overflowCount++;
	}
	if (TIM2->SR & TIM_SR_CC1IF)
	{
		TIM2->SR &= ~TIM_SR_CC1IF;

		if (lastValue == 0)
		{
			lastValue = TIM2->CCR1;
			overflowCount = 0;
		}
		else
		{
			currentValue = TIM2->CCR1;
			//timeInterval = (currentValue - lastValue + (1 + TIM4->ARR) * overflowCount);
			timeInterval = (currentValue - lastValue);
			lastValue = 0;
		}
	}
}


void Trigger_Setup()
{
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; // enabling GPIO clocks

	GPIOE->MODER &= ~GPIO_MODER_MODE11;		   // clearing GPIO mode bits
	GPIOE->MODER |= GPIO_MODER_MODE11_1;	   // setting GPIO mode to alternate functions
	GPIOE->AFR[1] = GPIO_AFRH_AFSEL11_0;	   // select TIM1_CH2 as function
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD11;		   // setting no PUPD
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT11;		   // setting push-pull mode
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11; // setting very high output speed

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable timer 1
	TIM1->PSC |= 0x0000000FU;			// setting prescaler value
	TIM1->ARR |= 0xFFFFU;				// setting ARR value

	TIM1->CCR2 = 10; // setting CCR value

	//	TIM1->CCMR1 &= ~TIM_CCMR1_CC2S;								// CC2 channel is configured as output
	//	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;								// clearing mode bits
	//	/* setting mode bits */
	//	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_0;
	//	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1;
	//	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2;
	//	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M_3;

	TIM1->CCMR1 = (TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2); // setting preload enable bit

	//TIM1->CCER &= ~TIM_CCER_CC2P;									// setting output polarity to active high
	TIM1->CCER = TIM_CCER_CC2E; // enabling channel 2 output

	TIM1->BDTR = (TIM_BDTR_MOE | TIM_BDTR_OSSR); // setting MOE
	//TIM1->BDTR |= TIM_BDTR_OSSR;									// setting OSSR
	TIM1->EGR = TIM_EGR_UG; // setting update generation

	TIM1->DIER = TIM_DIER_UIE; // update interrupt enabled
	TIM1->SR = 0;			   // clearing UIF

	TIM1->CR1 |= (TIM_CR1_CEN | TIM_CR1_ARPE); // enable counter
}