open Jest
open Expect
open Util

describe("uniqueId", () => {
  test("id is 1", () => 
    expect(uniqueId()) |> toEqual("1")
  )
  test("id is 2", () => 
    expect(uniqueId()) |> toEqual("2")
  )
  test("id is id-3", () => 
    expect(uniqueId(~prefix="id-", ())) |> toEqual("id-3")
  )
})