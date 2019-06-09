module type Basic = {
  type t('a);
  include (Plus.Basic with type t('a) := t('a));
  include (Applicative.Basic with type t('a) := t('a));
};

module MakeBasic = (M: Basic) => {
  type t('a) = M.t('a);
  let apply = M.apply;
  let pure = M.pure;
  let alt = M.alt;
  let empty = M.empty;
  let (<*>) = M.apply;
  let (<|>) = M.alt;
};
