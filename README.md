Gerber
================================================================================

Named after the well-known multi-tool brand, Gerber is inspired by [RamdaJS](http://ramdajs.com/). It is a collection of helper utility functions that are absent in the OCaml/ReasonML standard library.

Note that not all of Ramda was ported over, as many of Ramda's utilities are making up for deficits in Javascript, which Reason doesn't have. Furthermore, many of the functions that operate on objects, simply don't make sense in Reason.

Features
--------------------------------------------------------------------------------

### Exception-free List oprations

In the OCaml/ReasonML standard library, many of the common List operations throw exceptions if there's a problem. Gerber's utilities do not throw exceptions, and instead return `options`.

- head
- tail
- init
- last
- nth
- etc

### Monadic Options and Js.Results

Gerber includes monadic and functor operations ala Haskell for the `option` and `Js.Result` types.

```Reason
open Gerber;

Option.(
  GList.init(a)
  >>= ((x) => GList.last(a) >>| ((y) => GList.append(f(y), x)))
  >>| GList.concat(b)
  |> default(xs)
);
```

### Support for Point-free style

Gerber has `compose` and `pipe` functions, as well as supporting infix operators: `<||` and `||>` respectively.

### Infix Lens composition

Gerber also allows for fluid lens composition via infix operators: `-<<` and `>>-`.

```Reason
Lens.view(aLens >>- bLens >>- optional([]), { a: { b: Some(3) } });

```

### Function signatures for composition

Like in Ramda, functions always keep their data at the end making piping and composing a breeze:

```Reason
list
  |> take(9)
  |> drop(3)
  |> splitAt(4);
```