type identity('a) = Identity('a);

let lazy_return = Lazy.from_fun(a => Identity(a));
let ap = (Identity(f), a) => Identity(f(a));

include Monad.MakeBasic({
  type t('a) = identity('a);
  let bind = (Identity(a), f) => f(a)
  let return = a => Identity(a);
  let fmap = `DefineWithBind;
});

