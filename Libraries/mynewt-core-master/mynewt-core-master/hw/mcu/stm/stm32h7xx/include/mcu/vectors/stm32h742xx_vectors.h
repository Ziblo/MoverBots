/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

INT_VECTOR_STACK_TOP(__StackTop)
INT_VECTOR_RESET_HANDLER(Reset_Handler)
INT_VECTOR_NMI_HANDLER(NMI_Handler)
INT_VECTOR_HARDFAULT_HANDLER(HardFault_Handler)
INT_VECTOR_MEMMANAGE_HANDLER(MemManage_Handler)
INT_VECTOR_BUSFAULT_HANDLER(BusFault_Handler)
INT_VECTOR_USAGEFAULT_HANDLER(UsageFault_Handler)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR_SVC_HANDLER(SVC_Handler)
INT_VECTOR_DEBUGMON_HANDLER(DebugMon_Handler)
INT_VECTOR_UNUSED(0)
INT_VECTOR_PENDSV_HANDLER(PendSV_Handler)
INT_VECTOR_SYSTICK_HANDLER(SysTick_Handler)
INT_VECTOR(WWDG_IRQHandler)
INT_VECTOR(PVD_AVD_IRQHandler)
INT_VECTOR(TAMP_STAMP_IRQHandler)
INT_VECTOR(RTC_WKUP_IRQHandler)
INT_VECTOR(FLASH_IRQHandler)
INT_VECTOR(RCC_IRQHandler)
INT_VECTOR(EXTI0_IRQHandler)
INT_VECTOR(EXTI1_IRQHandler)
INT_VECTOR(EXTI2_IRQHandler)
INT_VECTOR(EXTI3_IRQHandler)
INT_VECTOR(EXTI4_IRQHandler)
INT_VECTOR(DMA1_Stream0_IRQHandler)
INT_VECTOR(DMA1_Stream1_IRQHandler)
INT_VECTOR(DMA1_Stream2_IRQHandler)
INT_VECTOR(DMA1_Stream3_IRQHandler)
INT_VECTOR(DMA1_Stream4_IRQHandler)
INT_VECTOR(DMA1_Stream5_IRQHandler)
INT_VECTOR(DMA1_Stream6_IRQHandler)
INT_VECTOR(ADC_IRQHandler)
INT_VECTOR(FDCAN1_IT0_IRQHandler)
INT_VECTOR(FDCAN2_IT0_IRQHandler)
INT_VECTOR(FDCAN1_IT1_IRQHandler)
INT_VECTOR(FDCAN2_IT1_IRQHandler)
INT_VECTOR(EXTI9_5_IRQHandler)
INT_VECTOR(TIM1_BRK_IRQHandler)
INT_VECTOR(TIM1_UP_IRQHandler)
INT_VECTOR(TIM1_TRG_COM_IRQHandler)
INT_VECTOR(TIM1_CC_IRQHandler)
INT_VECTOR(TIM2_IRQHandler)
INT_VECTOR(TIM3_IRQHandler)
INT_VECTOR(TIM4_IRQHandler)
INT_VECTOR(I2C1_EV_IRQHandler)
INT_VECTOR(I2C1_ER_IRQHandler)
INT_VECTOR(I2C2_EV_IRQHandler)
INT_VECTOR(I2C2_ER_IRQHandler)
INT_VECTOR(SPI1_IRQHandler)
INT_VECTOR(SPI2_IRQHandler)
INT_VECTOR(USART1_IRQHandler)
INT_VECTOR(USART2_IRQHandler)
INT_VECTOR(USART3_IRQHandler)
INT_VECTOR(EXTI15_10_IRQHandler)
INT_VECTOR(RTC_Alarm_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR(TIM8_BRK_TIM12_IRQHandler)
INT_VECTOR(TIM8_UP_TIM13_IRQHandler)
INT_VECTOR(TIM8_TRG_COM_TIM14_IRQHandler)
INT_VECTOR(TIM8_CC_IRQHandler)
INT_VECTOR(DMA1_Stream7_IRQHandler)
INT_VECTOR(FMC_IRQHandler)
INT_VECTOR(SDMMC1_IRQHandler)
INT_VECTOR(TIM5_IRQHandler)
INT_VECTOR(SPI3_IRQHandler)
INT_VECTOR(UART4_IRQHandler)
INT_VECTOR(UART5_IRQHandler)
INT_VECTOR(TIM6_DAC_IRQHandler)
INT_VECTOR(TIM7_IRQHandler)
INT_VECTOR(DMA2_Stream0_IRQHandler)
INT_VECTOR(DMA2_Stream1_IRQHandler)
INT_VECTOR(DMA2_Stream2_IRQHandler)
INT_VECTOR(DMA2_Stream3_IRQHandler)
INT_VECTOR(DMA2_Stream4_IRQHandler)
INT_VECTOR(ETH_IRQHandler)
INT_VECTOR(ETH_WKUP_IRQHandler)
INT_VECTOR(FDCAN_CAL_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR(DMA2_Stream5_IRQHandler)
INT_VECTOR(DMA2_Stream6_IRQHandler)
INT_VECTOR(DMA2_Stream7_IRQHandler)
INT_VECTOR(USART6_IRQHandler)
INT_VECTOR(I2C3_EV_IRQHandler)
INT_VECTOR(I2C3_ER_IRQHandler)
INT_VECTOR(OTG_HS_EP1_OUT_IRQHandler)
INT_VECTOR(OTG_HS_EP1_IN_IRQHandler)
INT_VECTOR(OTG_HS_WKUP_IRQHandler)
INT_VECTOR(OTG_HS_IRQHandler)
INT_VECTOR(DCMI_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR(RNG_IRQHandler)
INT_VECTOR(FPU_IRQHandler)
INT_VECTOR(UART7_IRQHandler)
INT_VECTOR(UART8_IRQHandler)
INT_VECTOR(SPI4_IRQHandler)
INT_VECTOR(SPI5_IRQHandler)
INT_VECTOR(SPI6_IRQHandler)
INT_VECTOR(SAI1_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR(DMA2D_IRQHandler)
INT_VECTOR(SAI2_IRQHandler)
INT_VECTOR(QUADSPI_IRQHandler)
INT_VECTOR(LPTIM1_IRQHandler)
INT_VECTOR(CEC_IRQHandler)
INT_VECTOR(I2C4_EV_IRQHandler)
INT_VECTOR(I2C4_ER_IRQHandler)
INT_VECTOR(SPDIF_RX_IRQHandler)
INT_VECTOR(OTG_FS_EP1_OUT_IRQHandler)
INT_VECTOR(OTG_FS_EP1_IN_IRQHandler)
INT_VECTOR(OTG_FS_WKUP_IRQHandler)
INT_VECTOR(OTG_FS_IRQHandler)
INT_VECTOR(DMAMUX1_OVR_IRQHandler)
INT_VECTOR(HRTIM1_Master_IRQHandler)
INT_VECTOR(HRTIM1_TIMA_IRQHandler)
INT_VECTOR(HRTIM1_TIMB_IRQHandler)
INT_VECTOR(HRTIM1_TIMC_IRQHandler)
INT_VECTOR(HRTIM1_TIMD_IRQHandler)
INT_VECTOR(HRTIM1_TIME_IRQHandler)
INT_VECTOR(HRTIM1_FLT_IRQHandler)
INT_VECTOR(DFSDM1_FLT0_IRQHandler)
INT_VECTOR(DFSDM1_FLT1_IRQHandler)
INT_VECTOR(DFSDM1_FLT2_IRQHandler)
INT_VECTOR(DFSDM1_FLT3_IRQHandler)
INT_VECTOR(SAI3_IRQHandler)
INT_VECTOR(SWPMI1_IRQHandler)
INT_VECTOR(TIM15_IRQHandler)
INT_VECTOR(TIM16_IRQHandler)
INT_VECTOR(TIM17_IRQHandler)
INT_VECTOR(MDIOS_WKUP_IRQHandler)
INT_VECTOR(MDIOS_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR(MDMA_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR(SDMMC2_IRQHandler)
INT_VECTOR(HSEM1_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR(ADC3_IRQHandler)
INT_VECTOR(DMAMUX2_OVR_IRQHandler)
INT_VECTOR(BDMA_Channel0_IRQHandler)
INT_VECTOR(BDMA_Channel1_IRQHandler)
INT_VECTOR(BDMA_Channel2_IRQHandler)
INT_VECTOR(BDMA_Channel3_IRQHandler)
INT_VECTOR(BDMA_Channel4_IRQHandler)
INT_VECTOR(BDMA_Channel5_IRQHandler)
INT_VECTOR(BDMA_Channel6_IRQHandler)
INT_VECTOR(BDMA_Channel7_IRQHandler)
INT_VECTOR(COMP1_IRQHandler)
INT_VECTOR(LPTIM2_IRQHandler)
INT_VECTOR(LPTIM3_IRQHandler)
INT_VECTOR(LPTIM4_IRQHandler)
INT_VECTOR(LPTIM5_IRQHandler)
INT_VECTOR(LPUART1_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR(CRS_IRQHandler)
INT_VECTOR(ECC_IRQHandler)
INT_VECTOR(SAI4_IRQHandler)
INT_VECTOR_UNUSED(0)
INT_VECTOR_UNUSED(0)
INT_VECTOR(WAKEUP_PIN_IRQHandler)
