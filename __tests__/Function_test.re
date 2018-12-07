open Jest;
open Expect;
open Function;

describe("debounce", () => {
  describe("immediate is false", () => {
    let num = ref(0);
    let add = debounce(~immediate=false, () => num := num^ + 1, 300);
    test("1: num is 0", () => {
      add();
      add();
      expect(num^) |> toEqual(0);
    });
    testAsync("2: num is 1", finish =>
      delay(() => finish(expect(num^) |> toEqual(1)), 300) |> ignore
    );
    testAsync("3: num is 1", finish => {
      add();
      delay(() => finish(expect(num^) |> toEqual(1)), 100) |> ignore;
    });
    testAsync("4: num is 2", finish =>
      delay(() => finish(expect(num^) |> toEqual(2)), 200) |> ignore
    );
  });
  describe("immediate is true", () => {
    let num = ref(0);
    let add = debounce(~immediate=true, () => num := num^ + 1, 300);
    test("1: num is 1", () => {
      add();
      add();
      expect(num^) |> toEqual(1);
    });
    testAsync("2: num is 1", finish =>
      delay(() => finish(expect(num^) |> toEqual(1)), 300) |> ignore
    );
    testAsync("3: num is 2", finish => {
      add();
      delay(() => finish(expect(num^) |> toEqual(2)), 100) |> ignore;
    });
    testAsync("4: num is 2", (finish) => {
      delay(() => finish(expect(num^) |> toEqual(2)), 200) |> ignore ;
    });
  });
});

describe("throttle", () => {
  let num = ref(0);
  let add =
    throttle(
      () => {
        num := num^ + 1;
        num;
      },
      300,
    );
  test("1: num is 1", () => {
    add();
    add();
    expect(num^) |> toEqual(1);
  });
  testAsync("2: num is 2", finish =>
    delay(() => finish(expect(num^) |> toEqual(2)), 350) |> ignore
  );

  testAsync("3: num is 4", finish => {
    delay(
      () => {
        add();
        add();
        add();
        add();
      },
      350,
    )
    |> ignore;
    delay(() => finish(expect(num^) |> toEqual(4)), 700) |> ignore;
  });
});