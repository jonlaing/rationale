module type Basic = {
  include Alt.Basic;
  let empty: unit => t('a);
};

module MakeBasic = (M: Basic) => {
  type t('a) = M.t('a);
  let alt = M.alt;
  let empty = M.empty;
  let (<|>) = M.alt;
};
