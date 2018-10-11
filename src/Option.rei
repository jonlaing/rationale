let some: 'a => option('a);

let none: 'a => option('b);

let isNone: option('a) => bool;

let isSome: option('a) => bool;

let default: ('a, option('a)) => 'a;

let ofResult: Belt.Result.t('a, 'b) => option('a);

let toExn: (string, option('a)) => 'a;

let firstSome: (option('a), option('a)) => option('a);

let tryWith: (unit => 'a) => option('a);

include Monad.Basic with type t('a) := option('a);

include Applicative.Basic with type t('a) := option('a);

let bind: (option('a), 'a => option('b)) => option('b);

let (>>=): (option('a), 'a => option('b)) => option('b);

let return: 'a => option('a);

let fmap: ('a => 'b, option('a)) => option('b);

let (<$>): (option('a), 'a => 'b) => option('b);

let apply: (option('a => 'b), option('a)) => option('b);

let pure: 'a => option('a);

module Infix: {
  let (>>=): (option('a), 'a => option('b)) => option('b);
  let (<$>): (option('a), 'a => 'b) => option('b);
  let (<*>): (option('a => 'b), option('a)) => option('b);
  let (|?): (option('a), option('a)) => option('a);
};

let map: ('a => 'b, option('a)) => option('b)

let iter: ('a => unit, option('a)) => unit

let fold: (('b, 'a) => 'b, 'b, option('a)) => 'b

let filter: ('a => bool, option('a)) => option('a)

let exists: ('a => bool, option('a)) => bool

let flatten: option(option('a)) => option('a)

let flatMap: ('a => option('b), option('a)) => option('b)

let compare: (('a, 'b) => int, option('a), option('b)) => int

let equal: (('a, 'b) => bool, option('a), option('b)) => bool

let toList: option('a) => list('a)

let ofList: list('a) => option('a)