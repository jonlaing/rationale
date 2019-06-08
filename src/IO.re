type io('a) = IO(Lazy.t('a));

let runIO = (IO(f)) => Lazy.force(f);

include Monad.MakeBasic({
  type t('a) = io('a);
  let return = a => IO(lazy (a));

  let bind = (iov, fn) => IO(
    lazy (
      runIO(fn(runIO(iov)))
    )
  );
  let fmap = `DefineWithBind;
});


