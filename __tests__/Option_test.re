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
    expect((ofResult(Belt.Result.Ok(true)), ofResult(Belt.Result.Error(false))))
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

test("tryWith", () => expect((tryWith(() => raise(Not_found)), tryWith(() => 3)))
  |> toEqual((None, Some(3)))
)

test("map", () =>
  expect((map(x => x + 1, Some(1)), map(x => x + 1, None)))
  |> toEqual((Some(2), None))
);

test("fold", () =>
  expect((
    fold((acc, num) => acc + num, 2, Some(2)),
    fold((acc, num) => acc + num, 2, None),
  ))
  |> toEqual((4, 2))
);

test("filter", () =>
  expect((
    filter(x => x > 0, Some(2)),
    filter(x => x > 0, Some(-1)),
    filter(x => x > 0, None),
  ))
  |> toEqual((Some(2), None, None))
);

test("exists", () =>
  expect((
    exists(x => x > 0, Some(2)),
    exists(x => x > 0, Some(-1)),
    exists(x => x > 0, None),
  ))
  |> toEqual((true, false, false))
);

test("flatten", () =>
  expect((flatten(None), flatten(Some(Some(2)))))
  |> toEqual((None, Some(2)))
);

test("flatMap", () =>
  expect(flatMap(x => Some(x + 1), Some(2))) |> toEqual(Some(3))
);

test("compare", () =>
  expect((
    compare(String.compare, None, None),
    compare(String.compare, Some("a"), None),
    compare(String.compare, Some("a"), Some("b")),
    compare(String.compare, None, Some("b")),
  ))
  |> toEqual((0, 1, (-1), (-1)))
);

test("equal", () =>
  expect((
    equal((==), None, None),
    equal((==), Some("a"), None),
    equal((==), Some("a"), Some("a")),
    equal((==), None, Some("b")),
  ))
  |> toEqual((true, false, true, false))
);

test("toList", () =>
  expect((toList(None), toList(Some(2)))) |> toEqual(([], [2]))
);

test("ofList", () =>
  expect((ofList([]), ofList([1, 2, 3]))) |> toEqual((None, Some(1)))
);