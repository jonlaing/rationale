open Jest;

open Expect;

open! Monoid;

describe("Monoid", () => {
  module M_String = Monoid.MakeGeneral({
      type t = string;
      let empty = "";
      let append = (a, b) => a ++ b;
      let rec concat = xs => switch xs {
      | [] => ""
      | [a, ...rs] => append(a, concat(rs))
      };
  });

  test("Associativity", () => 
    expect(
      M_String.concat(["a", M_String.concat(["b", "c"])])
    ) |> toEqual(
      M_String.concat([M_String.concat(["a", "b"]), "c"])
    )
  );

  test("Right identity", () => 
    expect(M_String.concat(["a", M_String.empty])) |> toEqual("a")
  );

  test("Left identity", () => 
    expect(M_String.concat([M_String.empty, "a"])) |> toEqual("a")
  );
  
  test("append", () => 
    expect(M_String.append("a", "b")) |> toEqual("ab")
  );

  test("concat", () => 
    expect(M_String.concat(["a", "b", "c"])) |> toEqual("abc")
  );
});
