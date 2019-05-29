type tx('a, 'w) = {
  runWriter: ('a, 'w)
}
type writer('a, 'w) = Writer(tx('a, 'w));

module type LogType = {
  type t;
}

module MakeWriter = (M: Monoid.General) => {
  include (Monad.MakeBasic({
    type t('a) = writer('a, M.t);
    let return = a => Writer({ runWriter: (a, M.empty)});
    let bind = (Writer({ runWriter: (x, v)}), f) => {
      let (Writer({ runWriter: (y, vp)})) = f(x);
      Writer({ runWriter: (y, M.append(v, vp))})
    }
    let fmap = `DefineWithBind;
  }));
  include (Applicative.MakeBasic({
    type t('a) = writer('a, M.t);
    let pure = a => Writer({ runWriter: (a, M.empty)});
    let apply = (Writer({ runWriter: (f, v)}), a) => {
      let (Writer({ runWriter: (fp, vp)})) = bind(a, b => return(f(b)));
      Writer({ runWriter: (fp, M.append(v, vp))});
    };
  }));
}

module WriterString = MakeWriter({
  type t = string;
  let empty = "";
  let append = (a, b) => a ++ b;
  let rec concat = xs => switch xs {
    | [] => ""
    | [a, ...rs] => append(a, concat(rs))
  };
});

module WriterSum = MakeWriter({
  type t = int;
  let empty = 0;
  let append = (a, b) => a + b;
  let rec concat = xs => switch xs {
    | [] => 0
    | [a, ...rs] => append(a, concat(rs))
  };
})
