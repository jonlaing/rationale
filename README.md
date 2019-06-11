Rationale
================================================================================

Rationale is inspired by [RamdaJS](http://ramdajs.com/). It is a collection of helper utility functions that are absent in the OCaml/ReasonML standard library.

Note that not all of Ramda was ported over, as many of Ramda's utilities are making up for deficits in Javascript, which Reason doesn't have. Furthermore, many of the functions that operate on objects, simply don't make sense in Reason.

Installation
--------------------------------------------------------------------------------

Run `npm install --save rationale` and add `rationale` to `bs-dependencies` in `bsconfig.json`. 

Features
--------------------------------------------------------------------------------

### Exception-free List operations

In the OCaml/ReasonML standard library, many of the common List operations throw exceptions if there's a problem. Rationale's utilities do not throw exceptions, and instead return `options`.

- head
- tail
- init
- last
- nth
- etc

### Monadic Options and Belt.Results

Rationale includes monadic and functor operations ala Haskell for the `option` and `Belt.Result` types.

```Reason
open Rationale.Option.Infix;
open Rationale.Function;

RList.init(a)
  >>= ((x) => RList.last(a) <$> f <$> flip(RList.append, x))
  <$> RList.concat(b)
  |> Option.default(xs);
```

### Additional ADT-s
  #### Reader
  #### Writer
  #### IO, KIO
  The main idea of the IO monad is to isolate our effects as much possible. Some languages like Haskell don't even
  allow the user to manually start an effect, which is not the case for Reason, but with a little bit of self-discipline we can handle side effects in a monadic fashion as well.

  KIO is a monadic structure with the type signature: ```io('a, 'env) = IO(Lazy.t('env => 'a))```
  which makes it posible to store some effect with its config environment. ( dependency injection )

  We can use `return` or `lift` methods to wrap our unsafe mutations.
  The difference is that with `return` we throw away the environment, while with `lift` we are using it.

  As you can see in our example we used `return` to wrap `Js.log`  
  ```reason
  let log = (x: string) =>
    KIO.return({
      Js.log(x)
    });
  ```

  In the following `saveFile` example we use lift which makes it possible to use an injected `env` config
  ( the injection happens when we call `KIO.runIO` )
  ```reason
  let saveFile = str => KIO.lift(env => {
    Node.Fs.writeFileSync(env.target, str, `ascii);
  });
  ```

  To run our effects we need to call: KIO.runIO(main, env);
  Ideally in your program this method will be called once on the bottom of your index file.
  Example:

  ```reason
    exception ReadError(string);
    type envT = {
      path: string,
      dir: string,
      target: string
    };
    let env = {path: "./input.txt", dir: "/", target: "./out.txt"};

    let readFile = KIO.lift(env => {
        try (Node.Fs.readFileSync(env.path, `ascii)) {
        | ReadError(msg) => raise @@ ReadError("File read failed: " ++ msg)
        };
      });

    let saveFile = str => KIO.lift(env => {
      Node.Fs.writeFileSync(env.target, str, `ascii);
    });

    let log = (x: string) =>
      KIO.return({
        Js.log(x)
      });

    let parseFile = input => {
      let l  = Js.String.split("\n", input);
      Array.map(x => x ++ "100", l);
    } 

    let joinArray = (xs: array(string)) => Js.Array.join(xs)

    let main = KIO.(
      readFile 
        <$> parseFile
        <$> joinArray
        >>= saveFile
    )

    KIO.runIO(main, env);
  ```

### Support for Point-free style

Rationale has `compose` and `pipe` functions, as well as supporting infix operators: `<||` and `||>` respectively.

### Infix Lens composition

Rationale also allows for fluid lens composition via infix operators: `-<<` and `>>-`.

```Reason
Lens.view(aLens >>- bLens >>- optional(0), { a: { b: Some(3) } });

```

### Function signatures for composition

Like in Ramda, functions always keep their data at the end making piping and composing a breeze:

```Reason
list
  |> take(9)
  |> drop(3)
  |> splitAt(4);
```

Usage
--------------------------------------------------------------------------------

### Using Optional Returns in RList and Dict

Returning `option('a)` from functions is generally preferred to throwing an exception.
It protects you from runtime errors, and forces you to deal with potential errors at
compile time. However, if you're not used to doing it, things can get a little
confusing.

Pattern matching for errors all the time would be extremely cumbersome. Fortunately,
we provide a host of useful methods to working with optional returns. Hopefully,
this doc will show you that you don't need to use excessive pattern matching to
work with optional returns.

#### Default

The most straight forward way to get out of an `option` is by calling `default`.

```Reason
Option.default(0, Some(1)); /* 1 */

Option.default(0, None); /* 0 */
```

#### Monads

Calling `default` will definitely get you out of the `option`, but what if you want
to do some things to it first? What if you need other funtions that also return `option`?

the Option module includes monadic operations for `option`, so you can take a *railway oriented*
approach to working with them.

First, let's check if the last item of a list is equal to a certain value:

```Reason
let lastEquals = (a, xs) => Option.fmap(Util.eq(a), RList.last(xs)) |> Option.default(false);

lastEquals(3, [1,2,3]); /* true */
lastEquals(3, [4,5,6]); /* false */
lastEquals(3, []); /* false */

/* Or, with infix operators */
open Option.Infix;

let lastEquals = (a, xs) => RList.last(xs) <$> Util.eq(a) |> Option.default(false);
```

Here we used `fmap` and its infix variation `<$>` to apply a function to the value *inside* the option.
Note that, `Util.eq` returns a `bool` not an `option`. So what if the next function also returns an
`option`? Well you'd get nested options, which doesn't really help anyone. So, instead, we would
use `bind`.

Now let's replace the last item of one list with the last item of another. Note that both `last` and
`init` return `option`:

```Reason
let swapLast = (xs, ys) =>
  Option.(bind(RList.last(xs), ((x) => fmap(RList.append(x), RList.init(ys)))) |> default(ys));

swapLast([1,2,3], [4,5,6]); /* [4,5,3] */
swapLast([], [4,5,6]); /* [4,5,6] */

/* Or, with infix operators */
open Option.Infix;

let swapLast = (xs, ys) =>
  RList.last(xs) >>= ((x) => RList.init(ys) <$> RList.append(x)) |> Option.default(ys);
```

Here we used `bind` and its infix variation `>>=` to apply a function that also returned an `option`.

#### Applicatives

Let's try checking if the last elements of two lists are equal. We could accomplish this using `bind`,
but that can be a little awkward.

```Reason
let lastEqual = (xs, ys) =>
  Option.(apply(apply(Some(Util.eq), RList.last(xs), RList.last(ys))) |> default(false));

lastEqual([1,2,3], [4,5,3]); /* true */
lastEqual([1,2,3], [4,5,6]); /* false */
lastEqual([], [4,5,6]); /* false */
lastEqual([1,2,3], []); /* false */

/* Or, with infix operators */
open Option.Infix;

let lastEqual = (xs, ys) =>
  Some(Util.eq) <*> RList.last(xs) <*> RList.last(ys) |> Option.default(false);
```

### Alternative

With alternative you can implement simple but powerful fallback mechanism your ADT-s;

Example:

```Reason
  open Option;
  let someData = some("Hello");
  let guard = fun
  | true => pure() 
  | _ => empty();


  let startWith: (string, string) => option(unit) = (str, char) => guard(Js.String.startsWith(char, str))
  let dataWithFallback =
    someData 
      >>= val_ => startWith(val_, "T")
      >>= (_ => {Js.log(val_); some(Js.String.toUpperCase(val_))})
      <|> some("Not started with T")
```

### Translating JS Idioms

#### Or chains

Take the following example in Javascript:

```Javascript
let x = a || b || c || d;
```

We can't translate that directly to Reason, because there is no `null` or `undefined` in Reason.
The closest approximation would be `option`, in which we can string together `Some` and `None`
to get the first one that is `Some`.

There is a helper function called `firstSome` and its infix variation `|?` that do exactly this.

```Reason
/* a, b, and c are all options, but d is not */
let x = a |? b |? c |> default(d);
```

Reference
--------------------------------------------------------------------------------

### Infix Operators

- `>>=`: Monadic Bind
- `<$>`: Functor Fmap
- `<*>`: Applicative Apply
- `<||`: Point-free Function Compose
- `||>`: Point-free Function Pipe
- `-<<`: Lens Compose
- `>>-`: Lens Pipe
- `|?`: Optional Or
