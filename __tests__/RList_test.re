open Jest;

open Expect;

open RList;

test("head", () => expect((head([1]), head([]))) |> toEqual((Some(1), None)));

test("tail", () => expect((tail([1, 2]), tail([]))) |> toEqual((Some([2]), None)));

test("nth", () => expect((nth(0, [1]), nth(1, [1]))) |> toEqual((Some(1), None)));

test("init", () => expect((init([1, 2]), init([]))) |> toEqual((Some([1]), None)));

test("last", () => expect((last([1, 2]), init([]))) |> toEqual((Some(2), None)));

test("append", () => expect(append(1, [])) |> toEqual([1]));

test("concat", () => expect(concat([1], [0])) |> toEqual([0, 1]));

describe(
  "take",
  () => {
    test("too small", () => expect(take(3, [1, 2])) |> toEqual([1, 2]));
    test("big enough", () => expect(take(3, [1, 2, 3, 4])) |> toEqual([1, 2, 3]))
  }
);

describe(
  "takeLast",
  () => {
    test("too small", () => expect(takeLast(3, [1, 2])) |> toEqual([1, 2]));
    test("big enough", () => expect(takeLast(3, [1, 2, 3, 4])) |> toEqual([2, 3, 4]))
  }
);