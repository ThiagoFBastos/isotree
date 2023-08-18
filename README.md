# Isotree

Algoritmo para encontrar árvores não rotuladas com n (<= 24, mas testado até 22) vértices:
Primeiro construímos a árvore com um vértice isolado e após isso vamos contruindo a árvore
por nível adicionando vértices cujo pai está a um nível acima e respeitando certas restrições.
Assim que a árvore está completa geramos um pattern para a árvore e o inserimos numa hashtable.

## Restrições
- A árvore é enraizada
- Um vértice que está mais perto da raíz possui um rótulo maior do que um que está mais longe
- Para dois vértices que estão no mesmo nível, o que possuir o maior rótulo, todos os seus filhos tem rótulos maiores do que os filhos do vértice de rótulo menor
- Para dois vértices irmãos, a altura da subárvore do que possui o maior rótulo não é menor que a altura da subárvore do vértice de menor rótulo
- A raíz faz parte do centro, então as duas subárvores de maior altura tem diferença de no máximo 1

## Instruções

- digite no terminal:
	```
	make
	```
- para gerar as árvores, digite no terminal:
	```
	./trees <número de vértices> <flag = 0 ou 1>
	```
	- se flag é diferente de 0, digite: 
		```
		./trees <número de vértices> 1 <arquivo binário>
		```
		- exemplo: 
			```
			./trees 20 1 trees.dat
			```
	- senão, digite:
		```
		./trees <número de vértices> 0
		```
		- exemplo:
			```
			./trees 20 0
			```
## Teste

Para ver se o arquivo está correto (não tem árvores duplicadas nem está mal formatado) e listar as matrizes de adjacências.

- digite no terminal: 
	```
	./teste <arquivo binário> <flag = 0 ou 1>	
	```
	- se flag é diferente de 0, digite: 
		```
		./teste <arquivo binário> 1 <arquivo de saida>
		```
		- exemplo:
			```
			./teste trees.dat 1 trees.txt
			```
	- senão, digite: 
		```
		./teste <arquivo binário> 0
		```
		- exemplo
			```
			./teste trees.dat 0
			```
