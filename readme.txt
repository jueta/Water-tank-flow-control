/*
*		UFMG
*       TRABALHO AUTOMACAO EM TEMPO REAL
*       ALUNO: JOAO PEDRO MIRANDA MARQUES
*       MATRICULA: 2017050495
*       PARTE 1
*
*/



Como usar:
 - O programa vai lhe solicitar que escolha os valores de referencia para os tanques 1, 2 e 3.
 - Em seguida serao printados:
	-> "free x": quando o controlador ja fez o calculo e atuou na entrada do processo, sendo x o processo.
	-> "task x waiting": Quando o processo estiver aguardando a entrada
	-> "task x Done": Quando a saida do processo tiver sido calculada (altura de agua no tanque).
	-> "resposta do sistema x no instante y": Saida do processo x no instante de tempo y.


Configuracoes no codigo:
 - Formato e volume do tanque: Dentro de cada dinamic_equation pode-se setar os valores de altura do tanque (H), raio inferior (rzin) e raio superior (rzao).
 - Constante Gama: Dentro da funcao qout pode-se setar a constante gama de saida de agua pelo furo inferior de cada tanque.
 - Periodo do controlador: a variavel definida como CONTROLLER_PERIOD pode ser setada como o periodo de cada ciclo na malha fechada de controle. 