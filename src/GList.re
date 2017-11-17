let head = (xs) => Option.ofExn(List.hd(xs));

let tail = (xs) => Option.ofExn(List.tl(xs));

let nth = (i, xs) => Option.ofExn(List.nth(xs, i));

let init = (xs) => Option.(xs |> List.rev |> tail >>| List.rev);

let last = (xs) => xs |> List.rev |> head;

let append = (a, xs) => List.append(xs, [a]);

let concat = (xs, ys) => List.append(ys, xs);

let take = {
  let rec loop = (i, xs, acc) =>
    switch (i, xs) {
    | (i, _) when i <= 0 => acc
    | (_, []) => acc
    | (i, [a, ...b]) => loop(i - 1, b, append(a, acc))
    };
  (i, xs) => loop(i, xs, [])
};

let takeLast = (i, xs) => List.rev(xs) |> take(i) |> List.rev;

let takeWhile = {
  let rec loop = (pred, xs, acc) =>
    switch xs {
    | [] => acc
    | [a, ...b] => pred(a) ? loop(pred, b, append(a, acc)) : acc
    };
  (pred, xs) => loop(pred, xs, [])
};

let takeLastWhile = (pred, xs) => List.rev(xs) |> takeWhile(pred) |> List.rev;

let rec drop = (i, xs) =>
  switch (i, xs) {
  | (_, []) => []
  | (i, _) when i <= 0 => xs
  | (i, [_, ...b]) => drop(i - 1, b)
  };

let dropLast = (i, xs) => take(List.length(xs) - i, xs);

let dropLastWhile = {
  let rec loop = (pred, xs, acc) =>
    switch xs {
    | [] => acc
    | [a] => pred(a) ? append(a, acc) : acc
    | [a, ...b] => pred(a) ? loop(pred, b, append(a, acc)) : acc
    };
  (pred, xs) => loop(pred, xs, [])
};

let dropRepeatsWith = {
  let rec loop = (pred, xs, acc) =>
    switch xs {
    | [] => acc
    | [a, ..._] when List.length(acc) == 0 => loop(pred, xs, append(a, acc))
    | [a, ..._] =>
      Option.(
        last(acc) >>| pred(a) |> default(false) ?
          loop(pred, xs, append(a, acc)) : loop(pred, xs, acc)
      )
    };
  (pred, xs) => loop(pred, xs, [])
};

let dropRepeats = (xs) => dropRepeatsWith((x, y) => x == y, xs);

let rec dropWhile = (pred, xs) =>
  switch xs {
  | [] => []
  | [a, ...b] => pred(a) ? dropWhile(pred, b) : b
  };

let splitAt = (i, xs) => (take(i, xs), takeLast(i - 1, xs));

let adjust = (f, i, xs) => {
  let (a, b) = splitAt(i, xs);
  switch a {
  | _ when i < 0 => xs
  | [] => b
  | [a] => [f(a), ...b]
  | a =>
    Option.(
      init(a) >>= ((x) => last(a) >>| f >>| Function.flip(append, x)) >>| concat(b) |> default(xs)
    )
  }
};

let aperature = {
  let rec loop = (i, xs, n, acc) =>
    if (n == i) {
      acc
    } else {
      let (_, b) = splitAt(n, xs);
      loop(i, xs, n + 1, append(take(n, b), acc))
    };
  (i, xs) => loop(i, xs, 0, [])
};

let containsWith = (f, x) => List.exists((y) => f(x, y));

let contains = (x) => containsWith(Util.eq, x);

let endsWith = (a, xs) => Option.(last(xs) >>| Util.eq(a) |> default(false));

let find = (pred, xs) => Option.ofExn(List.find(pred, xs));

let findIndex = {
  let rec loop = (pred, xs, i) =>
    switch xs {
    | [] => None
    | [a, ...b] => pred(a) ? Some(i) : loop(pred, b, i + 1)
    };
  (pred, xs) => loop(pred, xs, 0)
};

let findLast = (pred, xs) => xs |> List.rev |> find(pred);

let findLastIndex = (pred, xs) =>
  switch (findIndex(pred, List.rev(xs))) {
  | None => None
  | Some(a) => Some(List.length(xs) - a)
  };

let groupWith = {
  let rec loop = (pred, xs, acc) =>
    switch xs {
    | [] => acc
    | _ => loop(pred, dropWhile(pred, xs), append(takeWhile(pred, xs), acc))
    };
  (pred, xs) => loop(pred, xs, [[]])
};

let indexOf = (a, xs) => findIndex((x) => a == x, xs);

let insert = (i, x, xs) => {
  let (a, b) = splitAt(i, xs);
  a |> append(x) |> concat(b)
};

