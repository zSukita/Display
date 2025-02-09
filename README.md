# README - Sistema de Controle com Raspberry Pi Pico 🚀

## 📖 Visão Geral
Este projeto demonstra um sistema integrado utilizando o **Raspberry Pi Pico** para controle de uma matriz de LEDs WS2812B, exibição em display OLED SSD1306, leitura de botões físicos e acionamento de um buzzer. A aplicação permite interatividade via USB, possibilitando a exibição de caracteres e padrões numéricos na matriz de LEDs.

## 🔌 Hardware Utilizado
- **Raspberry Pi Pico** (Microcontrolador)
- **Display OLED SSD1306** (Conexão I2C)
- **Matriz de LEDs WS2812B** (25 LEDs controlados via PIO)
- **Botões Físicos:** BTN_A, BTN_B, BTN_STICK
- **LEDs Indicadores:** LED Vermelho, LED Verde, LED Azul
- **Buzzer Passivo**

## 📌 Pinagem
| Componente           | Pino GPIO |
|----------------------|-----------|
| **Botão A**          | GPIO 5    |
| **Botão B**          | GPIO 6    |
| **Botão STICK**      | GPIO 22   |
| **LED Vermelho**     | GPIO 13   |
| **LED Verde**        | GPIO 11   |
| **LED Azul**         | GPIO 12   |
| **Buzzer**           | GPIO 21   |
| **I2C SDA (Display)**| GPIO 14   |
| **I2C SCL (Display)**| GPIO 15   |
| **Matriz WS2812B**   | GPIO 7    |

## ⚙️ Funcionalidades
- **Interatividade com Botões:**
  - **BTN_A:** Alterna o estado do LED Verde e atualiza o display com mensagens.
  - **BTN_B:** Alterna o estado do LED Azul e atualiza o display com mensagens.
  - **BTN_STICK:** Ativa o modo BOOTSEL para reprogramação do dispositivo via USB.
- **Exibição no Display OLED:**
  - Exibe caracteres e mensagens interativas.
  - Desenha retângulos e textos para identificar a função acionada.
- **Controle da Matriz de LEDs WS2812B:**
  - Apresenta padrões numéricos predefinidos (0 a 9) na matriz de LEDs.
  - Converte valores de cor para o formato requerido pelos LEDs.
- **Debounce de Botões:**
  - Implementação de debounce para evitar múltiplas leituras devido a ruídos mecânicos.
- **Entrada USB:**
  - Permite a leitura de caracteres que, ao serem enviados, atualizam o display e acionam diferentes padrões na matriz.

## 🔧 Estrutura do Código
### 1. Inicialização do Sistema
- **GPIO:** Configuração dos pinos para botões, LEDs e buzzer.
- **I2C:** Inicialização e configuração do barramento I2C para comunicação com o display OLED.
- **PIO:** Configuração da state machine do PIO para controle da matriz de LEDs WS2812B.
- **Display:** Inicialização e configuração do display OLED, com envio inicial de dados para limpeza e configuração da tela.

### 2. Tratamento de Eventos e Interrupções
- **Interrupções dos Botões:** 
  - Configuradas para detectar borda de descida (falling edge).
  - Utilizam um tempo de debounce para garantir a estabilidade na leitura.
- **Callback dos Botões:** 
  - Função que identifica qual botão foi pressionado.
  - Alterna os estados dos LEDs e atualiza o display com mensagens específicas para cada botão.

### 3. Atualização e Exibição de Dados
- **Exibição no Display OLED:**
  - Função para desenhar caracteres e mensagens dinâmicas.
  - Atualiza a tela sempre que um novo caractere é recebido ou um botão é pressionado.
- **Controle da Matriz de LEDs:**
  - Função que converte os valores de intensidade para o formato RGB usado pelos LEDs WS2812B.
  - Apresenta diferentes padrões (números de 0 a 9) na matriz, conforme a entrada do usuário.

## 🚀 Como Utilizar
1. **Compilação:**
   - Utilize o SDK do Raspberry Pi Pico com o CMake para compilar o projeto.
   - Verifique se todas as dependências (bibliotecas para o display SSD1306, fontes e suporte ao PIO) estão corretamente instaladas.
2. **Carregamento do Binário:**
   - Conecte o Raspberry Pi Pico ao computador e coloque-o em modo BOOTSEL.
   - Transfira o arquivo `.uf2` gerado para o dispositivo.
3. **Operação:**
   - **Botões Físicos:** 
     - Use o **BTN_A** para alternar o LED Verde e exibir mensagens de “LED VERDE ON/OFF”.
     - Use o **BTN_B** para alternar o LED Azul e exibir mensagens de “LED AZUL ON/OFF”.
     - Use o **BTN_STICK** para reiniciar o dispositivo e entrar no modo de reprogramação.
   - **Entrada USB:**
     - Envie caracteres via USB que serão lidos pelo sistema.
     - O caractere enviado será exibido no display OLED e acionará um padrão específico na matriz de LEDs (de 0 a 9). Outros caracteres limparão a matriz.

## 🛠️ Instalação e Configuração do Ambiente
- **Pré-requisitos:**
  - Ferramentas de desenvolvimento para C/C++.
  - SDK do Raspberry Pi Pico.
  - CMake.
- **Passos de Configuração:**
  1. Clone o repositório do projeto.
  2. Configure o ambiente de desenvolvimento conforme a documentação do SDK.
  3. Compile o projeto utilizando CMake e Make.
  4. Carregue o binário (.uf2) no Raspberry Pi Pico.

## 📚 Contribuições
Contribuições, sugestões de melhorias ou reportes de bugs são muito bem-vindos!  
Para contribuir:
- Abra uma *issue* com a descrição do problema.
- Envie um *pull request* com as alterações propostas.


## 👨‍💻 Autor
- **Lucas Silva**  
- Email: [lucassilva1001@hotmail.com]

---

Divirta-se explorando e modificando o sistema conforme suas necessidades. Caso tenha dúvidas ou sugestões, sinta-se à vontade para contribuir!

