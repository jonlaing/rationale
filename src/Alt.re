module type Basic = {
  type t('a);
  let alt: (t('a), t('a)) => t('a);
};

module type Basic2 = {
  type t('a, 'b);
  let alt: (t('a, 'b), t('a, 'b)) => t('a, 'b);
};

module MakeBasic = (M: Basic) => {
  type t('a) = M.t('a);
  let alt = M.alt;
  let (<|>) = M.alt;
};

module MakeBasic2 = (M: Basic2) => {
  type t('a, 'b) = M.t('a, 'b);
  let alt = M.alt;
  let (<|>) = M.alt;
};
