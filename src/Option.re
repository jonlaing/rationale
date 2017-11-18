let some = (a) => Some(a);

let none = (_) => None;

let isNone = (o) =>
  switch o {
  | None => true
  | _ => false
  };

let isSome = (o) =>
  switch o {
  | Some(_) => true
  | _ => false
  };

let default = (d, o) =>
  switch o {
  | None => d
  | Some(a) => a
  };

let ofResult = (r) =>
  Js.Result.(
    switch r {
    | Ok(a) => Some(a)
    | Error(_) => None
    }
  );

let ofExn = (x) =>
  switch x {
  | exception _ => None
  | a => Some(a)
  };

let toExn = (o) =>
  switch o {
  | None => raise(Not_found)
  | Some(a) => a
  };

let firstSome = (a, b) =>
  switch a {
  | None => b
  | _ => a
  };

include
  Monad.MakeBasic(
    {
      type t('a) = option('a);
      let bind = (o, f) =>
        switch o {
        | None => None
        | Some(a) => f(a)
        };
      let return = some;
      let fmap = `DefineWithBind;
    }
  );

let ap = (o, a) =>
  switch o {
  | Some(f) => fmap(f, a)
  | _ => None
  };

let (<**>) = ap;