/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */


#include "stm32f7xx.h"
#include "stm32f769i_discovery.h"
#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"
#include "lvgl/lvgl.h"
#include "lv_examples/lv_tests/lv_test_theme/lv_test_theme_2.h"
#include "lv_examples/lv_tests/lv_test_objx/lv_test_kb/lv_test_kb.h"

static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);


LV_IMG_DECLARE(letter);

int main(void)
{

	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	/* STM32F7xx HAL library initialization:
	       - Configure the Flash prefetch
	       - Systick timer is configured by default as source of time base, but user
	         can eventually implement his proper time base source (a general purpose
	         timer for example or other time source), keeping in mind that Time base
	         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	         handled in milliseconds basis.
	       - Set NVIC Group Priority to 4
	       - Low Level Initialization
	 */
	HAL_Init();

	SystemClock_Config();

	lv_init();

	tft_init();

	touchpad_init();

//	lv_test_label_1();
//	lv_test_page_1();
//	demo_create();
//	benchmark_create();
//	lv_test_theme_2();
//	lv_test_kb_2();

/*
	Create a screen
	lv_obj_t * scr = lv_obj_create(NULL, NULL);
	lv_scr_load(scr);                                   Load the screen

	lv_obj_t * img1 = lv_img_create(scr, NULL);
    lv_img_set_src(img1, &letter);
    lv_obj_set_size(img1,512,512);
    lv_obj_align(img1,NULL, LV_ALIGN_CENTER, 0, 0);
*/


	static lv_style_t style_sb;
	lv_style_copy(&style_sb, &lv_style_plain);
	style_sb.body.main_color = LV_COLOR_BLACK;
	style_sb.body.grad_color = LV_COLOR_BLACK;
	style_sb.body.border.color = LV_COLOR_WHITE;
	style_sb.body.border.width = 1;
	style_sb.body.border.opa = LV_OPA_70;
	style_sb.body.radius = LV_RADIUS_CIRCLE;
	style_sb.body.opa = LV_OPA_60;



	/*create style for text area */
	static lv_style_t st;
	lv_style_copy( &st, &lv_style_plain );
	st.text.font = &lv_font_roboto_28;

	/*Create a normal Text area*/
	lv_obj_t * ta1 = lv_ta_create(lv_scr_act(), NULL);
	lv_obj_set_pos(ta1, 300, 40);
	lv_obj_set_size(ta1, 400, 200);
	lv_obj_align(ta1, NULL, LV_ALIGN_CENTER, 0, - LV_DPI / 2);
	lv_ta_set_style(ta1,LV_TA_STYLE_SB, &style_sb);                     /*Apply the scroll bar style*/
	lv_ta_set_cursor_type(ta1, LV_CURSOR_BLOCK);
	lv_ta_set_text(ta1, "A text in a Text Area\n""");
	lv_ta_set_cursor_pos(ta1, 2);                                       /*Set the cursor position*/
	lv_ta_add_text(ta1, "long");                                       /*Insert a word at the current cursor position*/
	lv_ta_set_cursor_type(ta1, LV_CURSOR_BLOCK);
	lv_obj_set_style( ta1, &st );
	 /*Create styles for the keyboard*/
	 static lv_style_t rel_style, pr_style;

	 lv_style_copy(&rel_style, &lv_style_btn_rel);
	 rel_style.body.radius = 0;

	 lv_style_copy(&pr_style, &lv_style_btn_pr);
	 pr_style.body.radius = 0;

	 /*Create a keyboard and apply the styles*/
	 lv_obj_t *kb = lv_kb_create(lv_scr_act(), NULL);
	 lv_kb_set_cursor_manage(kb, true);
	 lv_kb_set_style(kb, LV_KB_STYLE_BG, &lv_style_transp_tight);
	 lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &rel_style);
	 lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &pr_style);

	 /*Assign the text area to the keyboard*/
	 lv_kb_set_ta(kb,ta1);
	while(1) {
		lv_task_handler();
		HAL_Delay(5);

	}
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 7;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}


/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}
