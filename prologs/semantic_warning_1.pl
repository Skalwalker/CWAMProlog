/* khalil estuda iaa_2_Noturno */
estuda(khalil, iaa_2_Noturno).
/* guilherme ensina iaa_2_Noturno */
ensina(guilherme, iaa_2_Noturno).


/* renato estuda comp_conc2 */
estuda(renato, comp_conc2).
estuda(renato, comp_conc2) :- teste(a).
/* geraldo ensina comp_conc2 */
ensina(geraldo, comp_conc2).

estuda(a, b, c).

/* X é um professor de Y se X
ensina C e Y estua C */
professor(X, Aluno) :- ensina(X, Aula_1), estuda(Aluno, Aula_1).
