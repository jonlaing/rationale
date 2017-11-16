let allPass = (fs, x) => List.fold_left((acc, f) => acc ? f(x) : acc, true, fs);

let and_ = (a, b) => a && b;

let anyPass = (fs, x) => List.fold_left((acc, f) => acc ? acc : f(x), false, fs);

let both = (f, g, x) => f(x) && g(x);

let either = (f, g, x) => f(x) || g(x);

let not_ = (f, x) => ! f(x);

let or_ = (a, b) => a || b;

let unless = (pred, f, x) => pred(x) ? x : f(x);

let rec until = (pred, f, x) => pred(x) ? x : until(pred, f, f(x));

let when_ = (pred, f, x) => unless(not_(pred), f, x);