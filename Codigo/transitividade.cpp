/*
Ideia de Solução

Ordena topologicamente os nós
Acessa eles até adicionar todos (prioriza os primeiros)
- Adiciona cada nó direcionado ao atual ao grupo
- Set do grupo do nó atual com o grupo
- Chama para cada nó que aponta desse, com o grupo atualizado para conter ele também
	- Se o nó que está sendo chamado já tem todos membros do grupo que está sendo
    chamado pra ele, não adiciona essa aresta e não chama método


Falhas atuais:
- 'Adiciona cada nó direcionado ao atual ao grupo' falha em tomar conta nós 
  conectados que estejam a uma distância maior que 1

      -> B--> C-->  
X--> A             Z    Nesse exemplo, N não será adicionado ao grupo de A, então
      <--M <--N-->     sua conexão com Z será adicionada, apesar de redundante


Pensando nesse mesmo exemplo. Podemos talvez remover essa cláusula e ao
invés criar um check para comparar, só percorrer sempre, e se o grupo novo tiver
contido nele algum dos antigos, remover o antigo

*/