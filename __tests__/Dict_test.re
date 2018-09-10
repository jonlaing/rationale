open Jest;

open Expect;

open Dict;

let dict: t(int) = [("a", 1), ("b", 2), ("c", 3)];

test("get", () => expect((get("b", dict), get("d", dict))) |> toEqual((Some(2), None)));

describe(
  "set",
  () => {
    test("existing key", () => expect(set("b", 5, dict) |> get("b")) |> toEqual(Some(5)));
    test("nonexistant key", () => expect(set("d", 5, dict) |> get("d")) |> toEqual(Some(5)))
  }
);

test(
  "eqProps",
  () =>
    expect((eqProps("a", dict, [("a", 1)]), eqProps("a", dict, [("a", 2)])))
    |> toEqual((true, false))
);

test("map", () => expect(map((x) => x * 2, dict)) |> toEqual([("a", 2), ("b", 4), ("c", 6)]));

test(
  "mapi",
  () => expect(mapi((k, _) => k, dict)) |> toEqual([("a", "a"), ("b", "b"), ("c", "c")])
);

test(
  "evolve",
  () => {
    let e = [("a", Function.identity), ("b", (x) => x * 2)];
    expect(evolve(e, dict)) |> toEqual([("a", 1), ("b", 4), ("c", 3)])
  }
);

test("has", () => expect((has("a", dict), has("d", dict))) |> toEqual((true, false)));

test(
  "invert",
  () => {
    let raceResultsByFirstName = [("first", "alice"), ("second", "jake"), ("third", "alice")];
    expect(invert(raceResultsByFirstName))
    |> toEqual([("alice", ["first", "third"]), ("jake", ["second"])])
  }
);

test("keys", () => expect(keys(dict)) |> toEqual(["a", "b", "c"]));

test("merge", () => expect(merge(dict, [("a", 2)])) |> toEqual([("a", 2), ("b", 2), ("c", 3)]));

test(
  "mergeWithKey",
  () =>
    expect(mergeWithKey((k, l, r) => k == "a" ? [l, r] : [r], dict, [("a", 2), ("b", 5)]))
    |> toEqual([("a", [1, 2]), ("b", [5])])
);

test(
  "mergeWith",
  () =>
    expect(mergeWith((l, r) => [l, r], dict, [("a", 2), ("b", 3)]))
    |> toEqual([("a", [1, 2]), ("b", [2, 3])])
);

test("omit", () => expect(omit(["a", "c"], dict)) |> toEqual([("b", 2)]));

test("pick", () => expect(pick(["a", "c"], dict)) |> toEqual([("a", 1), ("c", 3)]));

/* TODO: project */
test("values", () => expect(values(dict)) |> toEqual([1, 2, 3]));

test(
  "where",
  () => {
    let w0 = [("a", Util.eq(1)), ("c", (x) => x < 4)];
    let w1 = [("b", (x) => x mod 2 == 1)];
    expect((where(w0, dict), where(w1, dict))) |> toEqual((true, false))
  }
);

test(
  "where",
  () => {
    let w0 = [("a", 1), ("c", 3)];
    let w1 = [("b", 4)];
    expect((whereEq(w0, dict), whereEq(w1, dict))) |> toEqual((true, false))
  }
);

test("filter", () => expect(filter((v) => v > 1, dict)) |> toEqual([("b", 2), ("c", 3)]));

test("filteri", () => expect(filteri((k, _) => k == "a", dict)) |> toEqual([("a", 1)]));

test("fold_left", () => expect(
    fold_left((acc, _, v) => acc + v, 0, dict)
  ) |> toEqual(6)
);

test("fold_right", () => expect(
    fold_right((_, v, acc) => acc +   (v > 1 ? v : 0), dict, 0)
  ) |> toEqual(5)
);

test("unzip", () => expect(unzip(dict)) |> toEqual((["a", "b", "c"], [1, 2, 3])))

test("unzip", () => expect(unzip([])) |> toEqual(([], [])))