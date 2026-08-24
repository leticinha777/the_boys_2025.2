# 🦸‍♂️ The Boys - Simulação de Eventos Discretos em C

Projeto prático desenvolvido para a disciplina de **Programação 1**.

O **The Boys** é um simulador em C onde heróis se deslocam por um mundo virtual, reúnem-se em bases, formam equipes para cumprir missões e enfrentam o risco de morte em combate. Toda a dinâmica é controlada por uma simulação dirigida por eventos de tempo discreto.

---

## 💡 Como Funciona a Simulação

- **Heróis e Bases:** Cada herói possui atributos próprios (habilidade, paciência, velocidade) e viaja entre bases pelo mapa.
- **Formação de Equipes:** Ao chegarem nas bases, os heróis entram em filas de espera e formam grupos com base na lotação e paciência.
- **Missões:** Eventos periódicos exigem conjuntos específicos de habilidades; se a equipe presente for forte o suficiente, a missão é cumprida, mas os heróis podem morrer - caso contrário, a missão pode ser adiada.
- **Relatório Final:** Ao término do tempo de simulação, o programa calcula e exibe estatísticas gerais (missões cumpridas, taxa de mortalidade, dados das bases).
- **Gerenciamento de Memória:** Toda a memória alocada dinamicamente é devidamente liberada ao final da execução.

---

## 🛠️ Estruturas e Conceitos Utilizados

- **Linguagem:** C
- **TADs & Estruturas de Dados:**
  - `Filas` e `Listas Encadeadas` (gerenciamento de eventos, filas nas bases e conjuntos de habilidades)
  - `Vetores` e `Structs` (entidades como Heróis, Bases e Missões)
- **Modularização:** Divisão limpa do código em múltiplos arquivos auxiliares (`.h` e `.c`) e `main`.
- **Lógica e Ponteiros:** Alocação dinâmica (`malloc`/`free`) com foco em prevenção de *memory leaks*.

---
