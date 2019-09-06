module type Basic = {
  type t('a);
  let apply: (t('a => 'b), t('a)) => t('b);
  let pure: 'a => t('a);
};

module type Basic2 = {
  type t('a, 'e);
  let apply: (t('a, 'e => 'b), t('a, 'e)) => t('b, 'e);
  let pure: 'a => t('a, 'e);
};

module MakeBasic = (M: Basic) => {
  type t('a) = M.t('a);
  let apply = M.apply;
  let pure = M.pure;
  let (<*>) = M.apply;
};

module MakeBasic2 = (M: Basic2) => {
  type t('a, 'e) = M.t('a, 'e);
  let apply = M.apply;
  let pure = M.pure;
  let (<*>) = M.apply;
};
