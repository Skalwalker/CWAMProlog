
ensina(guilherme, iaa_2_Noturno)
professor(X, Aluno)  ensina(X, Aula_1), estuda(Aluno Aula_1).

middle([_], [ ]).
middle([_  Tail, M) :- append(M, [_], Tail).

