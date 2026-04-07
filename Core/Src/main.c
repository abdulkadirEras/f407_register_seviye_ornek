/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint32_t msTicks;
	uint32_t adc_buffer[2];
	uint8_t hareketliPWM;
	uint8_t bir_ms_Mi;

}programDegiskenleri;

programDegiskenleri program;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Run_LED (1<<12)
#define Durum_Mak_Sure_Ms 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void TIM1_UP_TIM10_IRQHandler(void)
{


    if (TIM1->SR & TIM_SR_UIF) // Kesme bayrağını (UIF) kontrol et
    {
        program.msTicks++;            	 // Sayacı artır
        program.bir_ms_Mi=1;
        TIM1->SR &= ~TIM_SR_UIF; // Bayrağı temizle (Önemli!)
    }


}

void ADC_MultiChannel_DMA_Init(uint32_t adc_buffer[2]);
void Timer1_Init_1ms_IT(void);
void USART2_Init(void);
void UART_Yaz(int ch);
char UART_Oku(void);
void UART_Print(char *str);
void GPIO_Init_PD12(void);
void Timer4_PWM_PD13_Init_1KHz(void);
void Set_PWM_Yuzdelik(uint8_t yuzde);
void IWDG_Init_200ms(void);
void IWDG_Sifirla(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  GPIO_Init_PD12();
  Timer1_Init_1ms_IT();
  Timer4_PWM_PD13_Init_1KHz();
  USART2_Init();
  IWDG_Init_200ms();
  ADC_MultiChannel_DMA_Init(program.adc_buffer);


  UART_Print("STM32F407 UART Hazir!\r\n");
  program.msTicks=0;
  program.hareketliPWM=0;
  program.bir_ms_Mi=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(program.bir_ms_Mi==1)
	  {
		  //UART ı dinle
		  // Dinleme başladığını belirt
		  USART2->CR1 |= USART_CR1_RE; // Alıcıyı aktif et
		  if (USART2->SR & USART_SR_RXNE)
		  {
			  char veri = (char)(USART2->DR & 0xFF);
			  UART_Yaz(veri);

		  }

		  //Süre Doldu: Dinlemeyi Bırak
		  USART2->CR1 &= ~USART_CR1_RE; // RE (Receiver Enable) bitini temizle
		  program.bir_ms_Mi=0;
	  }

	  if (program.msTicks >= Durum_Mak_Sure_Ms)
	  {

	    GPIOD->ODR ^= Run_LED;



	    Set_PWM_Yuzdelik(program.hareketliPWM);
	    program.hareketliPWM++;
	    if(program.hareketliPWM>100) program.hareketliPWM=0;

	    IWDG_Sifirla();
	    program.msTicks = 0;

	   }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Timer1_Init_1ms_IT(void)
{
    /********* 1. TIM1 Saat Sinyalini Aktif Et *********/
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // TIM1, APB2 hattındadır

    /********* 2. Prescaler ve Auto-Reload Değerleri *********/
    /* Formül: Update Event (Hz) = fCK_PSC / ( (PSC + 1) * (ARR + 1) )
       Hedef: 1000 Hz (1ms)
       fCK_PSC: 16,000,000 Hz (16 MHz)

       Seçim: PSC = 15 => (16 / 16 = 1 MHz saat hızı timer için)
              ARR = 999 => (1 MHz / 1000 = 1 kHz = 1 ms)
    */
    TIM1->PSC = 15;
    TIM1->ARR = 999;

    /********* 3. Kesmeyi (Interrupt) Etkinleştir *********/
    TIM1->DIER |= TIM_DIER_UIE; // Update Interrupt Enable

    /********* 4. NVIC Ayarları *********/
    // TIM1 Update kesmesi için NVIC'i yetkilendir
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0); // En yüksek öncelik

    /********* 5. Sayacı Başlat *********/
    TIM1->CR1 |= TIM_CR1_CEN;   // Counter Enable
}

