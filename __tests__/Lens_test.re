open Jest;

open Expect;

open Lens;

type nestType = {
  innerBasic: int,
  innerSome: option(int),
  innerNone: option(int)
};

type recordType = {
  basic: int,
  nest: nestType,
  some: option(int),
  none: option(int),
  someNest: option(nestType),
  noneNest: option(nestType),
  list: list(int),
  emptyList: list(int),
  dict: Dict.t(int),
  tuple: (int, int)
};

let record = {
  basic: 1,
  nest: {innerBasic: 2, innerSome: Some(3), innerNone: None},
  some: Some(4),
  none: None,
  someNest: Some({innerBasic: 5, innerSome: Some(6), innerNone: None}),
  noneNest: None,
  list: [1, 2, 3],
  emptyList: [],
  dict: [("a", 7)],
  tuple: (8, 9)
};

let basicLens = make((a) => a.basic, (v, a) => {...a, basic: v});

let innerBasicLens = make((a) => a.innerBasic, (v, a) => {...a, innerBasic: v});

let nestLens = make((a) => a.nest, (v, a) => {...a, nest: v});

let someLens = make((a) => a.some, (v, a) => {...a, some: v}) >>- optional(0);

let noneLens = make((a) => a.none, (v, a) => {...a, none: v}) >>- optional(0);

let someNestLens =
  make((a) => a.someNest, (v, a) => {...a, someNest: v})
  >>- optional({innerBasic: 0, innerSome: Some(0), innerNone: None});

let noneNestLens =
  make((a) => a.noneNest, (v, a) => {...a, someNest: v})
  >>- optional({innerBasic: 0, innerSome: Some(0), innerNone: None});

let listLens = make((a) => a.list, (v, a) => {...a, list: v});

let emptyListLens = make((a) => a.emptyList, (v, a) => {...a, emptyList: v});

let dictLens = make((a) => a.dict, (v, a) => {...a, dict: v});

let aLens = prop("a") >>- optional(0);

let tupleLens = make((a) => a.tuple, (v, a) => {...a, tuple: v});

describe(
  "view",
  () => {
    test("basic", () => expect(view(basicLens, record)) |> toEqual(1));
    test("nest", () => expect(view(nestLens >>- innerBasicLens, record)) |> toEqual(2));
    test("some", () => expect(view(someLens, record)) |> toEqual(4));
    test("none", () => expect(view(noneLens, record)) |> toEqual(0));
    test("someNest", () => expect(view(someNestLens >>- innerBasicLens, record)) |> toEqual(5));
    test("noneNest", () => expect(view(noneNestLens >>- innerBasicLens, record)) |> toEqual(0));
    describe(
      "list",
      () => {
        test("head", () => expect(view(listLens >>- head >>- optional(0), record)) |> toEqual(1));
        test(
          "tail",
          () => expect(view(listLens >>- tail >>- optional([0]), record)) |> toEqual([2, 3])
        );
        test(
          "index",
          () => expect(view(listLens >>- index(1) >>- optional(0), record)) |> toEqual(2)
        )
      }
    );
    test("dict", () => expect(view(dictLens >>- aLens, record)) |> toEqual(7));
    describe(
      "tuple",
      () => {
        test("first", () => expect(view(tupleLens >>- first, record)) |> toEqual(8));
        test("second", () => expect(view(tupleLens >>- second, record)) |> toEqual(9))
      }
    )
  }
);

