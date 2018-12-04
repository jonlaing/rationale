open Jest;
open Expect;
open Function;

describe("debounce", () => {
  describe("immediate is false", () => {
    Jest.useFakeTimers();
    let num = ref(0);
    let add = debounce(~immediate=false, 30, () => num := num^ + 1);
    test("1: num is 0", () => {
      add();
      add();
      expect(num^) |> toEqual(0);
    });
    test("2: num is 1", () => {
      Jest.runTimersToTime(30);
      expect(num^) |> toEqual(1);
    });
    test("3: num is 1", () => {
      add();
      Jest.runTimersToTime(10);
      expect(num^) |> toEqual(1);
    });
    test("4: num is 2", () => {
      Jest.runTimersToTime(20);
      expect(num^) |> toEqual(2);
    });
  });
  describe("immediate is true", () => {
    Jest.useFakeTimers();
    let num = ref(0);
    let add = debounce(~immediate=true, 30, () => num := num^ + 1);
    test("1: num is 1", () => {
      add();
      add();
      expect(num^) |> toEqual(1);
    });
    test("2: num is 1", () => {
      Jest.runTimersToTime(30);
      expect(num^) |> toEqual(1);
    });
    test("3: num is 2", () => {
      add();
      Jest.runTimersToTime(10);
      expect(num^) |> toEqual(2);
    });
    test("4: num is 2", () => {
      Jest.runTimersToTime(20);
      expect(num^) |> toEqual(2);
    });
  });
});