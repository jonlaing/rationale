open Jest;

open Expect;

open RList;

test("head", () =>
  expect((head([1]), head([]))) |> toEqual((Some(1), None))
);

test("tail", () =>
  expect((tail([1, 2]), tail([]))) |> toEqual((Some([2]), None))
);

test("nth", () =>
  expect((nth(0, [1]), nth(1, [1]))) |> toEqual((Some(1), None))
);

test("init", () =>
  expect((init([1, 2]), init([]))) |> toEqual((Some([1]), None))
);

test("last", () =>
  expect((last([1, 2]), init([]))) |> toEqual((Some(2), None))
);

test("any", () => {
  let even = x => x mod 2 == 0;
  expect((any(even, [1, 2, 3]), any(even, [1, 3, 5])))
  |> toEqual((true, false));
});

test("all", () => {
  let even = x => x mod 2 == 0;
  expect((all(even, [2, 4, 6]), all(even, [2, 3, 4])))
  |> toEqual((true, false));
});

test("append", () =>
  expect(append(1, [])) |> toEqual([1])
);

test("concat", () =>
  expect(concat([1], [0])) |> toEqual([0, 1])
);

describe("take", () => {
  test("too small", () =>
    expect(take(3, [1, 2])) |> toEqual([1, 2])
  );
  test("big enough", () =>
    expect(take(3, [1, 2, 3, 4])) |> toEqual([1, 2, 3])
  );
});

describe("takeLast", () => {
  test("too small", () =>
    expect(takeLast(3, [1, 2])) |> toEqual([1, 2])
  );
  test("big enough", () =>
    expect(takeLast(3, [1, 2, 3, 4])) |> toEqual([2, 3, 4])
  );
});

test("takeWhile", () =>
  expect(takeWhile(x => x < 5, [1, 2, 3, 4, 5])) |> toEqual([1, 2, 3, 4])
);

test("takeLastWhile", () =>
  expect(takeLastWhile(x => x > 1, [1, 2, 3, 4, 5]))
  |> toEqual([2, 3, 4, 5])
);

describe("drop", () => {
  test("too small", () =>
    expect(drop(3, [1, 2])) |> toEqual([])
  );
  test("big enough", () =>
    expect(drop(3, [1, 2, 3, 4])) |> toEqual([4])
  );
});

describe("dropLast", () => {
  test("too small", () =>
    expect(dropLast(3, [1, 2])) |> toEqual([])
  );
  test("big enough", () =>
    expect(dropLast(3, [1, 2, 3, 4])) |> toEqual([1])
  );
});

test("dropWhile", () =>
  expect(dropWhile(x => x < 5, [1, 2, 3, 4, 5])) |> toEqual([5])
);

test("dropLastWhile", () =>
  expect(dropLastWhile(x => x > 2, [1, 2, 3, 4, 5])) |> toEqual([1, 2])
);

test("dropRepeatsWith", () =>
  expect(
    dropRepeatsWith(
      Util.eqBy(abs),
      [1, (-1), 1, 3, 4, (-4), (-4), (-5), 5, 3, 3],
    ),
  )
  |> toEqual([1, 3, 4, (-5), 3])
);

test("dropRepeats", () =>
  expect(dropRepeats([1, 1, 1, 2, 3, 4, 4, 2, 2]))
  |> toEqual([1, 2, 3, 4, 2])
);

describe("splitAt", () => {
  test("split at 0", () =>
    expect(splitAt(0, [1, 2, 3])) |> toEqual(([], [1, 2, 3]))
  );
  test("split at 1", () =>
    expect(splitAt(1, [1, 2, 3])) |> toEqual(([1], [2, 3]))
  );
  test("split at last", () =>
    expect(splitAt(3, [1, 2, 3])) |> toEqual(([1, 2, 3], []))
  );
});

