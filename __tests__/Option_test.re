open Jest;

open Expect;

open Option;

test(
  "some",
  () =>
    switch (some(1)) {
    | Some(a) => expect(a) |> toEqual(1)
    | _ => expect(0) |> toEqual(1)
    }
);

test(
  "none",
  () =>
    switch (none(1)) {
    | None => expect(true) |> toEqual(true)
    | _ => expect(false) |> toEqual(true)
    }
);

test("isNone", () => expect((isNone(None), isNone(Some(1)))) |> toEqual((true, false)));

test("isSome", () => expect((isSome(Some(1)), isSome(None))) |> toEqual((true, false)));

test(
  "default",
  () => expect((default(false, Some(true)), default(false, None))) |> toEqual((true, false))
);

test(
  "ofResult",
  () =>
    expect((ofResult(Js.Result.Ok(true)), ofResult(Js.Result.Error(false))))
    |> toEqual((Some(true), None))
);

/* TODO: Figure out how to test throwing an exception */
describe("toExn", () => test("Some", () => expect(toExn("blah", Some(true))) |> toEqual(true)));

test(
  "firstSome",
  () =>
    expect((firstSome(Some(0), Some(1)), firstSome(None, Some(1)))) |> toEqual((Some(0), Some(1)))
);

describe(
  "Monad",
  () => {
    test(
      "left identity",
      () => {
        let f = (x) => Some(x + 1);
        expect(return(0) >>= f) |> toEqual(f(0))
      }
    );
    test("right identity", () => expect(Some(0) >>= return) |> toEqual(Some(0)));
    test(
      "associativity",
      () => {
        let f = (x) => Some(x + 1);
        let g = (x) => Some(x + 3);
        expect(Some(0) >>= f >>= g) |> toEqual(Some(0) >>= ((x) => f(x) >>= g))
      }
    )
  }
);

describe(
  "Functor",
  () => {
    test(
      "identity",
      () => expect(Some(0) <$> Function.identity) |> toEqual(Some(0) |> Function.identity)
    );
    test(
      "composition",
      () => {
        open Function;
        let f = (x) => x + 1;
        let g = (x) => x + 3;
        expect(Some(0) <$> (f <|| g)) |> toEqual(Some(0) <$> g <$> f)
      }
    )
  }
);