palin([ ]).
palin(L) :- 1first(L, U), last(L, U), middle(L, M), palin(M).

append([], X, X).
append([X | Y], Z, [X / W]) :- append(Y, Z, W).

1first([U | _], U).
last([U], U).
last([_ | Tail], U) :- last(Tail, U).

/*
    This is a
    multiline
    comment
*/
middle([ ], [ ]).
middle([_], [ ]).
middle([_ | Tail], M) := append(M, [_], Tail).