describe("adjust", () => {
  let add10 = x => x + 10;
  test("adjust first", () =>
    expect(adjust(add10, 0, [1, 2, 3])) |> toEqual([11, 2, 3])
  );
  test("adjust middle", () =>
    expect(adjust(add10, 1, [1, 2, 3])) |> toEqual([1, 12, 3])
  );
  test("adjust last", () =>
    expect(adjust(add10, 2, [1, 2, 3])) |> toEqual([1, 2, 13])
  );
  test("adjust past last", () =>
    expect(adjust(add10, 3, [1, 2, 3])) |> toEqual([1, 2, 3])
  );
});

describe("aperture", () => {
  test("zero", () =>
    expect(aperture(0, [1, 2, 3, 4, 5])) |> toEqual([[], [], [], [], []])
  );
  test("less than length", () =>
    expect(aperture(2, [1, 2, 3, 4, 5]))
    |> toEqual([[1, 2], [2, 3], [3, 4], [4, 5]])
  );
  test("greater than length", () =>
    expect(aperture(7, [1, 2, 3, 4, 5])) |> toEqual([])
  );
});

test("containsWith", () => {
  let even = x => x mod 2 == 0;
  expect((containsWith(even, [1, 2, 3]), containsWith(even, [1, 3, 5])))
  |> toEqual((true, false));
});

test("contains", () =>
  expect((contains(2, [1, 2, 3]), contains(2, [1, 3, 5])))
  |> toEqual((true, false))
);

test("endsWith", () =>
  expect((endsWith(3, [1, 2, 3]), endsWith(2, [1, 2, 3])))
  |> toEqual((true, false))
);

test("find", () =>
  expect((find(Util.eq(3), [1, 2, 3]), find(Util.eq(3), [1, 2])))
  |> toEqual((Some(3), None))
);

test("findIndex", () =>
  expect((
    findIndex(Util.eq(3), [1, 2, 3, 3]),
    findIndex(Util.eq(3), [1, 2]),
  ))
  |> toEqual((Some(2), None))
);

test("findLastIndex", () =>
  expect((
    findLastIndex(Util.eq(3), [1, 2, 3, 3]),
    findLastIndex(Util.eq(3), [1, 2]),
  ))
  |> toEqual((Some(3), None))
);

describe("groupWith", () => {
  test("equals", () =>
    expect(groupWith(Util.eq, [0, 1, 1, 2, 3, 5, 8, 13, 21]))
    |> toEqual([[0], [1, 1], [2], [3], [5], [8], [13], [21]])
  );
  test("add", () =>
    expect(groupWith((a, b) => a + 1 == b, [0, 1, 1, 2, 3, 5, 8, 13, 21]))
    |> toEqual([[0, 1], [1, 2, 3], [5], [8], [13], [21]])
  );
  test("div by 2", () =>
    expect(
      groupWith(
        (a, b) => a mod 2 == b mod 2,
        [0, 1, 1, 2, 3, 5, 8, 13, 21],
      ),
    )
    |> toEqual([[0], [1, 1], [2], [3, 5], [8], [13, 21]])
  );
});

test("indexOf", () =>
  expect((indexOf(2, [1, 2, 3]), indexOf(5, [1, 2, 3])))
  |> toEqual((Some(1), None))
);

test("lastIndexOf", () =>
  expect((lastIndexOf(2, [1, 2, 2]), lastIndexOf(5, [1, 2, 2])))
  |> toEqual((Some(2), None))
);

test("insert", () =>
  expect(insert(2, 5, [1, 2, 3])) |> toEqual([1, 2, 5, 3])
);

test("insertAll", () =>
  expect(insertAll(2, [5, 5], [1, 2, 3])) |> toEqual([1, 2, 5, 5, 3])
);

test("intersperse", () =>
  expect(intersperse("n", ["ba", "a", "a"]))
  |> toEqual(["ba", "n", "a", "n", "a"])
);

test("join", () =>
  expect(join(":", ["1", "2", "3"])) |> toEqual("1:2:3")
);

test("none", () => {
  let even = x => x mod 2 == 0;
  expect((none(even, [1, 3, 5]), none(even, [2, 4, 6])))
  |> toEqual((true, false));
});

