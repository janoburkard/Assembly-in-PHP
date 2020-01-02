# Assembly em PHP

## Introdução

Quando falamos em rodar instruções Assembly em PHP, você poderia usar C para escrever uma extensão e dentro desta incluir Assembly inline.

Fugindo do pensamento comum escrevi uma biblioteca usando apenas PHP.

Lista de Opcodes implementados: PUSH, POP, NOP, MOV, CMP, CMC, CLC, NOP, ADD, SUB, INC, **AND**, **OR**, **XOR**, NOT, NEG, ROL, ROR, SHR, SHL, LEA.

## História

Durante a Engenharia Reversa de um software de monitoramento de DVRs (Gravador de vídeo para CFTV) foi possível identificar a função responsável que gera a Hash das senhas do dispositivo. Como o conjunto de instruções era muito longo e possivelmente o tempo gasto para entender e reescrever a rotina também seria, achei interessante para este e outros projetos, ter uma biblioteca que permitisse **simular** a execução de um conjunto de instruções x86.

Durante a Palestra "[Firmware Hacking](https://www.linkedin.com/posts/jan%C3%B4-falkowski-burkard-514248a0_palestra-firmware-hacking-apresentada-no-activity-6607810092978982912-V3P7)" na BHack 2019 foi apresentada uma idéia do que seria este projeto.

## Dahua Hash

A empresa **Dahua Technology** que é uma das maiores fabricantes de CFTV do mundo criou um gerador de Hash para as senhas dos dispositivos. O Hash é baseado no MD5 da senha que posteriormente é enviada para uma função que aplica a operação módulo que encontra o resto da divisão e posteriormente realiza um conjunto de testes para gerar um intervalo de caracteres ASCII.

# Agradecimentos

Primeiramente agradeço a **Ewerson Guimarães (Crash)** que gentilmente me convidou para palestrar na BHack 2019.

Em segundo lugar as pessoas que me incentivaram a publicar este código.
* Alexandra Percário
* Osmany Arruda
* Júlio Della Flora
* Alexandre Borges