let insertAll = (i, xs, ys) => {
  let (a, b) = splitAt(i, ys);
  a |> concat(xs) |> concat(b)
};

let intersperse = (a, xs) => List.fold_left((acc, v) => concat([a, v], acc), [List.hd(xs)], xs);

let join = (j) => List.fold_left((acc, v) => String.length(acc) == 0 ? v : acc ++ j ++ v, "");

let lastIndexOf = (a, xs) =>
  switch (indexOf(a, List.rev(xs))) {
  | None => None
  | Some(a) => Some(List.length(xs) - a)
  };

let none = (pred, xs) => ! List.exists(pred, xs);

let partition = (pred, xs) => (List.filter(pred, xs), List.filter((x) => ! pred(x), xs));

let range = {
  let rec loop = (inc, s, e, acc) =>
    Option.(
      switch acc {
      | [] => [s]
      | acc when inc(last(acc) |> default(e)) > e => acc
      | _ =>
        switch (last(acc)) {
        | None => acc
        | Some(a) => loop(inc, s, e, append(inc(a), acc))
        }
      }
    );
  (inc, s, e) => loop(inc, s, e, [])
};

let rangeInt = (step) => range((x) => x + step);

let rangeFloat = (step) => range((x) => x +. step);

let reduceWhile = (pred, f, x, xs) =>
  List.fold_left(
    (acc, v) =>
      switch acc {
      | (false, _) => acc
      | (_, a) => pred(v) ? (true, f(a, v)) : (false, a)
      },
    (true, x),
    xs
  )
  |> snd;

let reject = (pred) => List.filter((x) => ! pred(x));

let remove = (i, n, xs) => {
  let (a, b) = splitAt(i, xs);
  a @ drop(n, b)
};

let repeat = {
  let rec loop = (a, n, acc) =>
    switch n {
    | 0 => acc
    | n => loop(a, n - 1, append(a, acc))
    };
  (a, n) => loop(a, n, [])
};

let scan = (f: ('a, 'b) => 'a, i: 'a) =>
  Option.(
    List.fold_left(
      (acc, v) => last(acc) >>| Function.flip(f, v) >>| Function.flip(append, acc) |> default([]),
      [i]
    )
  );

let slice = (a, b, xs) => xs |> splitAt(a) |> snd |> splitAt(b) |> fst;

let splitEvery = {
  let rec loop = (n, xs, acc) =>
    switch xs {
    | [] => acc
    | xs => loop(n, drop(n, xs), append(take(n, xs), acc))
    };
  (n, xs) => loop(n, xs, [])
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
      loop(f, n, i + 1, concat(f(i), acc))
    } else {
      acc
    };
  (f, n) => loop(f, n, 0, [])
};

let uniqWithBy = (eq, f, xs) =>
  List.fold_left(
    (acc, v) => Option.(last(acc) >>| f >>| eq(f(v)) |> default(false) ? acc : append(v, acc)),
    [],
    xs
  );

let uniqBy = (f, xs) => uniqWithBy(Util.eq, f, xs);

let uniqWith = (eq, xs) => uniqWithBy(eq, Function.identity, xs);

let uniq = (xs) => uniqBy(Function.identity, xs);

let unionWith = (f, xs, ys) => concat(ys, xs) |> uniqWith(f);

let union = (xs, ys) => concat(ys, xs) |> uniq;

let update = (x, i, xs) => adjust(Function.always(x), i, xs);

let without = (exclude, xs) => List.filter((x) => contains(x, exclude), xs);

let zipWith = {
  let rec loop = (f, xs, ys, acc) =>
    switch (xs, ys) {
    | ([a, ...b], [c, ...d]) => loop(f, b, d, append(f(a, c), acc))
    | (_, _) => acc
    };
  (f, xs, ys) => loop(f, xs, ys, [])
};

let differenceWith = {
  let rec loop = (f, xs, ys, acc) =>
    switch xs {
    | [] => acc
    | [a, ...b] => containsWith(f, a, ys) ? loop(f, b, ys, acc) : loop(f, b, ys, append(a, acc))
    };
  (f, xs, ys) => loop(f, xs, ys, [])
};

let difference = (xs, ys) => differenceWith(Util.eq, xs, ys);

let intersection = {
  let rec loop = (xs, ys, acc) =>
    switch xs {
    | [] => acc
    | [a, ...b] => contains(a, ys) ? loop(b, ys, append(a, acc)) : loop(b, ys, acc)
    };
  (xs, ys) => loop(xs, ys, [])
};

let zip = {
  let rec loop = (xs, ys, acc) =>
    switch (xs, ys) {
    | ([], _)
    | (_, []) => acc
    | ([a, ...b], [c, ...d]) => loop(b, d, append((a, c), acc))
    };
  (xs, ys) => loop(xs, ys, [])
};