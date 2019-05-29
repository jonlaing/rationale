open Jest;

open Expect;

open! Writer;

module WriterList = Writer.MakeWriter({
  type t = list(int);
  let empty = [];
  let append = (a, b) => List.append(a, b);
  let concat = xs => List.concat(xs);
});

describe("Monad", () => {
  test("left identity", () => {
    open! Writer.WriterString;
    let add10 = x => Writer({ runWriter: (x + 10, "add10")});
    expect(
      return(5) >>= add10
    ) |> toEqual(add10(5));
  });

  test("right identity", () => {
    open! Writer.WriterSum;
    let init42 = Writer({ runWriter: ("Meaning of life", 42)});
    expect(
      init42 >>= return
    ) |> toEqual(init42)
  });

  test("associativity", () => {
    open! WriterList;
    let init = Writer({ runWriter: (10, [])});
    let f = x => Writer({ runWriter: (x * 100, [1])});
    let g = x => Writer({ runWriter: (x * 200, [2])});

    expect(init >>= f >>= g) |> toEqual(init >>= (x => f(x) >>= g));
  });
});