test("rangeInt", () =>
  expect(rangeInt(1, 0, 3)) |> toEqual([0, 1, 2, 3])
);

test("reduceWhile", () => {
  let isOdd = (_, x) => x mod 2 == 1;
  let xs = [1, 3, 5, 60, 777, 800];
  expect(reduceWhile(isOdd, (acc, x) => acc + x, 0, xs)) |> toEqual(9);
});

test("reject", () => {
  let isOdd = x => x mod 2 == 1;
  expect(reject(isOdd, [1, 2, 3])) |> toEqual([2]);
});

test("remove", () =>
  expect(remove(1, 1, [1, 2, 3])) |> toEqual([1, 3])
);

test("repeat", () =>
  expect(repeat(1, 3)) |> toEqual([1, 1, 1])
);

test("scan", () => {
  let numbers = [1, 2, 3, 4];
  let multiply = (x, y) => x * y;
  expect(scan(multiply, 1, numbers)) |> toEqual([1, 1, 2, 6, 24]);
});

test("slice", () =>
  expect(slice(1, 3, [1, 2, 3, 4, 5])) |> toEqual([2, 3, 4])
);

test("splitEvery", () =>
  expect(splitEvery(2, [1, 2, 3, 4, 5, 6]))
  |> toEqual([[1, 2], [3, 4], [5, 6]])
);

test("splitWhen", () => {
  let even = x => x mod 2 == 0;
  expect(splitWhen(even, [1, 2, 3])) |> toEqual(([1], [2, 3]));
});

test("startsWith", () =>
  expect((startsWith(1, [1, 2, 3]), startsWith(2, [1, 2, 3])))
  |> toEqual((true, false))
);

test("times", () =>
  expect(times(Function.identity, 5)) |> toEqual([0, 1, 2, 3, 4])
);

test("uniqBy", () =>
  expect(uniqBy(abs, [(-1), (-5), 2, 10, 1, 2]))
  |> toEqual([(-1), (-5), 2, 10])
);

test("uniq", () =>
  expect(uniq([1, 1, 2, 3, 2, 1, 3])) |> toEqual([1, 2, 3])
);

test("union", () =>
  expect(union([1, 2, 3], [2, 3, 4])) |> toEqual([1, 2, 3, 4])
);

test("update", () =>
  expect(update(5, 1, [1, 2, 3])) |> toEqual([1, 5, 3])
);

test("without", () =>
  expect(without([2, 4], [1, 2, 3, 4, 5])) |> toEqual([1, 3, 5])
);

test("zipWith", () => {
  let multiply = (a, b) => a * b;
  expect(zipWith(multiply, [1, 2, 3], [4, 5, 6])) |> toEqual([4, 10, 18]);
});

test("difference", () =>
  expect(difference([1, 2, 3], [3, 4, 5])) |> toEqual([1, 2, 4, 5])
);

test("intersection", () =>
  expect(intersection([1, 2, 3], [3, 4, 5])) |> toEqual([3])
);

test("zip", () =>
  expect(zip([1, 2, 3], [4, 5, 6]))
  |> toEqual([(1, 4), (2, 5), (3, 6)])
);

test("create", () =>
  expect((create(x => x, -1), create(x => x * 2, 4)))
  |> toEqual(([], [0, 2, 4, 6]))
);

test("filter_opt", () =>
  expect((
    filter_opt([]),
    filter_opt([Some(3), None, Some(4), None, Some(5)]),
  ))
  |> toEqual(([], [3, 4, 5]))
);

test("reduce", () =>
  expect((reduce((+), []), reduce((+), [1, 2, 3])))
  |> toEqual((None, Some(6)))
);

test("merge", () => {
  let compare = (a, b) => a - b;
  expect((merge(compare, [], []), merge(compare, [1, 3, 5], [2, 4])))
  |> toEqual(([], [1, 2, 3, 4, 5]));
});
