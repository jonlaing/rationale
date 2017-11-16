let eq = (a, b) => a == b;

let eqBy = (f, a, b) => eq(f(a), f(b));

let identical = (a, b) => a === b;