let head = xs =>
  switch (List.hd(xs)) {
  | exception _ => None
  | a => Some(a)
  };

let tail = xs =>
  switch (List.tl(xs)) {
  | exception _ => None
  | a => Some(a)
  };

let nth = (i, xs) =>
  switch (List.nth(xs, i)) {
  | exception _ => None
  | a => Some(a)
  };

let init = xs => Option.(xs |> List.rev |> tail <$> List.rev);

let last = xs => xs |> List.rev |> head;

let any = (pred, xs) =>
  List.fold_left((acc, v) => acc ? acc : pred(v), false, xs);

let all = (pred, xs) =>
  List.fold_left((acc, v) => acc ? pred(v) : acc, true, xs);

let append = (a, xs) => List.append(xs, [a]);

let concat = (xs, ys) => List.append(ys, xs);

let take = {
  let rec loop = (i, xs, acc) =>
    switch (i, xs) {
    | (i, _) when i <= 0 => acc
    | (_, []) => acc
    | (i, [a, ...b]) => loop(i - 1, b, append(a, acc))
    };
  (i, xs) => loop(i, xs, []);
};

let takeLast = (i, xs) => List.rev(xs) |> take(i) |> List.rev;

let takeWhile = {
  let rec loop = (pred, xs, acc) =>
    switch (xs) {
    | [] => acc
    | [a, ...b] => pred(a) ? loop(pred, b, append(a, acc)) : acc
    };
  (pred, xs) => loop(pred, xs, []);
};

let takeLastWhile = (pred, xs) =>
  List.rev(xs) |> takeWhile(pred) |> List.rev;

let rec drop = (i, xs) =>
  switch (i, xs) {
  | (_, []) => []
  | (i, _) when i <= 0 => xs
  | (i, [_, ...b]) => drop(i - 1, b)
  };

let dropLast = (i, xs) => take(List.length(xs) - i, xs);

let rec dropWhile = (pred, xs) =>
  switch (xs) {
  | [] => []
  | [a, ...b] => pred(a) ? dropWhile(pred, b) : xs
  };

let dropLastWhile = (pred, xs) =>
  List.rev(xs) |> dropWhile(pred) |> List.rev;

let dropRepeatsWith = {
  let rec loop = (pred, xs, acc) =>
    switch (xs) {
    | [] => acc
    | [a, ...b] when List.length(acc) == 0 => loop(pred, b, append(a, acc))
    | [a, ...b] =>
      Option.(
        last(acc) <$> pred(a) |> default(false) ?
          loop(pred, b, acc) : loop(pred, b, append(a, acc))
      )
    };
  (pred, xs) => loop(pred, xs, []);
};

let dropRepeats = xs => dropRepeatsWith((x, y) => x == y, xs);

let splitAt = (i, xs) => (take(i, xs), takeLast(List.length(xs) - i, xs));

let adjust = (f, i, xs) => {
  let (a, b) = splitAt(i + 1, xs);
  switch (a) {
  | _ when i < 0 => xs
  | _ when i >= List.length(xs) => xs
  | [] => b
  | [a] => [f(a), ...b]
  | a =>
    Option.(
      init(a)
      >>= (x => last(a) <$> f <$> Function.flip(append, x))
      <$> concat(b)
      |> default(xs)
    )
  };
};

let aperture = {
  let rec loop = (i, xs, acc) =>
    switch (xs) {
    | [] => acc
    | a when List.length(a) < i => acc
    | [_, ...b] => loop(i, b, append(take(i, xs), acc))
    };
  (i, xs) => loop(i, xs, []);
};

let containsWith = f => List.exists(f);

let contains = x => containsWith(Util.eq(x));

let endsWith = (a, xs) =>
  Option.(last(xs) <$> Util.eq(a) |> default(false));

let find = (pred, xs) =>
  switch (List.find(pred, xs)) {
  | exception _ => None
  | a => Some(a)
  };

let findIndex = {
  let rec loop = (pred, xs, i) =>
    switch (xs) {
    | [] => None
    | [a, ...b] => pred(a) ? Some(i) : loop(pred, b, i + 1)
    };
  (pred, xs) => loop(pred, xs, 0);
};

let findLast = (pred, xs) => xs |> List.rev |> find(pred);

let findLastIndex = (pred, xs) =>
  switch (findIndex(pred, List.rev(xs))) {
  | None => None
  | Some(a) => Some(List.length(xs) - a - 1)
  };

