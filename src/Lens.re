type t('a, 'b) = {
  get: 'a => 'b,
  set: ('b, 'a) => 'a
};

let make = (getter, setter) => {get: getter, set: setter};

let (||>) = (f, g, x) => g(f(x));

let view = (l, a) => l.get(a);

let set = (l, v, a) => l.set(v, a);

let over = (l, f, a) => {
  let v = l.get(a);
  l.set(f(v), a)
};

let compose = (l0, l1) => {get: l1.get ||> l0.get, set: l0.set ||> over(l1)};

let (-<<) = compose;

let pipe = (l0, l1) => compose(l1, l0);

let (>>-) = pipe;

let optional = (default) => {
  get: (a) =>
    switch a {
    | Some(v) => v
    | None => default
    },
  set: (v, _) => Some(v)
};
