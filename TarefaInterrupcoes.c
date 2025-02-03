// By: Alex Rodrigo Porto Barbosa

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "TarefaInterrupcoes.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define matrixRGB_PIN 7

#define LEDR_PIN 13
#define LEDB_PIN 12
#define LEDG_PIN 11

#define button_0 5
#define button_1 6

static volatile int numb = 0;  // variavel global que define o número exibido na matriz
static volatile uint32_t last_time = 0;
static volatile bool update_led_matrix = false; // variavel de atualização da matriz

// varialvel que armazena a cor e intensidade
uint8_t led_r = 1; // Intensidade do vermelho
uint8_t led_g = 1; // Intensidade do verde
uint8_t led_b = 1; // Intensidade do azul

// Matriz para armazenar quais LEDs serão ligados
bool matrixLED[10][NUM_PIXELS] = {
    {// 0
    0, 1, 1, 1, 0, 
    1, 0, 0, 0, 1, 
    1, 0, 0, 0, 1, 
    1, 0, 0, 0, 1, 
    0, 1, 1, 1, 0
}, {// 1
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0
}, {// 2
    0, 1, 1, 1, 0, 
    0, 0, 1, 0, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 0, 1, 1, 0
}, {// 3
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
}, {// 4
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0
}, {// 5
    0, 0, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 0, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
}, {// 6
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
}, {// 7
    0, 0, 0, 1, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
}, {// 8
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
}, {// 9
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
}
};

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


void set_one_led(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (matrixLED[numb][i])
        {
            put_pixel(color); // Liga os ledss que estiverem com 1  
        }
        else
        {
            put_pixel(0);  // Desliga os leds que estiverem com 0
        }
    }
}

static void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{   
    stdio_init_all();

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &TarefaInterrupcoes_program);

    TarefaInterrupcoes_program_init(pio, sm, offset, matrixRGB_PIN, 800000, IS_RGBW);
    
    gpio_init(LEDR_PIN);              
    gpio_set_dir(LEDR_PIN, GPIO_OUT);

    //Inicialização dos dois botões
    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN); 
    gpio_pull_up(button_0);   

    gpio_init(button_1);
    gpio_set_dir(button_1, GPIO_IN); 
    gpio_pull_up(button_1);          

    // Configuração da interrupção com callback para ambos os botões 
    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_1, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    set_one_led(led_r, led_g, led_b);

    while (true)
    {
        
        gpio_put(LEDR_PIN, 1);  // Acende o LED
        sleep_ms(150);         // Mantém aceso por 150 ms
        gpio_put(LEDR_PIN, 0);  // Apaga o LED
        sleep_ms(50);          // Mantém apagado por 50 ms

        if (update_led_matrix)
        {
            set_one_led(led_r, led_g, led_b);
            update_led_matrix = false;  // Resetar a flag após a atualização
        }
    
        
        
    }

    return 0;
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se passou tempo suficiente desde o último evento (debouncing)
    if (current_time - last_time > 200000) // 200ms de debouncing
    {
        last_time = current_time; // atualiza o tempo
        
        if (gpio == button_0) {

            set_one_led(0, 0, 0);
            //sleep_ms(500);
            if (numb < 9) {
                numb++;
                
            }
            update_led_matrix = true;// é colocada como true para a atualização da matriz de leds ocorrer
        } else if (gpio == button_1) {

            set_one_led(0, 0, 0);
            //sleep_ms(500);
            if (numb > 0) {
                numb--;
            }
            update_led_matrix = true;
        }
    }
}

