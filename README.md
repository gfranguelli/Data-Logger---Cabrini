<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <title>Data Logger com Arduino</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            background-color: #f5f5f5;
            color: #333;
        }
        h1 {
            text-align: center;
            color: #2c3e50;
        }
        h2 {
            color: #34495e;
            border-bottom: 2px solid #ddd;
            padding-bottom: 5px;
        }
        p {
            line-height: 1.6;
        }
        ul {
            margin-left: 20px;
        }
        .container {
            background: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
    </style>
</head>
<body>

<div class="container">

    <h1>Documentação do Projeto – Data Logger com Arduino</h1>

    <h2>1. Introdução</h2>
    <p>
        Este projeto consiste no desenvolvimento de um data logger baseado em Arduino,
        capaz de monitorar e registrar variáveis ambientais como temperatura, umidade e luminosidade.
    </p>
    <p>
        A proposta surgiu da necessidade de controlar condições ambientais em locais onde essas variáveis são críticas,
        como adegas, farmácias, armazéns e ambientes de armazenamento sensíveis.
    </p>

    <h2>2. Objetivo do Projeto</h2>
    <ul>
        <li>Monitorar continuamente condições ambientais</li>
        <li>Permitir ao usuário definir limites aceitáveis</li>
        <li>Emitir alertas em caso de valores fora do padrão</li>
        <li>Ser de fácil utilização e configuração</li>
    </ul>

    <h2>3. Aplicações</h2>
    <ul>
        <li>Armazenamento de vinhos</li>
        <li>Farmácias</li>
        <li>Armazéns de alimentos</li>
        <li>Ambientes industriais ou laboratoriais</li>
    </ul>

    <h2>4. Componentes Utilizados</h2>
    <ul>
        <li>Arduino Uno</li>
        <li>Sensor de temperatura</li>
        <li>Sensor de umidade e luminosidade</li>
        <li>Display LCD 20x4</li>
        <li>Botões (push buttons)</li>
        <li>Buzzer</li>
        <li>LEDs</li>
        <li>Estrutura impressa em 3D</li>
    </ul>

    <h2>5. Funcionamento do Sistema</h2>
    <p>
        O sistema realiza a leitura contínua das variáveis ambientais e as exibe no display LCD.
    </p>
    <p>
        O usuário pode configurar limites mínimos e máximos para cada variável.
        Por exemplo, temperatura entre 10°C e 25°C.
    </p>
    <p>
        Caso algum valor esteja fora do intervalo definido:
    </p>
    <ul>
        <li>O buzzer é acionado (alerta sonoro)</li>
        <li>Os LEDs piscam (alerta visual)</li>
    </ul>

    <h2>6. Interface do Usuário</h2>
    <p>A interação com o sistema é feita por meio de botões, permitindo:</p>
    <ul>
        <li>Configuração de limites</li>
        <li>Alteração da unidade de temperatura (°C / °F)</li>
        <li>Mudança de idioma (Português, Inglês, Espanhol)</li>
    </ul>

    <h2>7. Estrutura Física</h2>
    <p>
        Foi desenvolvida uma caixa utilizando impressão 3D para acomodar os componentes,
        garantindo melhor acabamento, segurança e organização do sistema.
    </p>

    <h2>8. Conclusão</h2>
    <p>
        O projeto demonstrou a aplicação prática de sistemas embarcados no monitoramento ambiental,
        oferecendo uma solução simples, eficiente e adaptável para diferentes contextos.
    </p>
    <p>
        Além disso, permitiu o desenvolvimento de habilidades em programação com Arduino,
        integração de sensores, interface com o usuário e prototipagem física.
    </p>

</div>

</body>
</html>
