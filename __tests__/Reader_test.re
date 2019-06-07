open Jest;

open Expect;

open Reader;

describe("Monad", () => {
  test("left identity", () => {
    let add = x => Reader(env => env + x);
    let eq1 = runReader(return(5) >>= add, 10);
    let eq2 = runReader(add(5), 10);
    
    expect(eq1) |> toEqual(eq2);
  });

  test("right identity", () => {
    let init42 = Reader(_ => 42);
    expect(runReader(init42 >>= return, 0)) |> toEqual(runReader(init42, 0));
  });

  test("associativity", () => {
    let init = Reader(_ => 10);
    let f = x => Reader(env => env + x);
    let g = x => Reader(env => env * x);

    let ls = init >>= f >>= g;
    let rs = init >>= (x => f(x) >>= g);
    expect(runReader(ls, 10)) |> toEqual(runReader(rs, 10));
  });
});
