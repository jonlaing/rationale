open Option.Infix;

open Function.Infix;

type t('a) = list((string, 'a));

let findEntry = k => fst ||> Util.eq(k);

let get = (k, d) => RList.find(findEntry(k), d) <$> snd;

let set = (k, v, d) =>
  switch (RList.findIndex(findEntry(k), d)) {
  | None => RList.append((k, v), d)
  | Some(i) => RList.update((k, v), i, d)
  };

let unset = (k, d) => RList.reject(findEntry(k), d);

let eqProps = (k, d0: t('a), d1: t('a)) =>
  Option.(Some(Util.eq) <*> get(k, d0) <*> get(k, d1) |> default(false));

let map = f => List.map(((k, v)) => (k, f(v)));

let mapi = f => List.map(((k, v)) => (k, f(k, v)));

let evolve = (e, d) =>
  mapi(
    (k, v) =>
      switch (get(k, e)) {
      | None => v
      | Some(f) => f(v)
      },
    d,
  );

let has = (k, d) => get(k, d) <$> Function.true_ |> Option.default(false);

let invert = d =>
  List.fold_left(
    (acc, (k, v)) =>
      set(v, get(v, acc) <$> RList.append(k) |> Option.default([k]), acc),
    [],
    d,
  );

let keys = d => List.map(fst, d);

let merge = (d0, d1) =>
  List.fold_left((acc, (k, v)) => set(k, v, acc), d0, d1);

let mergeWithKey = (f, d0, d1) => {
  let intersect = RList.intersection(keys(d0), keys(d1));
  List.map(
    k =>
      Option.(
        k,
        Some(f(k))
        <*> get(k, d0)
        <*> get(k, d1)
        |> toExn("Error merging Dicts"),
      ),
    intersect,
  );
};

let mergeWith = (f, d0, d1) => mergeWithKey(Function.always(f), d0, d1);

let omit = (ks, d) => RList.reject(((k, _)) => RList.contains(k, ks), d);

let pickBy = (pred, d) => List.filter(((k, v)) => pred(k, v), d);

let pick = ks => pickBy((k, _) => RList.contains(k, ks));

let project = (ks, ds) => List.map(pick(ks), ds);

let values = d => List.map(snd, d);

let where = (predD, d) =>
  List.fold_left(
    (acc, (k, f)) =>
      acc ? Option.(Some(f) <*> get(k, d) |> default(false)) : false,
    true,
    predD,
  );

let whereEq = (d0, d1) => map(Util.eq, d0) |> Function.flip(where, d1);

let filter = f => List.filter(((_, v)) => f(v));

let filteri = f => List.filter(((k, v)) => f(k, v));

let fold_left = f => List.fold_left((acc, (k, v)) => f(acc, k, v));

let fold_right = f => List.fold_right(((k, v), acc) => f(k, v, acc));