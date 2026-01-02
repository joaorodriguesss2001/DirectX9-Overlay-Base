#pragma once
#pragma once
#include <iostream>
#include <cmath>
#include <vector>

// Isso diz pro compilador: "Não apague isso, mesmo que pareça inútil!"
#pragma optimize("", off)

void LixoMatematico1() {
    volatile int a = 154;
    volatile int b = 33;
    for (int i = 0; i < 50; i++) {
        a = (a * i) + (b - i);
        b = (b % 2) + a;
    }
}

void LixoVetores() {
    std::vector<int> lixo;
    for (int i = 0; i < 100; i++) {
        lixo.push_back(i * 3);
    }
    lixo.clear(); // Limpa pra não pesar na memória
}

void LixoString() {
    std::string s = "TextoAleatorioParaMudarOByteDoArquivo";
    for (int i = 0; i < s.length(); i++) {
        s[i] = s[i] + 1;
    }
}

void ExecutarLixo() {
    // Chama tudo misturado
    LixoMatematico1();
    LixoString();
    LixoMatematico1();
    LixoVetores();
}

#pragma optimize("", on)