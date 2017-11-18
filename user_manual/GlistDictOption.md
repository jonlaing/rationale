Using Optional Returns in GList and Dict
================================================================================

Returning `option('a)` from functions is generally preferred to throwing an exception.
It protects you from runtime errors, and forces you to deal with potential errors at
compile time. However, if you're not used to doing it, things can get a little
confusing.

Pattern matching for errors all the time would be extremely cumbersome. Fortunately,
we provide a host of useful methods to working with optional returns. Hopefully,
this doc will show you that you don't need to use excessive pattern matching to
work with optional returns.

## Default

The most straight forward way to get out of an `option` is by calling `default`.

```Reason
Option.default(0, Some(1)); /* 1 */

Option.default(0, None); /* 0 */
```

## Monads

Calling `default` will definitely get you out of the `option`, but what if you want
to do some things to it first? What if you need other funtions that also return `option`?

the Option module includes monadic operations for `option`, so you can take a *railway oriented*
approach to working with them.

First, let's check if the last item of a list is equal to a certain value:

```Reason
let lastEquals = (a, xs) => Option.fmap(Util.eq(a), GList.last(xs)) |> Option.default(false);

lastEquals(3, [1,2,3]); /* true */
lastEquals(3, [4,5,6]); /* false */
lastEquals(3, []); /* false */

/* Or, with infix operators */
open Option.Infix;

let lastEquals = (a, xs) => GList.last(xs) <$> Util.eq(a) |> Option.default(false);
```

Here we used `fmap` and its infix variation `<$>` to apply a function to the value *inside* the option.
Note that, `Util.eq` returns a `bool` not an `option`. So what if the next function also returns an
`option`? Well you'd get nested options, which doesn't really help anyone. So, instead, we would
use `bind`.

Now let's replace the last item of one list with the last item of another. Note that both `last` and
`init` return `option`:

```Reason
let swapLast = (xs, ys) =>
  Option.(bind(Glist.last(xs), ((x) => fmap(append(x), init(ys)))) |> default(ys));

swapLast([1,2,3], [4,5,6]); /* [4,5,3] */
swapLast([], [4,5,6]); /* [4,5,6] */

/* Or, with infix operators */
open Option.Infix;
open Option.Infix;

let swapLast = (xs, ys) =>
  Glist.last(xs) >>= ((x) => init(ys) <$> append(x)) |> Option.default(ys);
```

Here we used `bind` and its infix variation `>>=` to apply a function that also returned an `option`.

## Applicatives

Let's try checking if the last elements of two lists are equal. We could accomplish this using `bind`,
but that can be a little awkward.

```Reason
let lastEqual = (xs, ys) =>
  Option.(apply(apply(Some(Util.eq), GList.last(xs), GList.last(ys))) |> default(false));

lastEqual([1,2,3], [4,5,3]); /* true */
lastEqual([1,2,3], [4,5,6]); /* false */
lastEqual([], [4,5,6]); /* false */
lastEqual([1,2,3], []); /* false */

/* Or, with infix operators */
let lastEqual = (xs, ys) =>
  Some(Util.eq) <*> GList.last(xs) <*> GList.last(ys) |> Option.default(false);
```

Hope you found that useful!