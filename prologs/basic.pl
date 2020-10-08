/* khalil estuda iaa_2_Noturno */
estuda(khalil, iaa_2_Noturno).
/* renato estuda iaa_2_Noturno */
estuda(renato, iaa_2_Noturno).
/* rodrigo estuda comp_conc2 */
estuda(rodrigo, comp_conc2).
/* johannes estuda csc134 */
estuda(johannes, csc134).


/* guilherme ensina iaa_2_Noturno */
ensina(guilherme, iaa_2_Noturno).
/* geraldo ensina comp_conc2 */
ensina(geraldo, comp_conc2).
/* carla ensina comp_2 */
ensina(carla, comp_2).
/* teo ensina csc134 */
ensina(teo, csc134).

/* X é um professor de Y se X
ensina C e Y estua C */
professor(X, Aluno) :- ensina(X, Aula_1), estuda(Aluno, Aula_1).


/*
{printf("Arvore Fato: \n"); print_fact($1);}

estuda
    (
        khalil ,
        iaa_2_Noturno
    )
.

professor
    (
        X ,
        Aluno
    )
:-
ensina
    (
        X,
        Aula_1
    )
, estuda
    (
        Aluno,
        Aula_1
    )
.

*/
