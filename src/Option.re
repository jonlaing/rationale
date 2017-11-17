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

let fmap = (f, o) =>
  switch o {
  | None => None
  | Some(a) => Some(f(a))
  };

let bind = (o, f) =>
  switch o {
  | None => None
  | Some(a) => f(a)
  };

let (>>=) = bind;

let return = some;

let ap = (o, a) =>
  switch o {
  | Some(f) => Some(f(a))
  | _ => None
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

let firstSome = (a, b) =>
  switch a {
  | None => b
  | _ => a
  };