let groupWith = {
  let rec loop = (pred: ('a, 'a) => bool, n, xs, acc) =>
    Option.Infix.(
      switch (xs) {
      | [] => List.length(acc) == 0 ? [[]] : acc
      | [a] => append([a], acc)
      | _ when n > List.length(xs) => append(xs, acc)
      | _ =>
        let (x0, x1) = splitAt(n + 1, xs);
        Some(pred) <*> last(x0) <*> head(x1) |> Option.default(true) ?
          loop(pred, n + 1, xs, acc) : loop(pred, 0, x1, append(x0, acc));
      }
    );
  (pred, xs) => loop(pred, 0, xs, []);
};

let indexOf = (a, xs) => findIndex(x => a == x, xs);

let insert = (i, x, xs) => {
  let (a, b) = splitAt(i, xs);
  a |> append(x) |> concat(b);
};

let insertAll = (i, xs, ys) => {
  let (a, b) = splitAt(i, ys);
  a |> concat(xs) |> concat(b);
};

let intersperse = (a, xs) =>
  List.fold_left(
    (acc, v) =>
      List.length(acc) > 0 ? acc |> append(a) |> append(v) : [v],
    [],
    xs,
  );

let join = j =>
  List.fold_left(
    (acc, v) => String.length(acc) == 0 ? v : acc ++ j ++ v,
    "",
  );

let lastIndexOf = (a, xs) =>
  switch (indexOf(a, List.rev(xs))) {
  | None => None
  | Some(a) => Some(List.length(xs) - a - 1)
  };

let none = (pred, xs) => !List.exists(pred, xs);

let partition = (pred, xs) => (
  List.filter(pred, xs),
  List.filter(x => !pred(x), xs),
);

let range = {
  let rec loop = (inc, s, e, acc) =>
    switch (List.rev(acc)) {
    | [] => loop(inc, s, e, [s])
    | [a, ..._] => inc(a) > e ? acc : loop(inc, s, e, append(inc(a), acc))
    };
  (inc, s, e) => loop(inc, s, e, []);
};

let rangeInt = step => range(x => x + step);

let rangeFloat = step => range(x => x +. step);

let reduceWhile = (pred, f, x, xs) =>
  List.fold_left(
    (acc, v) =>
      switch (acc) {
      | (false, _) => acc
      | (_, a) => pred(a, v) ? (true, f(a, v)) : (false, a)
      },
    (true, x),
    xs,
  )
  |> snd;

let reject = pred => List.filter(x => !pred(x));

let remove = (i, n, xs) => {
  let (a, b) = splitAt(i, xs);
  a @ drop(n, b);
};

let repeat = {
  let rec loop = (a, n, acc) =>
    switch (n) {
    | 0 => acc
    | n => loop(a, n - 1, append(a, acc))
    };
  (a, n) => loop(a, n, []);
};

let scan = (f: ('a, 'b) => 'a, i: 'a) =>
  Option.(
    List.fold_left(
      (acc, v) =>
        last(acc)
        <$> Function.flip(f, v)
        <$> Function.flip(append, acc)
        |> default([]),
      [i],
    )
  );

let slice = (a, b, xs) => xs |> splitAt(a) |> snd |> splitAt(b) |> fst;

let splitEvery = {
  let rec loop = (n, xs, acc) =>
    switch (xs) {
    | [] => acc
    | xs => loop(n, drop(n, xs), append(take(n, xs), acc))
    };
  (n, xs) => loop(n, xs, []);
};

let splitWhen = (pred, xs) =>
  switch (findIndex(pred, xs)) {
  | None => (xs, [])
  | Some(a) => splitAt(a, xs)
  };

let startsWith = (x, xs) =>
  switch (head(xs)) {
  | None => false
  | Some(a) => a == x
  };

let times = {
  let rec loop = (f, n, i, acc) =>
    if (i < n) {
      loop(f, n, i + 1, append(f(i), acc));
    } else {
      acc;
    };
  (f, n) => loop(f, n, 0, []);
};

let uniqWithBy = (eq, f, xs) =>
  List.fold_left(
    ((acc, tacc), v) =>
      containsWith(eq(f(v)), tacc) ?
        (acc, tacc) : (append(v, acc), append(f(v), tacc)),
    ([], []),
    xs,
  )
  |> fst;

