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

let default = (d, o) =>
  switch o {
  | None => d
  | Some(a) => a
  };