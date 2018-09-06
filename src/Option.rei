let some: 'a => option('a);

let none: 'a => option('b);

let isNone: option('a) => bool;

let isSome: option('a) => bool;

let default: ('a, option('a)) => 'a;

let ofResult: Belt.Result.t('a, 'b) => option('a);

let toExn: (string, option('a)) => 'a;

let firstSome: (option('a), option('a)) => option('a);

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