describe(
  "set",
  () => {
    test("basic", () => expect(set(basicLens, 5, record) |> view(basicLens)) |> toEqual(5));
    test(
      "nest",
      () =>
        expect(set(nestLens >>- innerBasicLens, 5, record) |> view(nestLens >>- innerBasicLens))
        |> toEqual(5)
    );
    test("some", () => expect(set(someLens, 5, record) |> view(someLens)) |> toEqual(5));
    test("none", () => expect(set(noneLens, 5, record) |> view(noneLens)) |> toEqual(5));
    test(
      "someNest",
      () =>
        expect(
          set(someNestLens >>- innerBasicLens, 7, record) |> view(someNestLens >>- innerBasicLens)
        )
        |> toEqual(7)
    );
    test(
      "noneNest",
      () =>
        expect(
          set(noneNestLens >>- innerBasicLens, 5, record) |> view(noneNestLens >>- innerBasicLens)
        )
        |> toEqual(0)
    );
    describe(
      "list",
      () => {
        test(
          "head",
          () =>
            expect(
              set(listLens >>- head >>- optional(0), 5, record)
              |> view(listLens >>- head >>- optional(0))
            )
            |> toEqual(5)
        );
        test(
          "tail",
          () =>
            expect(
              set(listLens >>- tail >>- optional([0]), [5, 5], record)
              |> view(listLens >>- tail >>- optional([0]))
            )
            |> toEqual([5, 5])
        );
        test(
          "index",
          () =>
            expect(
              set(listLens >>- index(1) >>- optional(0), 5, record)
              |> view(listLens >>- index(1) >>- optional(0))
            )
            |> toEqual(5)
        )
      }
    );
    test(
      "dict",
      () => expect(set(dictLens >>- aLens, 5, record) |> view(dictLens >>- aLens)) |> toEqual(5)
    );
    describe(
      "tuple",
      () => {
        test(
          "first",
          () =>
            expect(set(tupleLens >>- first, 5, record) |> view(tupleLens >>- first)) |> toEqual(5)
        );
        test(
          "second",
          () =>
            expect(set(tupleLens >>- second, 5, record) |> view(tupleLens >>- second))
            |> toEqual(5)
        )
      }
    )
  }
);

describe(
  "over",
  () => {
    let double = (x) => x * 2;
    test("basic", () => expect(over(basicLens, double, record) |> view(basicLens)) |> toEqual(2));
    test(
      "nest",
      () =>
        expect(
          over(nestLens >>- innerBasicLens, double, record) |> view(nestLens >>- innerBasicLens)
        )
        |> toEqual(4)
    );
    test("some", () => expect(over(someLens, double, record) |> view(someLens)) |> toEqual(8));
    test("none", () => expect(over(noneLens, double, record) |> view(noneLens)) |> toEqual(0));
    test(
      "someNest",
      () =>
        expect(
          over(someNestLens >>- innerBasicLens, double, record)
          |> view(someNestLens >>- innerBasicLens)
        )
        |> toEqual(10)
    );
    test(
      "noneNest",
      () =>
        expect(
          over(noneNestLens >>- innerBasicLens, double, record)
          |> view(noneNestLens >>- innerBasicLens)
        )
        |> toEqual(0)
    );
    describe(
      "list",
      () => {
        test(
          "head",
          () =>
            expect(
              over(listLens >>- head >>- optional(0), double, record)
              |> view(listLens >>- head >>- optional(0))
            )
            |> toEqual(2)
        );
        test(
          "tail",
          () =>
            expect(
              over(listLens >>- tail >>- optional([0]), List.map(double), record)
              |> view(listLens >>- tail >>- optional([0]))
            )
            |> toEqual([4, 6])
        );
        test(
          "index",
          () =>
            expect(
              over(listLens >>- index(1) >>- optional(0), double, record)
              |> view(listLens >>- index(1) >>- optional(0))
            )
            |> toEqual(4)
        )
      }
    );
    test(
      "dict",
      () =>
        expect(over(dictLens >>- aLens, double, record) |> view(dictLens >>- aLens)) |> toEqual(14)
    );
    describe(
      "tuple",
      () => {
        test(
          "first",
          () =>
            expect(over(tupleLens >>- first, double, record) |> view(tupleLens >>- first))
            |> toEqual(16)
        );
        test(
          "second",
          () =>
            expect(over(tupleLens >>- second, double, record) |> view(tupleLens >>- second))
            |> toEqual(18)
        )
      }
    )
  }
);