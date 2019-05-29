module type General = {
  type t;
  let empty: t;
  let append: (t, t) => t;
  let concat: list(t) => t
  

};

module MakeGeneral = (M: General) => {
  let empty = M.empty;
  let append = M.append;
  let concat = M.concat;
}