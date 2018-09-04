type t('a) = list((string, 'a));

let get: (string, t('a)) => option('a);

let set: (string, 'a, t('a)) => t('a);

let unset: (string, t('a)) => t('a);

let eqProps: (string, t('a), t('a)) => bool;

let map: ('a => 'b, t('a)) => t('b);

let mapi: ((string, 'a) => 'b, t('a)) => t('b);

let evolve: (t(('a => 'a)), t('a)) => t('a);

let has: (string, t('a)) => bool;

let invert: t(string) => t(list(string));

let keys: t('a) => list(string);

let merge: (t('a), t('a)) => t('a);

let mergeWith: (('a, 'a) => 'b, t('a), t('a)) => t('b);

let mergeWithKey: ((string, 'a, 'a) => 'b, t('a), t('a)) => t('b);

let omit: (list(string), t('a)) => t('a);

let pick: (list(string), t('a)) => t('a);

let pickBy: ((string, 'a) => bool, t('a)) => t('a);

let project: (list(string), list(t('a))) => list(t('a));

let values: t('a) => list('a);

let where: (t(('a => bool)), t('a)) => bool;

let whereEq: (t('a), t('a)) => bool;

let filter: ('a => bool, t('a)) => t('a);

let filteri: ((string, 'a) => bool, t('a)) => t('a);

let fold_left: (('a, string, 'b) => 'a, 'a, t('b)) => 'a

let fold_right: ((string, 'a, 'b) => 'b, t('a), 'b ) => 'b