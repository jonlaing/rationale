/** Get the first element of a list */
let head: list('a) => option('a);

/** Get everything except the first element of the list */
let tail: list('a) => option(list('a));

/** Get the element of a list at a particular index */
let nth: (int, list('a)) => option('a);

/** Get everything except the last element of a list */
let init: list('a) => option(list('a));

/** Get the last element of a list */
let last: list('a) => option('a);

/** Append a value to the end of a list */
let append: ('a, list('a)) => list('a);

/** Combine two lists */
let concat: (list('a), list('a)) => list('a);

/** Take the first N elements from a list */
let take: (int, list('a)) => list('a);

/** Take the lsat N elements of a list */
let takeLast: (int, list('a)) => list('a);

/** Take elements from the front of a list while a predicate is true */
let takeWhile: ('a => bool, list('a)) => list('a);

/** Take elements from the back of a list while a predicate is true */
let takeLastWhile: ('a => bool, list('a)) => list('a);

/** Take all but the first N elements of a list */
let drop: (int, list('a)) => list('a);

/** Take all but the last N elements for a list */
let dropLast: (int, list('a)) => list('a);

/** Drop the last elements of a list while a predicate is true */
let dropLastWhile: ('a => bool, list('a)) => list('a);

let dropRepeatsWith: (('a, 'a) => bool, list('a)) => list('a);

let dropRepeats: list('a) => list('a);

let dropWhile: ('a => bool, list('a)) => list('a);

let splitAt: (int, list('a)) => (list('a), list('a));

let adjust: ('a => 'a, int, list('a)) => list('a);

let aperature: (int, list('a)) => list(list('a));

let containsWith: (('a, 'b) => bool, 'a, list('b)) => bool;

let contains: ('a, list('a)) => bool;

let endsWith: ('a, list('a)) => bool;

let find: ('a => bool, list('a)) => option('a);

let findIndex: ('a => bool, list('a)) => option(int);

let findLast: ('a => bool, list('a)) => option('a);

let findLastIndex: ('a => bool, list('a)) => option(int);

let groupWith: ('a => bool, list('a)) => list(list('a));

let indexOf: ('a, list('a)) => option(int);

let insert: (int, 'a, list('a)) => list('a);

let insertAll: (int, list('a), list('a)) => list('a);

let intersperse: ('a, list('a)) => list('a);

let join: (string, list(string)) => string;

let lastIndexOf: ('a, list('a)) => option(int);

let none: ('a => bool, list('a)) => bool;

let partition: ('a => bool, list('a)) => (list('a), list('a));

let range: ('a => 'a, 'a, 'a) => list('a);

let rangeInt: (int, int, int) => list(int);

let rangeFloat: (float, float, float) => list(float);

let reduceWhile: ('a => bool, ('b, 'a) => 'b, 'b, list('a)) => 'b;

let reject: ('a => bool, list('a)) => list('a);

let remove: (int, int, list('a)) => list('a);

let repeat: ('a, int) => list('a);

let scan: (('a, 'b) => 'a, 'a, list('b)) => list('a);

let slice: (int, int, list('a)) => list('a);

let splitEvery: (int, list('a)) => list(list('a));

let splitWhen: ('a => bool, list('a)) => (list('a), list('a));

let startsWith: ('a, list('a)) => bool;

let times: (int => list('a), int) => list('a);

let uniqWithBy: (('a, 'a) => bool, 'b => 'a, list('b)) => list('b);

let uniqBy: ('a => 'b, list('a)) => list('a);

let uniqWith: (('a, 'a) => bool, list('a)) => list('a);

let uniq: list('a) => list('a);

let unionWith: (('a, 'a) => bool, list('a), list('a)) => list('a);

let union: (list('a), list('a)) => list('a);

let update: ('a, int, list('a)) => list('a);

let without: (list('a), list('a)) => list('a);

let zipWith: (('a, 'b) => 'c, list('a), list('b)) => list('c);

let differenceWith: (('a, 'b) => bool, list('a), list('b)) => list('a);

let difference: (list('a), list('a)) => list('a);

let intersection: (list('a), list('a)) => list('a);

let zip: (list('a), list('b)) => list(('a, 'b));