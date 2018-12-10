open RArray;
open Jest;
open Expect;

test("swap", () => {
  let arr = [|1, 2, 3|];
  swap(0, 1, arr);
  swap(0, 2, arr);
  expect(arr) |> toEqual([|3, 1, 2|]);
});

describe("shuffle", () => {
  test("size 1 arrays", () => {
    let arr = [|1|];
    shuffle(arr);
    expect(arr) |> toEqual([|1|]);
  });
  let arr = [|1, 2, 3, 4, 5|];
  shuffle(arr);
  test("size equals to 5", () =>
    expect(Array.length(arr)) |> toEqual(5)
  );
});