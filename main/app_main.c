#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
static const char *TAG = "ADC EXAMPLE";
static esp_adc_cal_characteristics_t adc1_chars;

const float TDS_FACTOR =1.0;
const float K_VALUE=1.0;
const float TEMPERATURE=25.00;

void app_main(void)
{
    float voltage;

    // tds cal values

    float ecValue=0.0;
    float ecValue25 =0.0;
    float tdsValue=0.0;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11));
    while (1) 
    {
        //voltage = (esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_0), &adc1_chars));

        // convert adc val to voltage
        voltage = (adc1_get_raw(ADC1_CHANNEL_0)* 2.5)/ 8192;
        ESP_LOGI(TAG, "ADC1_CHANNEL_0: %2f V", voltage);
        
        // const to TDS
	    ecValue=(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage)*K_VALUE;
	    ecValue25  =  ecValue / (1.0+0.02*(TEMPERATURE-25.0));  //temperature compensation
	    tdsValue = ecValue25 * TDS_FACTOR;

        ESP_LOGI(TAG,"The TDS val of ample is :- %2f ppm",tdsValue);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}