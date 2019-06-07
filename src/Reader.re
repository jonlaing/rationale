type reader('e, 'a) = Reader('e => 'a);


let runReader: (reader('a, 'b), 'a) => 'b = (Reader(f), a) => f(a);
let ask = () => Reader(env => env);

include Monad.MakeBasic2({
  type t('e, 'a) = reader('a, 'e);
  let run = fun | Reader(f) => f;
  let return = a => Reader(_ => a);

  let bind = (ra, fn) => Reader(env => {
    let a = run(ra, env);
    let rb = fn(a);
    run(rb, env);
  });
  let fmap = `DefineWithBind;
});

/*
include Applicative.MakeBasic2({
  type t('e, 'a) = reader('e, 'a);
  let pure = a => Reader(_ => a);
  /*let apply = (Reader(f), a) => {
    bind(a, b => return(f(b)));
  };*/
  let apply = (f, Reader(a)) => {
    Reader(env => f(env));
  }    
});
*/