void USART2_Init(void)
{
    // 1. Clock (Saat) sinyallerini aktif et
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA clock aktif
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2 clock aktif

    // 2. GPIO Pinlerini Alternatif Fonksiyon (AF7) olarak ayarla
    // PA2: TX, PA3: RX
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1); // Alternate Function modu

    // AF7 (USART2) seçimi (AFR[0] low register, AFR[1] high register'dır)
    GPIOA->AFR[0] |= (7 << (2 * 4)); // PA2 -> AF7
    GPIOA->AFR[0] |= (7 << (3 * 4)); // PA3 -> AF7

    // 3. USART2 Konfigürasyonu
    // Baud Rate: 115200 @ 8MHz APB1
    USART2->BRR = 0x45;

    // Kontrol Register Ayarları
    USART2->CR1 |= USART_CR1_TE; // Transmit (Yazma) Aktif
    USART2->CR1 |= USART_CR1_RE; // Receive (Okuma) Aktif
    USART2->CR1 |= USART_CR1_UE; // USART Modülünü Aktif Et
}

void UART_Yaz(int ch)
{
    // TXE (Transmit data register empty) bayrağının set olmasını bekle
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (ch & 0xFF);
}

char UART_Oku(void)
{
    // RXNE (Read data register not empty) bayrağının set olmasını bekle
    while (!(USART2->SR & USART_SR_RXNE));
    return (char)(USART2->DR & 0xFF);
}


void UART_Print(char *str)
{
    while (*str) {
        UART_Yaz(*str++);
    }
}

void GPIO_Init_PD12(void)
{
    /********* 1. GPIOD Saat Sinyalini Aktif Et *********/
    RCC->AHB1ENR |= (1 << 3); // GPIOD EN (Bit 3)

    /********* 2. PD12 Pinini Çıkış Olarak Ayarla *********/
    // MODER register'ında her pin için 2 bit vardır.
    // Pin 12 için 24. ve 25. bitler: 01 = General purpose output mode
    GPIOD->MODER &= ~(3 << (12 * 2)); // Önce temizle
    GPIOD->MODER |=  (1 << (12 * 2)); // 01 yap (Output)

    // Opsiyonel: Hız ve Tip ayarları (Varsayılanlar genelde yeterlidir)
    GPIOD->OTYPER &= ~(1 << 12);      // 0 = Push-pull
    GPIOD->OSPEEDR |= (2 << (12 * 2)); // 10 = High speed
}



