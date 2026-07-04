/**
 * @file stm32_cubemx_setup.h
 * @brief STM32CubeMX Configuration Reference
 * 
 * This file documents the required STM32CubeMX peripheral configurations
 * for the Projeto Final firmware.
 * 
 * Place all HAL initialization code in USER CODE sections only.
 * Custom firmware modules should be added to Inc/ and Src/ directories.
 * 
 * @author ASSERT Laboratory
 * @date 2026
 */

/* ===== REQUIRED STM32CUBEMX CONFIGURATIONS ===== */

/*
 * 1. ADC1 Configuration
 *    - Conversion: Single channel (potentiometer input)
 *    - Resolution: 12-bit
 *    - Scan mode: Disabled (single channel)
 *    - Continuous mode: Disabled (triggered by timer)
 *    - External trigger: Timer event (ADC sampling timer)
 */

/*
 * 2. Timer for ADC Sampling (5ms period)
 *    - Frequency: Internal clock
 *    - Prescaler: Configure for 1ms tick (adjust based on system clock)
 *    - Period: 4 (gives 5ms with 1ms tick)
 *    - CK_INT: Enabled
 *    - Update Event: Triggers ADC conversion
 *    - Interrupt: Update interrupt ENABLED
 *    - Timer runs continuously
 *    
 *    Handler: Bsp_SamplingTimerIsr()
 *    - Sets bsp_flags.adc_sampling_flag = true
 *    - Counts for 1-second interval (200 x 5ms = 1000ms)
 *    - Sets bsp_flags.one_second_flag every 200 interrupts
 */

/*
 * 3. Timer for Button Debounce (20ms period)
 *    - Frequency: Internal clock
 *    - Prescaler: Configure for 1ms tick
 *    - Period: 19 (gives 20ms with 1ms tick)
 *    - CK_INT: Enabled
 *    - Interrupt: Update interrupt ENABLED
 *    - Timer: START DISABLED (only started by EXTI handler)
 *    
 *    Handler: Bsp_DebounceTimerIsr()
 *    - Stops the timer
 *    - Sets bsp_flags.button_pressed_flag = true
 *    - Re-enables EXTI interrupt
 */

/*
 * 4. Timer for PWM (3 channels for 3 LEDs)
 *    - Frequency: Internal clock
 *    - Prescaler: Configure for desired PWM frequency (e.g., 1kHz)
 *    - Period: 999 (for 1kHz with 1MHz timer clock)
 *    - Channel 1: PWM Generation CH1 (LED1)
 *    - Channel 2: PWM Generation CH2 (LED2)
 *    - Channel 3: PWM Generation CH3 (LED3)
 *    - Fast mode: Enabled
 *    - All channels: Output enabled
 *    - Timer: Auto-start ENABLED
 */

/*
 * 5. EXTI Configuration for Button
 *    - GPIO Port: Configure as input
 *    - Pull mode: Pull-up (or pull-down depending on button design)
 *    - EXTI line: Rising edge or falling edge (depending on design)
 *    - Interrupt: ENABLED
 *    
 *    Handler: Bsp_ButtonExtiIsr()
 *    - Disable EXTI interrupt temporarily
 *    - Start debounce timer
 */

/*
 * 6. USART3 Configuration
 *    - Baud rate: 115200
 *    - Data bits: 8
 *    - Stop bits: 1
 *    - Parity: None
 *    - Hardware flow control: None
 *    - Mode: TX + RX
 *    - RX Interrupt: ENABLED (or use DMA)
 *    - Global interrupt: ENABLED
 *    
 *    Handler: Bsp_Usart3RxIsr()
 *    - Read byte from RX register
 *    - Buffer for SerialCmd_Process()
 */

/* ===== USER CODE SECTIONS ===== */

/*
 * In STM32CubeMX-generated files, place all HAL initialization in:
 * 
 * /* USER CODE BEGIN 1 */ ... /* USER CODE END 1 */
 * 
 * And interrupt handlers in:
 * 
 * void ADC1_IRQHandler(void)
 * {
 *     /* USER CODE BEGIN ADC1_IRQn 0 */
 *     Bsp_AdcIsr();
 *     /* USER CODE END ADC1_IRQn 0 */
 *     
 *     HAL_ADC_IRQHandler(&hadc1);
 *     
 *     /* USER CODE BEGIN ADC1_IRQn 1 */
 *     /* USER CODE END ADC1_IRQn 1 */
 * }
 */

#endif /* STM32_CUBEMX_SETUP_H */
