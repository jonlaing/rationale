let some = a => Some(a);

let none = _ => None;

let isNone = o =>
  switch (o) {
  | None => true
  | _ => false
  };

let isSome = o =>
  switch (o) {
  | Some(_) => true
  | _ => false
  };

let default = (d, o) =>
  switch (o) {
  | None => d
  | Some(a) => a
  };

let ofResult = r =>
  Belt.Result.(
    switch (r) {
    | Ok(a) => Some(a)
    | Error(_) => None
    }
  );

let toExn = (err, o) =>
  switch (o) {
  | None => raise(Failure(err))
  | Some(a) => a
  };

let firstSome = (a, b) =>
  switch (a) {
  | None => b
  | _ => a
  };

let tryWith = f =>
  try (Some(f())) {
  | _ => None
  };

include Monad.MakeBasic({
  type t('a) = option('a);
  let bind = (o, f) =>
    switch (o) {
    | None => None
    | Some(a) => f(a)
    };
  let return = some;
  let fmap = `DefineWithBind;
});

include Applicative.MakeBasic({
  type t('a) = option('a);
  let apply = (o, a) =>
    switch (o) {
    | Some(f) => bind(a, b => return(f(b)))
    | _ => None
    };
  let pure = some;
});

module Infix = {
  let (>>=) = (>>=);
  let (<$>) = (<$>);
  let (<*>) = (<*>);
  let (|?) = firstSome;
};

let map = (f, o) => Function.flip((<$>), f, o);

let iter = (f, o) =>
  switch (o) {
  | None => ()
  | Some(x) => f(x)
  };

let fold = (f, acc, o) =>
  switch (o) {
  | None => acc
  | Some(x) => f(acc, x)
  };

let filter = (p, o) =>
  switch (o) {
  | Some(x) as o when p(x) => o
  | _ => None
  };

let exists = (p, o) =>
  switch (o) {
  | None => false
  | Some(x) => p(x)
  };

let flatten = o =>
  switch (o) {
  | None => None
  | Some(x) => x
  };
/*
  flatten(None) == None
  flatten(Some(Some(1))) == 1
 */

let flatMap = (f, o) => Function.flip((>>=), f, o)

let compare = (f, o1, o2) =>
  switch (o1, o2) {
  | (None, None) => 0
  | (Some(_), None) => 1
  | (None, Some(_)) => (-1)
  | (Some(x), Some(y)) => f(x, y)
  };

let equal = (f, o1, o2) =>
  switch (o1, o2) {
  | (None, None) => true
  | (None, Some(_)) => false
  | (Some(_), None) => false
  | (Some(x), Some(y)) => f(x, y)
  };

let toList = o =>
  switch (o) {
  | None => []
  | Some(x) => [x]
  };

let ofList = l =>
  switch (l) {
  | [x, ..._] => Some(x)
  | [] => None
  };