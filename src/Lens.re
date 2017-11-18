/* Inspired by: https://github.com/avsm/ocaml-lens */
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

let head = {
  get: RList.head,
  set: (v, xs) =>
    switch v {
    | None => xs
    | Some(a) => RList.tail(xs) |> Option.default([]) |> RList.append(a)
    }
};

let tail = {
  get: RList.tail,
  set: (v, xs) =>
    switch v {
    | None => xs
    | Some(a) => Option.(RList.head(xs) |> fmap((xs) => RList.append(xs, a)) |> default(a))
    }
};

let index = (i) => {
  get: RList.nth(i),
  set: (v, xs) =>
    switch v {
    | None => xs
    | Some(a) => RList.update(a, i, xs)
    }
};

let prop = (k) => {
  get: Dict.get(k),
  set: (v, d) =>
    switch v {
    | None => d
    | Some(a) => Dict.set(k, a, d)
    }
};

let listMap = (l) => {get: List.map(l.get), set: List.map2(l.set)};

let first = {get: fst, set: (v, a) => (v, snd(a))};

let second = {get: snd, set: (v, a) => (fst(a), v)};