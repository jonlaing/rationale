let always = (x, _y) => x;

let compose = (f, g, x) => f(g(x));

let pipe = (f, g, x) => g(f(x));

let false_ = x => always(false, x);

let true_ = x => always(true, x);

let flip = (f, a, b) => f(b, a);

let identity = x => x;

module Infix = {
  let (<||) = compose;
  let (||>) = pipe;
};

include Infix;
