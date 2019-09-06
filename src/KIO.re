type io('a, 'env) =
  | IO(Lazy.t('env => 'a));

let runIO: (io('a, 'env), 'env) => 'a = (IO(f), env) => Lazy.force(f)(env)

let lift = fn => IO(lazy (fn));

include Monad.MakeBasic2({
  type t('a, 'env) = io('a, 'env);
  let return = a => IO(lazy (_ => a));

  let bind = (iov: io('a, 'env), fn: ('a) => io('b, 'c)) => IO(lazy (env => {
    let a = runIO(iov, env);
    let rb = fn(a);
    runIO(rb, env);
  }));
  let fmap = `DefineWithBind;
});
