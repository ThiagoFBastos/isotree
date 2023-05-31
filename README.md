algoritmo para encontrar árvores não rotuladas com n (<= 24, mas testado até 21) vértices
usando backtracking e prunning: 
- os vértices são rotulados
- a árvore é enraizada
- a raíz faz parte do centro
- os vértices mais próximos da raíz tem rótulos maiores
- o filho de maior rótulo de cada vértice tem a maior altura
- se um filho é folha então não existe outro com menor rótulo que não é folha

# uso

- make
- ./trees (número de vértices) (flag = 0 ou 1) 
	- se flag é diferente de 0: ./trees (número de vértices) 1 (arquivo binário)
	- exemplo: ./trees 20 1 trees.dat

# teste

para ver se o arquivo está correto (não tem árvores duplicadas nem está mal formatado)

- use ./teste (arquivo binário) (flag = 0 ou 1)
- se flag é diferente de 0: ./teste (arquivo binário) 1 (arquivo de saida)
	- exemplo: ./teste trees.dat 1 trees.txt
