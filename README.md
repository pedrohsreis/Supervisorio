# Supervisorio - Rinobot SPL


Este repositório contém o código fonte da aplicação de supervisionamento do funcionamento do robô NAO. Foi desenvolvido e é mantido pela equipe Rinobot.


# Code Release

A aplicação foi desenvolvida pra funcionar com o Code Release da Rinobot, mas pode ser convertido para funcionar com outros códigos-fonte.


# Funcionamento

Com a integração dos módulos programados no Code Release, o Supervisório opera as seguintes funções:

* Envio e recebimento de mensagens (UDP/TCP), com níveis de: Debug, Warning, Info e Error.
* Visualização da câmera, podendo controlar na aplicação os níveis de: Saturação, Contraste, Brilho e Sharpness.
* Há a possibilidade de selecionar qual Code Release será utilizado, através da busca do arquivo.
* Os códigos podem ser re-compilados, configurados e limpos através do uso da interface.

# Log

O log é uma caixa de texto disponível na interface da aplicação.
Essa caixa de texto é atualizada automaticamente, através dos eventos da aplicação.
Através do log é possível saber qual o estado passado e atual de execução da aplicação. Faz-se útil para identificação de possiveis erros.

# Código

A conexão entre o robô e o supervisório é realizada através de uma conexão Servidor - Cliente, usando o protocolo TCP.
Através dessa conexão, usando estrutura de classes e tipos abstratos de dados, o Supervisório pode desempenhar as demais funções citadas acima.

# Campo de Futebol

O Supervisório também disponibiliza a possibilidade de uma visualização do campo em tempo real.
Através da localização do robô, em um mapa 2D é mostrada a localização dos robôs conectados no campo.
Para mais informações, consulte o Code Release - SPL Rinobot.