let uniqBy = (f, xs) => uniqWithBy(Util.eq, f, xs);

let uniqWith = (eq, xs) => uniqWithBy(eq, Function.identity, xs);

let uniq = xs => uniqBy(Function.identity, xs);

let unionWith = (f, xs, ys) => concat(ys, xs) |> uniqWith(f);

let union = (xs, ys) => concat(ys, xs) |> uniq;

let update = (x, i, xs) => adjust(Function.always(x), i, xs);

let without = (exclude, xs) => reject(x => contains(x, exclude), xs);

let zipWith = {
  let rec loop = (f, xs, ys, acc) =>
    switch (xs, ys) {
    | ([a, ...b], [c, ...d]) => loop(f, b, d, append(f(a, c), acc))
    | (_, _) => acc
    };
  (f, xs, ys) => loop(f, xs, ys, []);
};

let differenceWith = {
  let rec loop = (f, xs, ys, acc) =>
    switch (xs) {
    | [] => acc
    | [a, ...b] =>
      containsWith(f(a), ys) ?
        loop(f, b, ys, acc) : loop(f, b, ys, append(a, acc))
    };
  (f, xs, ys) => loop(f, xs, ys, []) @ loop(f, ys, xs, []);
};

let difference = (xs, ys) => differenceWith(Util.eq, xs, ys);

let intersection = {
  let rec loop = (xs, ys, acc) =>
    switch (xs) {
    | [] => acc
    | [a, ...b] =>
      contains(a, ys) ? loop(b, ys, append(a, acc)) : loop(b, ys, acc)
    };
  (xs, ys) => loop(xs, ys, []);
};

let zip = {
  let rec loop = (xs, ys, acc) =>
    switch (xs, ys) {
    | ([], _)
    | (_, []) => acc
    | ([a, ...b], [c, ...d]) => loop(b, d, append((a, c), acc))
    };
  (xs, ys) => loop(xs, ys, []);
};

let fold_lefti = {
  let rec loop = (i, f, acc, xs) =>
    switch (xs) {
    | [] => acc
    | [a, ...b] => loop(i + 1, f, f(acc, i, a), b)
    };
  (f, init, xs) => loop(0, f, init, xs);
};

let fold_righti = {
  let xis = xs => fold_lefti((acc, i, a) => [(i, a), ...acc], [], xs);
  (f, init, xs) =>
    List.fold_left((acc, (i, a)) => f(acc, i, a), init, xis(xs));
};

let filteri = {
  let rec loop = (pred, xs, i, acc) =>
    switch (xs) {
    | [] => acc
    | [a, ...b] =>
      pred(i, a) ?
        loop(pred, b, i + 1, append(a, acc)) : loop(pred, b, i + 1, acc)
    };
  (pred, xs) => loop(pred, xs, 0, []);
};

let filter_map = (pred, f, xs) =>
  List.fold_left((acc, x) => pred(x) ? append(f(x), acc) : acc, [], xs);

let filter_mapi = (pred, f, xs) =>
  fold_lefti(
    (acc, i, a) => pred(i, a) ? append(f(i, a), acc) : acc,
    [],
    xs,
  );

let pure = a => [a];

let create = (f, n) =>
  if (n < 0) {
    [];
  } else {
    let rec loop = (n, acc) =>
      n == 0 ? acc : loop(n - 1, [f(n - 1), ...acc]);
    loop(n, []);
  };

let filter_opt = xs => {
  let rec loop = (l, acc) =>
    switch (l) {
    | [] => acc
    | [hd, ...tl] =>
      switch (hd) {
      | None => loop(tl, acc)
      | Some(x) => loop(tl, [x, ...acc])
      }
    };
  List.rev(loop(xs, []));
};

let reduce = (f, xs) =>
  switch (xs) {
  | [] => None
  | [hd, ...tl] => Some(List.fold_left(f, hd, tl))
  };

let merge = (compare, xs, ys) => {
  let rec loop = (acc, xs, ys) =>
    switch (xs, ys) {
    | ([], ys) => List.rev_append(acc, ys)
    | (xs, []) => List.rev_append(acc, xs)
    | ([h1, ...t1], [h2, ...t2]) =>
      compare(h1, h2) <= 0 ?
        loop([h1, ...acc], t1, ys) : loop([h2, ...acc], xs, t2)
    };
  loop([], xs, ys);
};
