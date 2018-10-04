module type Basic = {
  type t('a);
  let apply: (t('a => 'b), t('a)) => t('b);
  let pure: 'a => t('a);
};

module MakeBasic = (M: Basic) => {
  type t('a) = M.t('a);
  let apply = M.apply;
  let pure = M.pure;
  let (<*>) = M.apply;
};
