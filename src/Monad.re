module type General = {
  type t('a, 'i, 'j, 'd, 'e);
  let bind:
    (t('a, 'i, 'j, 'd, 'e), 'a => t('b, 'j, 'k, 'd, 'e)) =>
    t('b, 'i, 'k, 'd, 'e);
  let fmap: [
    | `DefineWithBind
    | `Custom(('a => 'b, t('a, 'i, 'j, 'd, 'e)) => t('b, 'i, 'j, 'd, 'e))
  ];
  let return: 'a => t('a, 'i, 'i, 'd, 'e);
};

module type Basic = {
  type t('a);
  let bind: (t('a), 'a => t('b)) => t('b);
  let return: 'a => t('a);
  let fmap: [ | `DefineWithBind | `Custom(('a => 'b, t('a)) => t('b))];
};

module type Infix = {
  type t('a);
  let (>>=): (t('a), 'a => t('b)) => t('b);
  let (<$>): (t('a), 'a => 'b) => t('b);
};

module type Basic2 = {
  type t('a, 'e);
  let bind: (t('a, 'e), 'a => t('b, 'e)) => t('b, 'e);
  let return: 'a => t('a, 'e);
  let fmap: [
    | `DefineWithBind
    | `Custom(('a => 'b, t('a, 'e)) => t('b, 'e))
  ];
};

module type Infix2 = {
  type t('a, 'e);
  let (>>=): (t('a, 'e), 'a => t('b, 'e)) => t('b, 'e);
  let (<$>): (t('a, 'e), 'a => 'b) => t('b, 'e);
};

module MakeGeneral = (M: General) => {
  let bind = M.bind;
  let return = M.return;
  let fmap =
    switch (M.fmap) {
    | `DefineWithBind => ((f, m) => M.bind(m, a => M.return(f(a))))
    | `Custom(f) => f
    };
  let (>>=) = M.bind;
  let (<$>) = (m, f) => fmap(f, m);
  let join = m => m >>= (a => a);
  let all = {
    let rec loop = (vs, ms) =>
      switch (ms) {
      | [] => return(List.rev(vs))
      | [t, ...ts] => t >>= (v => loop([v, ...vs], ts))
      };
    ts => loop([], ts);
  };
  let rec all_ignore = ms =>
    switch (ms) {
    | [] => return()
    | [t, ...ts] => t >>= (() => all_ignore(ts))
    };
};

module MakeBasic = (M: Basic) =>
  MakeGeneral({
    type t('a, 'i, 'j, 'd, 'e) = M.t('a);
    include (M: Basic with type t('a) := M.t('a));
  });

module MakeBasic2 = (M: Basic2) =>
  MakeGeneral({
    type t('a, 'i, 'j, 'd, 'e) = M.t('a, 'd);
    include (M: Basic2 with type t('a, 'd) := M.t('a, 'd));
  });