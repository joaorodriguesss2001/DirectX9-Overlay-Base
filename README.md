# DirectX 9 Hooking & Overlay Framework

Este projeto é um framework de desenvolvimento em C++ focado em manipulação de memória e injeção de código em aplicações DirectX 9. Desenvolvido para fins educacionais e estudo de Engenharia Reversa.

##  Tecnologias Utilizadas
* **C++ Moderno** (Gerenciamento de memória, Ponteiros)
* **Windows API** (Injeção de DLL, Threads, Input Handling)
* **DirectX 9 SDK** (Manipulação da VTable, Render States)
* **ImGui** (Interface Gráfica interna)
* **Reverse Engineering** (Análise de Offsets e Estruturas)

##  Funcionalidades Técnicas
* **VTable Hooking:** Interceptação das funções `EndScene` (42) e `DrawIndexedPrimitive` (82) para renderização customizada.
* **Detour & Trampoline:** Implementação segura de desvio de fluxo de execução.
* **Device Reset Handling:** Tratamento de perda de dispositivo (Alt+Tab) para evitar crashes de GPU.
* **WndProc Hooking:** Interceptação de mensagens do Windows para controle de input (Mouse/Teclado) dentro do Overlay.

##  Aviso Legal
Este software foi desenvolvido estritamente para fins de aprendizado sobre como sistemas operacionais e APIs gráficas funcionam.
