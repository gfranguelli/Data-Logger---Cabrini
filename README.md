# 📊 Data Logger com Arduino

## 📌 Introdução
Este projeto consiste no desenvolvimento de um **data logger baseado em Arduino**, capaz de monitorar e registrar variáveis ambientais como **temperatura, umidade e luminosidade**.

A proposta surgiu da necessidade de controlar condições ambientais em locais onde essas variáveis são críticas, como **adegas, farmácias, armazéns e ambientes de armazenamento sensíveis**.

---

## 🎯 Objetivo do Projeto
- Monitorar continuamente condições ambientais  
- Permitir ao usuário definir limites aceitáveis  
- Emitir alertas em caso de valores fora do padrão  
- Ser de fácil utilização e configuração  

---

## 🧪 Aplicações
- Armazenamento de vinhos  
- Farmácias  
- Armazéns de alimentos  
- Ambientes industriais ou laboratoriais  

---

## 🔧 Componentes Utilizados
- Arduino Uno  
- Sensor de temperatura  
- Sensor de umidade e luminosidade  
- Display LCD 20x4  
- Botões (push buttons)  
- Buzzer  
- LEDs  
- Estrutura impressa em 3D  

---

## ⚙️ Funcionamento do Sistema
O sistema realiza a leitura contínua das variáveis ambientais e as exibe no display LCD.

O usuário pode configurar limites mínimos e máximos para cada variável.  
Exemplo:
- Temperatura: entre **10°C e 25°C**

Caso algum valor esteja fora do intervalo definido:
- 🔊 O buzzer é acionado (alerta sonoro)  
- 💡 Os LEDs piscam (alerta visual)  

---

## 🖥️ Interface do Usuário
A interação com o sistema é feita por meio de botões, permitindo:

- Configuração de limites  
- Alteração da unidade de temperatura:
  - Celsius (°C)  
  - Fahrenheit (°F)  
- Mudança de idioma:
  - Português  
  - Inglês  
  - Espanhol  

As informações são exibidas de forma clara no display LCD 20x4.

---

## 📦 Estrutura Física
Foi desenvolvida uma caixa utilizando **impressão 3D**, garantindo:
- Melhor acabamento  
- Maior segurança dos circuitos  
- Facilidade de transporte  

---

## ✅ Conclusão
O projeto demonstrou a aplicação prática de sistemas embarcados no monitoramento ambiental, oferecendo uma solução simples, eficiente e adaptável.

Além disso, possibilitou o desenvolvimento de habilidades em:
- Programação com Arduino  
- Integração de sensores  
- Interface com o usuário  
- Prototipagem física  
