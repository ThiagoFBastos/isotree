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

## Versão paralela
A versão paralela do algoritmo utiliza a biblioteca openmp para dividir a computação das árvores em múltiplos threads. 
A abordagem é bem simples: dado o número máximo de threads a serem lançadas, o algoritmo separa cada sub-árvore da raiz para uma thread disponível e asim ela obtém todas as árvores não-rotuladas alcançáveis e no final das computações de todas as threads, os resultados são combinados para se obter as árvores de forma não repetida.

### Instruções

- digite no terminal:
	```
	make
	```
- para gerar as árvores, digite no terminal:
	```
	./trees <número de vértices> <número de threads> <flag = 0 ou 1>
	```
	- se flag é diferente de 0, digite: 
		```
		./trees <número de vértices> <número de threads> 1 <arquivo binário>
		```
		- exemplo: 
			```
			./trees 20 10 1 trees.dat
			```
	- senão, digite:
		```
		./trees <número de vértices> <número de threads> 0
		```
		- exemplo:
			```
			./trees 20 10 0

### Teste
Os mesmos passos que o sequencial