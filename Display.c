#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "Display.pio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include <stdbool.h>


#define INIT_GPIO_H
#define BTN_A 5
#define BTN_B 6
#define BTN_STICK 22
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BUZZER 21
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
ssd1306_t ssd;
bool cor = true;


#define NUM_PIXELS 25
// pino de saída da matriz(ws2812) da placa
#define OUT_PIN 7
// variaveis para inciar a matriz
uint32_t valor_led;
double r = 0.0, b = 0.0, g = 0.0;
PIO pio = pio0;
uint offset;
uint sm;


#define DEBOUNCE_TIME 200000 // 200ms em us de debounce
uint32_t last_time = 0;      // captura o tempo do ultimo acionamento do botão para o debounce
bool verde_on = 0;           // verifica qual led está ligado, 1 -> VERDE, 0 -> AZUL

void init_pio_matriz()
{
  // Configuração da PIO, necessario para matriz
  offset = pio_add_program(pio, &Display_program);
  sm = pio_claim_unused_sm(pio, true);
  Display_program_init(pio, sm, offset, OUT_PIN);
}


double apagar_leds[NUM_PIXELS] =
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0};
double num_0[NUM_PIXELS] =
    {0.0, 0.35, 0.35, 0.35, 0.0,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.0, 0.35, 0.35, 0.35, 0.0};
double num_1[NUM_PIXELS] =
    {0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.2, 0.0,
     0.2, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};
double num_2[NUM_PIXELS] =
    {0.2, 0.2, 0.2, 0.2, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.0,
     0.2, 0.2, 0.2, 0.2, 0.2,
     0.0, 0.0, 0.0, 0.0, 0.2,
     0.2, 0.2, 0.2, 0.2, 0.2};
double num_3[NUM_PIXELS] =
    {0.0, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.35, 0.35, 0.35, 0.35};
double num_4[NUM_PIXELS] =
    {0.0, 0.35, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.35, 0.0,
     0.0, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35};
double num_5[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.35,
     0.0, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.35, 0.35, 0.35, 0.35, 0.35};
double num_6[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.0,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35};
double num_7[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35};
double num_8[NUM_PIXELS] =
    {0.0, 0.35, 0.35, 0.35, 0.0,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.0, 0.35, 0.35, 0.35, 0.0};
double num_9[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.35, 0.35, 0.35, 0.35};

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// rotinas para acionar a matrix de leds - ws2812b
void liga_matriz(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
 for (int16_t i = 0; i < NUM_PIXELS; i++)
  {

    valor_led = matrix_rgb(desenho[24 - i], r = 0.0, g = 0.0);
    pio_sm_put_blocking(pio, sm, valor_led);
  }
}

void init_gpio()
{
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);

    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    gpio_init(BTN_STICK);
    gpio_set_dir(BTN_STICK, GPIO_IN);
    gpio_pull_up(BTN_STICK);

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
}


void init_i2c()
{
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels LIGADOS.
    ssd1306_fill(&ssd, true);
    ssd1306_send_data(&ssd);
}

void desenha_caractere(char c) // atualiza o display com o caractere inserido pelo usuario
{
    ssd1306_fill(&ssd, cor);
    ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
    ssd1306_draw_string(&ssd, "CARACTERE", 28, 10);
    ssd1306_draw_string(&ssd, &c, 63, 30);
    ssd1306_send_data(&ssd);
}

void resposta(bool led, bool verde) //trata o acionamento dos botoes, verificando estado e cor dos leds
{
    if (verde)
    {
        gpio_put(LED_BLUE, false);
        gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
        if (led)
        {
            ssd1306_fill(&ssd, cor);
            ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
            ssd1306_draw_string(&ssd, "BOTAO A", 35, 10);
            ssd1306_draw_string(&ssd, "LED VERDE ON", 16, 48);
            ssd1306_send_data(&ssd);
            printf("Led VERDE ligado\n");
        }
        else
        {
            ssd1306_fill(&ssd, cor);
            ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
            ssd1306_draw_string(&ssd, "BOTAO A", 35, 10);
            ssd1306_draw_string(&ssd, "LED VERDE OFF", 16, 48);
            ssd1306_send_data(&ssd);
            printf("Led VERDE desligado\n");
        }
    }
    else
    {
        gpio_put(LED_GREEN, false);
        gpio_put(LED_BLUE, !gpio_get(LED_BLUE));
        if (led)
        {
            ssd1306_fill(&ssd, cor);
            ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
            ssd1306_draw_string(&ssd, "BOTAO B", 35, 10);
            ssd1306_draw_string(&ssd, "LED AZUL ON", 20, 48);
            ssd1306_send_data(&ssd);
            printf("Led AZUL ligado\n");
        }
        else
        {
            ssd1306_fill(&ssd, cor);
            ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
            ssd1306_draw_string(&ssd, "BOTAO B", 35, 10);
            ssd1306_draw_string(&ssd, "LED AZUL OFF", 20, 48);
            ssd1306_send_data(&ssd);
            printf("Led AZUL desligado\n");
        }
    }
}

// Função de interrupção para todos os botões
void btns_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // captura o momento do acionamento e converte para microsegundos
    if (current_time - last_time > DEBOUNCE_TIME)    // Verifica se ja passou o tempo do debounce para acionamento novamente do botao
    {
        if (gpio == BTN_A)
        {
            resposta(!gpio_get(LED_GREEN), verde_on = true); //func para acionar o led VERDE, imprimir no terminal e atualizar o display
        }
        else if (gpio == BTN_B)
        {
            resposta(!gpio_get(LED_BLUE), verde_on = false); //func para acionar o led AZUL, imprimir no terminal e atualizar o display
        }
        else if (gpio == BTN_STICK)
        {
            reset_usb_boot(0, 0); //func para entrar no modo bootsel 
        }
        last_time = current_time; // Atualiza o tempo para o debounce
    }
}




int main()
{   
    stdio_init_all();
    init_gpio(); //inicializa todos os GPIOs
    init_pio_matriz(); // inicializa a matriz de leds
    init_i2c(); //inicializa o display
    //selecione a func e pressione f12 para ir até onde a mesma é criada
    // Configuração das interrupções para todos os botões
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_STICK, GPIO_IRQ_EDGE_FALL, true, btns_callback);

    while (true)
    {
        if (stdio_usb_connected()) // Certifica-se de que o USB está conectado
        { 
            char c;
            if (scanf("%c", &c) == 1) // Lê caractere da entrada padrão
            { 
                printf("Numero recebido: %c\n", c);
                switch (c)
                {
                case '0':
                    liga_matriz(num_0, valor_led, pio, sm, r, g, b); // liga a matriz de leds, é a mesma func apenas alterando o frame(desenho) a ser apresentado
                    desenha_caractere(c);
                    break;
                case '1':
                    liga_matriz(num_1, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '2':
                    liga_matriz(num_2, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '3':
                    liga_matriz(num_3, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '4':
                    liga_matriz(num_4, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '5':
                    liga_matriz(num_5, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '6':
                    liga_matriz(num_6, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '7':
                    liga_matriz(num_7, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '8':
                    liga_matriz(num_8, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '9':
                    liga_matriz(num_9, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                default:
                    desenha_caractere(c); //func para atualizar o display com o caractere digitado
                    liga_matriz(apagar_leds, valor_led, pio, sm, r, g, b);
                }
            }
        }
        sleep_ms(10); // Pequeno delay para estabilidade
    }
}
