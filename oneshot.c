#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

// Defines
#define BUTTON 5

// Variáveis globais
const uint8_t pinos[3] = {12, 13, 11};
static volatile uint32_t last_time = 0;
bool desligados = true;

// Declaração de funções
void setup();
bool debouncing();
int64_t turn_off_callback(alarm_id_t id, void *user_data);


int main()
{
    // Inicializações
    stdio_init_all();
    setup();

    while (true) {
        // Rotina verifica o bouncing e depois verifica se os LEDs estão desligados e o botão for apertado
        if (debouncing() && (gpio_get(BUTTON) == 0 && desligados)){
            // Liga todos os LEDs
            for(int i = 0; i < 3; i++){
                gpio_put(pinos[i], true);
            }
            // Muda o estado da flag
            desligados = false;

            // Alarme para desligar os LEDs
            add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        }
        sleep_ms(10);
    }
}

void setup(){
    for(int i = 0; i<3; i++){
        gpio_init(pinos[i]);
        gpio_set_dir(pinos[i], GPIO_OUT);
    }

    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
}

bool debouncing(){
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    
    // Debouncing
    if (current_time - last_time > 50000){
        last_time = current_time;
        return true;
    }
    return false;
}

// Função utilizada para desligar os LEDs e Ativar o Callback
int64_t turn_off_callback(alarm_id_t id, void *user_data){
    if(gpio_get(12)){
        gpio_put(12, false);
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    }
    else if(gpio_get(13)){
        gpio_put(13, false);
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    }
    else{
        gpio_put(11, false);
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        desligados = true;
    }
    return 0;
}