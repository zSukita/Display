#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "Display.pio.h"
#include "inc/ssd1306.h" // Biblioteca para o display SSD1306
#include "inc/font.h"    // Biblioteca para fontes
#include <stdbool.h>


// Definições de pinos e constantes
#define BTN_A 5          // Pino do botão A
#define BTN_B 6          // Pino do botão B
#define BTN_STICK 22     // Pino do botão STICK
#define LED_RED 13       // Pino do LED vermelho
#define LED_GREEN 11     // Pino do LED verde
#define LED_BLUE 12      // Pino do LED azul
#define BUZZER 21        // Pino do buzzer
#define I2C_PORT i2c1    // Porta I2C usada para o display
#define I2C_SDA 14       // Pino SDA do I2C
#define I2C_SCL 15       // Pino SCL do I2C
#define endereco 0x3C    // Endereço I2C do display SSD1306
ssd1306_t ssd;

// Variável para o display
ssd1306_t ssd;
bool cor = true;         // Variável para controle de cor do display

// Definições para a matriz de LEDs WS2812B
#define NUM_PIXELS 25    // Número de LEDs na matriz
#define OUT_PIN 7        // Pino de saída da matriz
uint32_t valor_led;      // Variável para controle do valor do LED
double r = 0.0, b = 0.0, g = 0.0; // Variáveis para valores de cor
PIO pio = pio0;          // PIO usado para controlar a matriz
uint offset;             // Offset para o programa PIO
uint sm;                 // State machine do PIO


// Debounce para botões
#define DEBOUNCE_TIME 200000 // Tempo de debounce em microssegundos
uint32_t last_time = 0;      // Último tempo de acionamento para debounce
bool verde_on = 0;           // Estado do LED verde (ligado/desligado)

// Inicializa a matriz de LEDs usando PIO
void init_pio_matriz()
{
    offset = pio_add_program(pio, &Display_program); // Adiciona o programa PIO
    sm = pio_claim_unused_sm(pio, true);             // Obtém uma state machine disponível
    Display_program_init(pio, sm, offset, OUT_PIN);  // Inicializa o programa PIO
}

// Padrões de números para a matriz de LEDs
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

// Converte valores de cor para o formato WS2812B
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8); // Retorna o valor de cor no formato esperado
}

// Aciona a matriz de LEDs com um padrão específico
void liga_matriz(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
 for (int16_t i = 0; i < NUM_PIXELS; i++)
  {

    valor_led = matrix_rgb(desenho[24 - i], r = 0.0, g = 0.0);
    pio_sm_put_blocking(pio, sm, valor_led);
  }
}
// Inicializa os GPIOs para botões, LEDs e buzzer
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

// Inicializa o display SSD1306
void init_i2c()
{
    // Inicializa o I2C a 400 kHz
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Configura o pino SDA
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Configura o pino SCL
    gpio_pull_up(I2C_SDA);                     // Pull-up no pino SDA
    gpio_pull_up(I2C_SCL);                     // Pull-up no pino SCL

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display

    ssd1306_fill(&ssd, true);   // Limpa o display (todos os pixels ligados)
    ssd1306_send_data(&ssd);    // Envia os dados para o display
}

// Desenha um caractere no display
void desenha_caractere(char c) // atualiza o display com o caractere inserido pelo usuario
{
    ssd1306_fill(&ssd, cor);
    ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
    ssd1306_draw_string(&ssd, "CARACTERE", 28, 10);
    ssd1306_draw_string(&ssd, &c, 63, 30);
    ssd1306_send_data(&ssd);
}
// Trata o acionamento dos botões (alterna LEDs e atualiza o display)
void resposta(bool led, bool verde) 
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

// Função de callback para interrupções dos botões
void btns_callback(uint gpio, uint32_t events)
{
    // Captura o momento atual desde o início do boot e converte para microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se o tempo desde o último acionamento é maior que o tempo de debounce
    if (current_time - last_time > DEBOUNCE_TIME)
    {
        // Verifica qual botão foi pressionado
        if (gpio == BTN_A)
        {
            // Se o botão A for pressionado:
            // - Alterna o estado do LED verde (!gpio_get(LED_GREEN))
            // - Atualiza o display e o terminal (verde_on = true)
            resposta(!gpio_get(LED_GREEN), verde_on = true);
        }
        else if (gpio == BTN_B)
        {
            // Se o botão B for pressionado:
            // - Alterna o estado do LED azul (!gpio_get(LED_BLUE))
            // - Atualiza o display e o terminal (verde_on = false)
            resposta(!gpio_get(LED_BLUE), verde_on = false);
        }
        else if (gpio == BTN_STICK)
        {
            // Se o botão STICK for pressionado:
            // - Entra no modo BOOTSEL para reprogramação do microcontrolador via USB
            reset_usb_boot(0, 0);
        }

        // Atualiza o tempo do último acionamento para o momento atual
        // Isso garante que o próximo acionamento só será tratado após o tempo de debounce
        last_time = current_time;
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
