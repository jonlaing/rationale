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
