palin([ ]).
palin(L) :- first(L, U), last(L, U), middle(L, M), palin(M).

append([], X, X).
append([X | Y], Z, [X | W]) :- append(Y, Z, W).

first([U | _], U).
last([U], U).
last([_ | Tail], U) :- last(Tail, U).

middle([ ], [ ]).
middle([_], [ ]).
middle([_ | Tail], M) :- append(M, [_], Tail).
