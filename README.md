Algoritmo para encontrar árvores não rotuladas com n (<= 24, mas testado até 22) vértices:
- A árvore é enraizada e construída nível por nível
- Os vértices de um nível = h possuem rótulos maiores que os do nível = h + 1
- Para dois vértices em um mesmo nível, o que possui o maior rótulo tem os filhos com rótulos maiores
- A raiz faz parte do centro (a diferença entre as alturas das duas subárvores mais altas é no máximo 1)
- Para dois vértices irmãos, o que possui o maior rótulo tem a altura da subárvore maior ou igual a um irmão de rótulo menor
- O padrão da árvore (usando algoritmo de Ahu) é inserido em uma tabela hash
  
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