void ADC_MultiChannel_DMA_Init(uint32_t adc_buffer[2])
{
    // 1. Saat Sinyallerini Aktif Et
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA için
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;   // DMA2 için (ADC2 DMA2 hattındadır)
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // ADC1 için

    // 2. GPIO Ayarları (Analog Mod)
    GPIOA->MODER |= (3 << (0 * 2)); // PA0 Analog
    GPIOA->MODER |= (3 << (1 * 2)); // PA1 Analog

    // 3. DMA2 Stream 0 Channel 0 Yapılandırması (ADC1 için)
    DMA2_Stream0->CR &= ~DMA_SxCR_EN; // Önce DMA'yı durdur
    while(DMA2_Stream0->CR & DMA_SxCR_EN);

    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;      // Kaynak: ADC Data Register
    DMA2_Stream0->M0AR = (uint32_t)adc_buffer;    // Hedef: Bizim dizimiz
    DMA2_Stream0->NDTR = 2;                       // 2 kanal okunacak

    DMA2_Stream0->CR |= (0 << DMA_SxCR_CHSEL_Pos); // Channel 0 seçimi
    DMA2_Stream0->CR |= DMA_SxCR_MINC;             // Bellek adresini artır (Memory Increment)
    DMA2_Stream0->CR |= DMA_SxCR_MSIZE_1;          // Bellek boyutu: 32 bit
    DMA2_Stream0->CR |= DMA_SxCR_PSIZE_1;          // Çevre birimi boyutu: 32 bit
    DMA2_Stream0->CR |= DMA_SxCR_CIRC;             // Döngüsel mod (Sürekli güncellesin)
    DMA2_Stream0->CR |= DMA_SxCR_EN;               // DMA Aktif

    // 4. ADC Ortak Ayarları (ADC_CCR)
    ADC->CCR |= (1 << ADC_CCR_ADCPRE_Pos); // Prescaler: 8MHz / 4 = 2MHz (Sınır 18MHz)

    // 5. ADC1 Ayarları
    ADC1->CR1 |= ADC_CR1_SCAN;     // Tarama (Scan) modunu aç
    ADC1->CR2 |= ADC_CR2_CONT;     // Sürekli (Continuous) çevrim
    ADC1->CR2 |= ADC_CR2_DMA;      // DMA isteğini aç
    ADC1->CR2 |= ADC_CR2_DDS;      // DMA istekleri her çevrimde devam etsin

    // 6. Kanal Sıralaması (SQR)
    ADC1->SQR1 |= (1 << ADC_SQR1_L_Pos); // Toplam 2 kanal okunacak (L=1 -> 2 kanal demek)
    ADC1->SQR3 |= (0 << ADC_SQR3_SQ1_Pos); // 1. sırada Kanal 0
    ADC1->SQR3 |= (1 << ADC_SQR3_SQ2_Pos); // 2. sırada Kanal 1

    // 7. ADC'yi Başlat
    ADC1->CR2 |= ADC_CR2_ADON; // ADC Power On

    // Küçük bir gecikme sonrası çevrimi başlat
    for(int i=0; i<1000; i++);
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

void Timer4_PWM_PD13_Init_1KHz(void)
{
	// 1. Saatleri Aktif Et
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	// 2. PD13 Pinini AF2 (TIM4_CH2) Olarak Ayarla
	GPIOD->MODER &= ~(GPIO_MODER_MODER13);
	GPIOD->MODER |= GPIO_MODER_MODER13_1; // Alternate Function
	// AFR[1] (High Register) PD13 için 20-23. bitleri kontrol eder
	GPIOD->AFR[1] &= ~(0xF << (5 * 4));   // PD13 pin index 5 (13-8=5)
	GPIOD->AFR[1] |= (2 << (5 * 4));      // AF2

	// 3. Timer Zamanlaması (8 MHz APB1 için 1 kHz)
	TIM4->PSC = 8 - 1;   // 1 MHz tick
	TIM4->ARR = 1000 - 1; // 1 kHz periyot

	// 4. PWM Modu Ayarı (Channel 2 için CCMR1'in üst bitleri)
	TIM4->CCMR1 &= ~TIM_CCMR1_OC2M;       // Temizle
	TIM4->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos); // PWM Mode 1
	TIM4->CCMR1 |= TIM_CCMR1_OC2PE;       // Preload Enable

	// 5. Kanal 2 Çıkışını ve Timer'ı Aktif Et
	TIM4->CCER |= TIM_CCER_CC2E;          // Channel 2 Output Enable
	TIM4->CR1 |= TIM_CR1_CEN;             // Counter Enable
}

void Set_PWM_Yuzdelik(uint8_t yuzde)
{
    if (yuzde > 100) yuzde = 100;

    TIM4->CCR2 = (uint32_t)(yuzde * 10);
}

void IWDG_Init_200ms(void)
{
    // 1. IWDG anahtar kaydedicisine (KR) erişimi aç (0x5555)
    // Bu işlem PR ve RLR registerlarını yazılabilir yapar.
    IWDG->KR = 0x5555;

    // 2. Prescaler ayarı: 32 kHz / 16 = 2000 Hz (Register değeri 2 -> /16)
    IWDG->PR = 0x02;

    // 3. Reload ayarı: 400 tick * 0.5 ms = 200 ms
    IWDG->RLR = 400;

    // 4. Ayarları yükle ve başlat (0xAAAA reload, 0xCCCC başlat)
    IWDG->KR = 0xAAAA; // Sayacı RLR değerinden başlatır
    IWDG->KR = 0xCCCC; // Watchdog'u aktif eder
}
void IWDG_Sifirla(void)
{
    IWDG->KR = 0xAAAA;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  GPIOD->ODR ^= Run_LED;
	  HAL_Delay(500);
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
