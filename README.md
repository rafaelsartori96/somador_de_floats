# Um pequeno somador de números em ponto flutuante

Neste exercício, construiremos um pequeno somador de números. Ele recebe como
entrada uma string e identifica nela todos os números em formato float ou ponto
flutuante, e então retorna a soma desses números. O sistema não precisa operar
com números negativos, mas deve rejeitar números que estão "grudados" em strings
(veja os exemplos). A resposta deve ser dada em tão poucas casas decimais quanto
possível. Os números podem ter zeros à esquerda (`003` é igual a `3`),
mas a notação `.3` para representar `0.3` é inválida. Caso não haja números na
entrada, o programa deve retornar zero.

## Exemplos

Entrada | Saida
------- | -----
`1.5 2.5` | `4`
`Teste 1.5 2.5 teste` | `4`
`Teste1.5 2.5 teste` | `2.5`
`Teste 1 2 3 teste` | `6`
`1 2 3teste` | `3`
`teste`  | `0`


## Prazo de submissão
O prazo de submissão desta tarefa é às 23:59 do dia marcado. Não serão aceitos
atrasos.

## Instruções adicionais

1. Vá diretamente ao conjunto de testes para verificar como tratar casos
   limítrofes ou mal-definidos.
1. Lembre-se de seguir as [instruções de submissão](doc/instrucoes.md).
1. Se desejar, proponha novos casos de teste.
