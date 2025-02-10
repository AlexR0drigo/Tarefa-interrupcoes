# Tarefa 27/01 - Interrupções 

Este projeto tem como objetivo a implementação e utilização de interrupções no microcontrolador RP2040, explorando a placa de desenvolvimento BitDogLab. A aplicação inclui o uso de LEDs comuns e LEDs endereçáveis WS2812, além da manipulação de botões com tratamento de bouncing via software.

## Funcionalidades

O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo.

O botão A incrementa o número exibido na matriz de LEDs a cada pressionamento.

O botão B decrementa o número exibido na matriz de LEDs a cada pressionamento.

A matriz de LEDs WS2812 exibe números de 0 a 9 em formato digital.

Implementação de interrupções para lidar com os botões de forma eficiente.

Tratamento de bouncing via software para evitar leituras indesejadas dos botões.

## Como executar o projeto

Passo 1: Clone o repositório do projeto para o seu ambiente local e abri-lo no VS Code.

Passo 2: Antes de compilar o código, é necessário garantir que todas as extensões e ferramentas estão devidamente instaladas.

Instalar o Pico SDK e extensão Raspberry Pi Pico: Siga as instruções do Pico SDK para configurar o ambiente de desenvolvimento no VS Code e instale a extensão Raspberry Pi Pico.

Passo 3: Com o ambiente devidamente configurado, abra o código no VS Code e compile o projeto a partir do menu de compilação da extensão Raspberry Pi Pico.

Passo 4: Após a compilação, conecte a placa de desenvolvimento BitDogLab ao computador via USB e coloque ela no modo bootsel.

Passo 5: No mesmo menu da extensão Raspberry Pi Pico selecione a opção "Run Project (USB)" para carrecar o uf2 do código na placa e a partir dai já poderá ver o comportamento esperado do código.


## Link para o video 

https://drive.google.com/drive/folders/1cOvivlfGcfRL578HgfG3-990rDUF1gCL?usp=drive_link

## Projeto e video por Alex Rodrigo 
